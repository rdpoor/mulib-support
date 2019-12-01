/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// =============================================================================
// includes

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "driver_init.h"
#include "mu_sched.h"
#include "mu_sleep.h"
#include "mu_string.h"
#include "mu_task.h"
#include "mu_time.h"
#include "mu_port.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// types and definitions

// #define USE_CHEAP_FTOA

// the LED is on when its GPIO pin is low
#define LED_ON false
#define LED_OFF true

// Define repetition interval for toggling LED
#define LED_ON_TIME 0.005
#define LED_OFF_TIME 0.995

// Define repetition interval for updating the display
#define SCREEN_UPDATE_INTERVAL 5.0

// # of interrupts notifications we can handle between calls to sched_step().
// Must be a power of two.
#define ISR_QUEUE_POOL_SIZE 8

// character sequence to home cursor and clear to end of screen
#define CLEAR_SCREEN "\e[1;1H\e[2J"

// allocate a buffer for writes to the screen
#define WRITE_BUFFER_SIZE (80)

#define READ_BUFFER_SIZE (10)

// define the number of tasks we display on the screen
#define N_TASKS (5)

typedef enum {
  TX_BUFFER_AVAILABLE,
  TX_BUFFER_USER_OWNS,
  TX_BUFFER_ISR_OWNS
} tx_buffer_owner_t;

// Gather all the state into a singleton struct.
typedef struct {
  mu_sched_t sched;
  mu_task_t idle_task;
  mu_task_t led_task;
  mu_task_t screen_redraw_task;
  mu_task_t screen_update_task;
  mu_task_t keyboard_monitor_task;
  bool low_power_mode;
  mu_string_t tx_buffer;
  int screen_state;
  tx_buffer_owner_t tx_buffer_owner_a;
  tx_buffer_owner_t tx_buffer_owner_b;
} demo3_t;

// =============================================================================
// forward declarations

/**
 * \brief mulib function called when the scheduler has nothing to run.
 */
static void idle_task_fn(void *self, void *arg);

/**
 * \brief led_task_fn is called whenever the s_led_task is triggered (see
 * below).  It will toggle the LED on the SAMD21 XPlained board and then
 * reschedule s_led_task to be called again.
 */
static void led_task_start(demo3_t *demo3);
static void led_task_stop(demo3_t *demo3);
static void led_task_fn(void *self, void *arg);

/**
 * \brief screen_update_fn starts the screen_redraw task.
 *
 * If screen updates are enabled, screen_update_fn starts the screen_redraw
 * task and reschedules itself to trigger in SCREEN_UPDATE_INTERVAL seconds.
 */
 static void screen_update_task_start(demo3_t *demo3);
 static void screen_update_task_stop(demo3_t *demo3);
 static void screen_update_fn(void *self, void *arg);

/**
 * \brief screen_redraw_fn is called when the s_screen_redraw task triggers.
 * It will update the display to show the state of the mulib tasks.
 */
static void screen_redraw_task_start(demo3_t *demo3);
static void screen_redraw_fn(void *self, void *arg);
static void sprintf_task_status(demo3_t *demo3, mu_string_t *s, mu_task_t *task);
static char get_task_state(demo3_t *demo3, mu_task_t *task);

/**
 * \brief keyboard_monitor_fn is called whenever the s_keyboard_task is called.
 * It will read keyboard input and update the state of the demo program.
 */
static void keyboard_monitor_fn(void *self, void *arg);
static void echo_user_string(char *s);

/**
 * \brief Set/get low-power mode.
 *
 * When enabled, the idle task will put the processor into STANDBY mode for the
 * duration of the idle task.  In low-power mode, a button press is required to
 * start listening on the keybaord again.
 */
static void set_low_power_mode(demo3_t *demo3, bool enable);
static bool is_low_power_mode(demo3_t *demo3);

static void start_periodic_task(demo3_t *demo3, mu_task_t *task);
static void stop_periodic_task(demo3_t *demo3, mu_task_t *task);

/**
 * Called (from interrupt level) when the user button is pushed
 */
static void button_on_PA15_pressed(void);

/**
 * Right justify a string within a field, padding on left with padchar.
 */
static const char *rjust(const char *s, int width, char padchar);

/**
 * Print a float without using the %f directive.
 */
#ifdef USE_CHEAP_FTOA
static const char *cheap_ftoa(float x);
#endif

// ==========
// Board specific serial IO functions
// ==========

static void board_serial_init(void);

/**
 * @brief Return a string buffer to write into, or NULL if not available.
 */
static mu_string_t *board_serial_claim_tx_buffer(void);

/**
 * @brief Pass a string to the serial driver for writing.  buf *must* be a
 * mu_string previously returned by board_serial_get_write_buffer().
 */
static int board_serial_write(mu_string_t *buf);

/**
 * @brief Return true as long as there is data yet to be printed.
 */
static bool board_serial_tx_is_active(void);

/**
 * @brief Read as many serial characters as are available into s
 */
static int board_serial_read(mu_string_t *s);

/**
 * @brief Arrive here at interrupt level when the previous io_write completes.
 */
static void txc_cb();

/**
 * @brief Arrive here at interrupt level when chars are available for reading.
 */
static void rxc_cb();

/*
 * @brief Arrive here at interrupt level at a serial I/O error.
 */
static void err_cb();

// =============================================================================
// local storage

/**
 * \brief Allocate storage for the ISR task queue.
 */
static mu_queue_obj_t s_isr_queue_pool[ISR_QUEUE_POOL_SIZE];

/**
 * \brief storage for double-buffered serial writes
 */
static char s_write_buffer_a[WRITE_BUFFER_SIZE];
static char s_write_buffer_b[WRITE_BUFFER_SIZE];

/**
 * \brief The demo object.  We require a static object for the ISR rourintes.
 */
static demo3_t s_demo3;

// =============================================================================
// public code

int main(void) {
  demo3_t *demo3 = &s_demo3;

  // Perform board-specific initialization
  atmel_start_init();

  // It appears that Atmel START cannot enable pullups on GPIO pins that are
  // configured as external interrupt sources.
  gpio_set_pin_pull_mode(USER_BUTTON, GPIO_PULL_UP);

  // Perform port-specific initialization needed by mulib
  mu_port_init();

  board_serial_init();

  // Initialize the scheduler along with storage for tasks queued from interrupt
  // level.
  mu_sched_init(&demo3->sched, s_isr_queue_pool, ISR_QUEUE_POOL_SIZE);

  // Initialize and install the idle task in the scheduler.
  mu_task_init(&demo3->idle_task, idle_task_fn, demo3, "Idle");
  mu_sched_set_idle_task(&demo3->sched, &demo3->idle_task);

  // Initialize the LED task.  The task will be initially scheduled in a call to
  // set_led_task_enable(true).  Thereafter, led_task_fn() will reschedule it.
  mu_task_init(&demo3->led_task, led_task_fn, demo3, "LED");

  // Initialize the screen redraw task.  It will be periodically triggered by
  // the screen_update task.
  mu_task_init(&demo3->screen_redraw_task, screen_redraw_fn, demo3, "Redraw");
  demo3->screen_state = 0;

  // Initialize the screen update task.  It will initially be scheduled by a
  // call to set_screen_update_enabled(true), thereafter it will reschedule
  // itself.
  mu_task_init(&demo3->screen_update_task, screen_update_fn, demo3, "Update");

  // Initialize the keyboard monitor task.  This task is called whenever a
  // serial character becomes available via the read callback.
  mu_task_init(&demo3->keyboard_monitor_task,
                     keyboard_monitor_fn,
                     demo3,
                     "Keyboard");

  // set up application defaults
  set_low_power_mode(demo3, false);

  ext_irq_register(PIN_PA15, button_on_PA15_pressed);

  // queue initial tasks
  led_task_start(demo3);
  screen_update_task_start(demo3);

  // Start the scheduler
  while (1) {
    mu_sched_step(&demo3->sched);
  }
  // Like Charlie on the MTA, it will never return...
  return 0;
}

// =============================================================================
// private code

// ==========
// Idle task
// ==========

// Arrive here when the schedule has nothing runnable.  It puts the processor
// into standby mode until an interrupt wakes the processor again.  Using this
// technique, average current on the SAMD21 is reduced from 1.2mA to about 24
// uA.  And since it's installed as the idle task, no changes are required to
// the user code to take advantage of the power savings.
//
// Things to note:
// - Arriving here, all immediate tasks will have been processed (because the
//   idle task is called only when there are no runnable tasks).
// - If the task list is empty it means that there are no scheduled tasks.  In
//   this case, the idle task will call mu_sleep_indefinitely() and only an
//   external interrupt will wake the processor.
// - If the task list is not empty, the first task is the "soonest", in which
//   case the idle task will call mu_sleep_until() to wake the processor on an
//   RTC match interrupt.
static void idle_task_fn(void *self, void *arg) {
  // Note: here (and in other task_fn's) we could have used the global s_demo3
  // object and s_demo3.sched, but we want to show that they're available as
  // self and arg respectively.
  demo3_t *demo3 = (demo3_t *)self;
  mu_sched_t *sched = (mu_sched_t *)arg;

  if (!is_low_power_mode(demo3)) {
    // Don't sleep if we are not in low-power mode
    return;
  } else if (board_serial_tx_is_active()) {
    // Don't sleep until the serial port finishes writing data
    return;
  } else if (demo3->screen_state != -1) {
    // don't sleep if we're haven't completed the screen update
    return;
  }

  mu_task_t *next_task = mu_sched_get_tasks(sched);

  if (next_task == NULL) {
    mu_sleep_indefinitely();
  } else {
    mu_sleep_until(mu_task_time(next_task));
  }

  asm("nop");
}

// ==========
// LED task
// ==========

static void led_task_start(demo3_t *demo3) {
  start_periodic_task(demo3, &demo3->led_task);
}

static void led_task_stop(demo3_t *demo3) {
  gpio_set_pin_level(LED0, true);  // turn off LED (low-true logic)
  stop_periodic_task(demo3, &demo3->led_task);
}

// This is the function that gets called whenever the s_led_task is triggered.
static void led_task_fn(void *self, void *arg) {
  demo3_t *demo3 = (demo3_t *)self;
  mu_sched_t *sched = (mu_sched_t *)arg;
  float duration;

  if (gpio_get_pin_level(LED0) == LED_ON) {
    duration = LED_OFF_TIME;
  } else {
    duration = LED_ON_TIME;
  }
  gpio_toggle_pin_level(LED0);

  // Reschedule the LED task to trigger duration seconds in the future. Note
  // that in order to prevent timing drift, the task time is computed as
  // (prev_task_time + interval) rather than (now + interval).
  mu_task_advance_time(&demo3->led_task,
                       mu_time_seconds_to_duration(duration));
  mu_sched_add(sched, &demo3->led_task);
}

// ==========
// Screen Update task
// ==========

static void screen_update_task_start(demo3_t *demo3) {
  start_periodic_task(demo3, &demo3->screen_update_task);
}

static void screen_update_task_stop(demo3_t *demo3) {
  stop_periodic_task(demo3, &demo3->screen_update_task);
}

static void screen_update_fn(void *self, void *arg) {
  demo3_t *demo3 = (demo3_t *)self;
  mu_sched_t *sched = (mu_sched_t *)arg;

  screen_redraw_task_start(demo3);

  // Reschedule the screen update task to trigger SCREEN_UPDATE_INTERVAL
  // seconds in the future.
  mu_task_advance_time(&demo3->screen_update_task,
                       mu_time_seconds_to_duration(SCREEN_UPDATE_INTERVAL));
  mu_sched_add(sched, &demo3->screen_update_task);
 }

// ==========
// Screen Redraw Task
// ==========

static void screen_redraw_task_start(demo3_t *demo3) {
  demo3->screen_state = 0;
  mu_task_set_immediate(&demo3->screen_redraw_task);
  mu_sched_add(&demo3->sched, &demo3->screen_redraw_task);
}

/**
 * The screen redraw task.  Draws one line of text at each call, using
 * demo3->screen_state to determine which line.
 */
static void screen_redraw_fn(void *self, void *arg) {
  demo3_t *demo3 = (demo3_t *)self;
  // mu_sched_t *sched = (mu_sched_t *)arg;

  static const char *strings[] = {
    CLEAR_SCREEN,                                                     // 0
    "\r\nmulibDemo3\r\n",                                             // 1
    "\r\n>     Task: State    At     Calls  MaxLatency     Runtime",  // 2
    "\r\n*********************************************************",  // 3
    "\r\n\r\nLow-power idle mode: press user button to enter full-power mode.",  // 4 + NTASKS
    "\r\n\r\nFull-power mode. Select one:",                      //  4 + NTASKS
    "\r\n b: activate blink task",                               //  5 + NTASKS
    "\r\n B: suspend blink task",                                //  6 + NTASKS
    "\r\n d: activate display task",                             //  7 + NTASKS
    "\r\n D: suspend display task",                              //  8 + NTASKS
    "\r\n p: enter low-power idle mode"                          //  9 + NTASKS
  };
  mu_string_t *buf;

  if (demo3->screen_state == -1) {
    // waiting for someone to start this task.
    return;
  }

  // From here: get a buffer to write into, write into it and post it.
  // The async driver will print the contents and re-trigger this task
  // when the next buffer comes available.

  if ((buf = board_serial_claim_tx_buffer()) == NULL) {
    // no write buffer is available yet.  A callback will restart the task.
    return;
  }

  if (demo3->screen_state < 4) {
    // display the first three lines
    mu_string_sprintf(buf, "%s", strings[demo3->screen_state]);
    demo3->screen_state += 1;

  } else if (demo3->screen_state == 4) {
  // display info on the idle task
  sprintf_task_status(demo3, buf, &demo3->idle_task);
  demo3->screen_state += 1;

  } else if (demo3->screen_state == 5) {
  // display info on the led task
  sprintf_task_status(demo3, buf, &demo3->led_task);
  demo3->screen_state += 1;

  } else if (demo3->screen_state == 6) {
  // display info on the screen redraw task
  sprintf_task_status(demo3, buf, &demo3->screen_redraw_task);
  demo3->screen_state += 1;

  } else if (demo3->screen_state == 7) {
  // display info on the screen update task
  sprintf_task_status(demo3, buf, &demo3->screen_update_task);
  demo3->screen_state += 1;

  } else if (demo3->screen_state == 8) {
  // display info on the keyboard task
  sprintf_task_status(demo3, buf, &demo3->keyboard_monitor_task);
  demo3->screen_state += 1;

  } else if (is_low_power_mode(demo3)) {
      mu_string_sprintf(buf, "%s", strings[demo3->screen_state - N_TASKS]);
      demo3->screen_state = -1;  // no more text to display -- return to idle state

  } else {
      mu_string_sprintf(buf, "%s", strings[demo3->screen_state - N_TASKS + 1]);
      demo3->screen_state += 1;
      if (demo3->screen_state > N_TASKS + 9) {
        demo3->screen_state = -1;  // no more text to display -- return to idle state
      }
  }
  // Pass the buffer for printing to the screen.
  board_serial_write(buf);
}

static void sprintf_task_status(demo3_t *demo3, mu_string_t *s, mu_task_t *task) {
  static char buf[128];

  mu_string_sprintf(s,
                    "\r\n%s: %c",
                    rjust(mu_task_name(task), 10, ' '),
                    get_task_state(demo3, task));
  if (mu_task_is_immediate(task)) {
    mu_string_sprintf(s, "%s", rjust("immediate", 10, ' '));
  } else {
    sprintf(buf, "%u", (int)mu_task_time(task));
    mu_string_sprintf(s, "%s", rjust(buf, 10, ' '));
  }
  sprintf(buf, " %u", mu_task_call_count(task));
  mu_string_sprintf(s, "%s", rjust(buf, 10, ' '));
#ifdef USE_CHEAP_FTOA
  sprintf(buf, " %s", cheap_ftoa(mu_task_max_latency(task)));
  mu_string_sprintf(s, "%s", rjust(buf, 12, ' '));
  sprintf(buf, " %s", cheap_ftoa(mu_task_runtime(task)));
  mu_string_sprintf(s, "%s", rjust(buf, 12, ' '));
#else
mu_string_sprintf(s, " %11.5f", mu_task_max_latency(task));
mu_string_sprintf(s, " %11.5f", mu_task_runtime(task));
#endif
}

/*
 * A = active (current)
 * I = inactive (not in sched)
 * R = runnable (ready to run)
 * P = pending (not yet runnable)
 */
static char get_task_state(demo3_t *demo3, mu_task_t *task) {
  if (mu_sched_current_task(&demo3->sched) == task) {
    return 'A'; // this task is currently running
  } else if (!mu_sched_has_task(&demo3->sched, task)) {
    return 'I'; // this task is not scheduled
  } else if (mu_task_is_runnable(task, mu_time_now())) {
    return 'R'; // this task is ready to run
  } else {
    return 'P'; // this task is in the schedule but its time hasn't arrived
  }
}

// ==========
// Keyboard Monitor task
// ==========

/**
 * Characters arriving on the serial input will trigger keyboard_monitor_task.
 */
void keyboard_monitor_fn(void *self, void *arg) {
  demo3_t *demo3 = (demo3_t *)self;
  // mu_sched_t *sched = (mu_sched_t *)arg;

  char buf[READ_BUFFER_SIZE];
  mu_string_t s;
  mu_cstring_init(&s, buf, READ_BUFFER_SIZE);

  // TODO find return type for io_read()
  int err = board_serial_read(&s);
  if (err < 0) {
    // some sort of error
    return;
  }

  int n_read = mu_string_length(&s);
  if (n_read == 0) {
	  return;
  }

  if (n_read < mu_string_capacity(&s)) {
	mu_string_buf(&s)[n_read] = '\0';  // Null terminate
  }

  // In this example, we only pay attention to the first character
  switch(mu_string_buf(&s)[0]) {
    case 'b':  // activate blink
    led_task_start(demo3);
    echo_user_string("b: activate blink task");
    break;
    case 'B':  // deactivate blink
    led_task_stop(demo3);
    echo_user_string("B: suspend blink task");
    break;
    case 'd':  // activate display
    screen_update_task_start(demo3);
    echo_user_string("d: activate display task");
    break;
    case 'D':  // deactivate display
    screen_update_task_stop(demo3);
    echo_user_string("D: suspend display task");
    break;
    case 'p':  // low power mode
    set_low_power_mode(demo3, true);
    echo_user_string("p: enter low-power idle mode");
    break;
    default:   // echo typed char
    echo_user_string(mu_string_buf(&s));
  }
  // force an immediate update to the screen
  screen_redraw_task_start(demo3);
}

/**
 * If async is available, write now.
 */
static void echo_user_string(char *s) {
  // TBD...
}

static void set_low_power_mode(demo3_t *demo3, bool enable) {
  demo3->low_power_mode = enable;
}

static bool is_low_power_mode(demo3_t *demo3) {
  return demo3->low_power_mode;
}

static void start_periodic_task(demo3_t *demo3, mu_task_t *task) {
  if (mu_sched_has_task(&demo3->sched, task)) {
    // already in schedule -- no action needed
  } else {
    // set initial time and add to schedule
    mu_sched_add(&demo3->sched, mu_task_set_time(task, mu_time_now()));
  }
}

static void stop_periodic_task(demo3_t *demo3, mu_task_t *task) {
  // remove from schedule (if present)
  mu_sched_remove(&demo3->sched, task);
}

static void button_on_PA15_pressed(void) {
  set_low_power_mode(&s_demo3, false);
  mu_sched_add_from_isr(&s_demo3.sched, &s_demo3.keyboard_monitor_task);
}

// ==========
// utilities
// ==========

/**
 * /brief Right justify s to width chars wide, padding on left with spaces.
 *
 * Results are stored in static memory and must be copied elsewhere before
 * the next call.
 */
static const char *rjust(const char *s, int width, char padchar) {
  static char buf[32];
  int padding = width - strlen(s);
  if (padding <= 0)
    return s;

  memset(buf, padchar, padding);
  strcpy(&buf[padding], s);
  return buf;
}

#ifdef USE_CHEAP_FTOA
// print five digits to the right of the decimal point
#define FTOA_PRECISION 100000

/**
 * \brief cheap float to ascii conversion
 *
 * Restrictions:
 * - Output is written to a static buffer.  The caller must copy the contents
 *   to somewhere before the next call to cheap_ftoa()
 * - Doesn't handle negative numbers (but it's easy to add if your application
 *   requires it).
 * - Results with anything that overflows an int are undefined, including NaN
 *   and Inf.
 */
static const char *cheap_ftoa(float x) {
  static char buf[20];
  static char fbuf[6];                           // five digits plus null
  int c = (x * FTOA_PRECISION) + 0.5;            // handle rounding
  int i = c / FTOA_PRECISION;                    // the integer part
  int f = c - (i * FTOA_PRECISION);              // the fractional part
  sprintf(fbuf, "%u", f);                        // fractional part as ascii
  sprintf(buf, "%u.%s", i, rjust(fbuf, 5, '0')); // pad with leading '0'
  return buf;
}
#endif

// ==========
// board-specific serial IO
// ==========

static void board_serial_init() {
  s_demo3.tx_buffer_owner_a = TX_BUFFER_AVAILABLE;
  s_demo3.tx_buffer_owner_b = TX_BUFFER_AVAILABLE;

  usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, rxc_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, txc_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, err_cb);

  usart_async_enable(&EDBG_COM);
}

/**
 * @brief Return a string buffer for the user to write into.
 */
static mu_string_t *board_serial_claim_tx_buffer(void) {
  if (TX_BUFFER_AVAILABLE == s_demo3.tx_buffer_owner_a) {
    mu_string_init(&s_demo3.tx_buffer, s_write_buffer_a, WRITE_BUFFER_SIZE);
    s_demo3.tx_buffer_owner_a = TX_BUFFER_USER_OWNS;
    return &s_demo3.tx_buffer;
  } else if (TX_BUFFER_AVAILABLE == s_demo3.tx_buffer_owner_b) {
    mu_string_init(&s_demo3.tx_buffer, s_write_buffer_b, WRITE_BUFFER_SIZE);
    s_demo3.tx_buffer_owner_b = TX_BUFFER_USER_OWNS;
    return &s_demo3.tx_buffer;
  } else {
    // neither buffer available
    return NULL;
  }
}

/**
 * @brief Pass the contents of the active buffer to the serial write routine,
 * and switch to using the other buffer.
 */
static int board_serial_write(mu_string_t *s) {
  int ret;
  struct io_descriptor *io;

  if (mu_string_buf(s) == s_write_buffer_a) {
    s_demo3.tx_buffer_owner_a = TX_BUFFER_ISR_OWNS;
  } else if (mu_string_buf(s) == s_write_buffer_b) {
    s_demo3.tx_buffer_owner_b = TX_BUFFER_ISR_OWNS;
  } else {
    // TODO: error return
  }

  // initiate asynchronous write
  usart_async_get_io_descriptor(&EDBG_COM, &io);
  ret = io_write(io, (const uint8_t *const)mu_cstring_data(s), mu_string_length(s));

  return ret;
}

static bool board_serial_tx_is_active(void) {
  return s_demo3.tx_buffer_owner_a == TX_BUFFER_ISR_OWNS ||
         s_demo3.tx_buffer_owner_b == TX_BUFFER_ISR_OWNS;
  // return (usart_async_get_status(&EDBG_COM, NULL) == ERR_BUSY) &&
  //        !usart_async_is_tx_empty(&EDBG_COM);
}

/**
 * @brief Read as many serial characters as are available into s
 */
static int board_serial_read(mu_string_t *s) {
  int ret;  // non-negative signifies # chars read, negative signfies error.
  struct io_descriptor *io;

  usart_async_get_io_descriptor(&EDBG_COM, &io);
  ret = io_read(io, (uint8_t *const )mu_string_buf(s), mu_string_capacity(s));

  if (ret >= 0) {
    mu_string_slice(s, 0, ret, NULL);
  } else {
    mu_string_reset(s);  // start = end = 0.
  }

  return ret;
}

/**
 * @brief Arrive here at interrupt level when the previous io_write completes.
 */
static void txc_cb() {
  if (s_demo3.tx_buffer_owner_a == TX_BUFFER_ISR_OWNS) {
    s_demo3.tx_buffer_owner_a = TX_BUFFER_AVAILABLE;
  } else if (s_demo3.tx_buffer_owner_b == TX_BUFFER_ISR_OWNS) {
    s_demo3.tx_buffer_owner_b = TX_BUFFER_AVAILABLE;
  }
  mu_sched_add_from_isr(&s_demo3.sched, &s_demo3.screen_redraw_task);
}

/**
 * @brief Arrive here at interrupt level when chars are available for reading.
 */
static void rxc_cb() {
  mu_sched_add_from_isr(&s_demo3.sched, &s_demo3.keyboard_monitor_task);
}

static void err_cb() {
  s_demo3.tx_buffer_owner_a = TX_BUFFER_AVAILABLE;
  s_demo3.tx_buffer_owner_b = TX_BUFFER_AVAILABLE;
  asm("nop");
}
