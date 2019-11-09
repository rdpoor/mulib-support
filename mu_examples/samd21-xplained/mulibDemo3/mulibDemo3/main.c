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
#include "mu_async.h"
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

// Define repetition interval for toggling LED
#define LED_UPDATE_INTERVAL 0.5

// Define repetition interval for updating the display
#define SCREEN_UPDATE_INTERVAL 5.0

// # of interrupts notifications we can handle between calls to sched_step().
// Must be a power of two.
#define ISR_QUEUE_POOL_SIZE 8

// character sequence to home cursor and clear to end of screen
#define CLEAR_SCREEN "\e[1;1H\e[2J"

// allocate a buffer for writes to the screen
#define WRITE_BUFFER_SIZE (80*2)

#define READ_BUFFER_SIZE 10

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
static void led_task_start();
static void led_task_stop();
static void led_task_fn(void *self, void *arg);

/**
 * \brief screen_update_fn starts the screen_redraw task.
 *
 * If screen updates are enabled, screen_update_fn starts the screen_redraw
 * task and reschedules itself to trigger in SCREEN_UPDATE_INTERVAL seconds.
 */
 static void screen_update_task_start();
 static void screen_update_task_stop();
 static void screen_update_fn(void *self, void *arg);

/**
 * \brief screen_redraw_fn is called when the s_screen_redraw task triggers.
 * It will update the display to show the state of the mulib tasks.
 */
static void screen_redraw_task_start();
static void screen_redraw_fn(void *self, void *arg);
static void sprintf_task_status(mu_string_t *s, mu_task_t *task);
static char get_task_state(mu_task_t *task);

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
static void set_low_power_mode(bool enable);
static bool is_low_power_mode();

static void start_periodic_task(mu_task_t *task);
static void stop_periodic_task(mu_task_t *task);

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
static const char *cheap_ftoa(float x);

// =============================================================================
// local storage

/**
 * \brief s_sched is an instance of the mulib scheduler.  You schedule an task
 * to trigger at some point in the future through a call to mu_sched_queue().
 */
static mu_sched_t s_sched;

/**
 * \brief Allocate storage for the ISR task queue.
 */
static mu_queue_obj_t s_isr_queue_pool[ISR_QUEUE_POOL_SIZE];

/**
 * Serial I/O.  This works in conjunction with the scheduler to provide double
 * buffered writes and to handle serial I/O events generated at interrupt level.
 */
static mu_async_t s_async;

/*
 * \brief s_idle_task is the task that the scheduler runs when nothing else is
 * runnable.  In this case, we put the processor into stanby mode until an
 * external interrupt or RTC match interrupt wakes the processor.
 */
static mu_task_t s_idle_task;

/**
 * \brief When triggered by the scheduler, s_led_task will call led_task_fn()
 * to toggle the LED.
 */
static mu_task_t s_led_task;

/**
 * \brief s_screen_redraw_task is responsible for updating the screen.
 */
static mu_task_t s_screen_redraw_task;

/**
 * \brief s_screen_update_task is responsible for periodically starting the
 * screen redraw task.
 */
static mu_task_t s_screen_update_task;

/**
 * \brief s_keyboard_monitor_task is triggered on keyboard input and is
 * responsible for reading user input.
 */
static mu_task_t s_keyboard_monitor_task;

/**
 * \brief State flags for the application.
 */
static bool s_low_power_mode;

static char s_write_buffer[WRITE_BUFFER_SIZE];

static char s_read_buffer[READ_BUFFER_SIZE];
static mu_string_t s_read_string;

/**
 * We create an explicit list of tasks for the sole purpose of being able to
 * display their status.
 */
static mu_task_t *s_tasks[] = {
  &s_idle_task,
  &s_led_task,
  &s_screen_redraw_task,
  &s_screen_update_task,
  &s_keyboard_monitor_task
};

const size_t N_TASKS = sizeof(s_tasks) / sizeof(mu_task_t *);

/**
 * Track which line is being printed to the screen.
 */
int s_screen_state;

// =============================================================================
// public code

int main(void) {
  // Perform board-specific initialization
  atmel_start_init();

  // It appears that Atmel START cannot enable pullups on GPIO pins that are
  // configured as external interrupt sources.
  gpio_set_pin_pull_mode(USER_BUTTON, GPIO_PULL_UP);

  // Perform port-specific initialization needed by mulib
  mu_port_init();

  // set up a string buffer for read operations
  mu_string_init(&s_read_string, s_read_buffer, READ_BUFFER_SIZE);

  // Initialize the scheduler along with storage for tasks queued from interrupt
  // level.
  mu_sched_init(&s_sched, s_isr_queue_pool, ISR_QUEUE_POOL_SIZE);

  // Initialize and install the idle task in the scheduler.
  mu_task_init_immed(&s_idle_task, idle_task_fn, NULL, "Idle");
  mu_sched_set_idle_task(&s_sched, &s_idle_task);

  // Initialize the LED task.  The task will be initially scheduled in a call to
  // set_led_task_enable(true).  Thereafter, led_task_fn() will reschedule it.
  mu_task_init_immed(&s_led_task, led_task_fn, NULL, "LED");

  // Initialize the screen update task.  It will be triggered from within the
  // screen_update task.
  mu_task_init_immed(&s_screen_redraw_task, screen_redraw_fn, NULL, "Redraw");
  s_screen_state = 0;

  // Initialize the screen update task.  It will initially be scheduled by a
  // call to set_screen_update_enabled(true), thereafter it will reschedule
  // itself.
  mu_task_init_immed(&s_screen_update_task, screen_update_fn, NULL, "Update");

  // Initialize the keyboard monitor task.  This task is called whenever a
  // serial character becomes available via the async read callback.
  mu_task_init_immed(&s_keyboard_monitor_task,
                     keyboard_monitor_fn,
                     NULL,
                     "Keyboard");

  // Initialize the I/O stream for the serial port.  TBD: port-specific hw arg.
  mu_async_init(&s_async, &s_sched, NULL, s_write_buffer, WRITE_BUFFER_SIZE);

  // Upon completion of a write to the serial port, schedule a call to the
  // screen_redraw_task to produce the next update.
  mu_async_set_write_cb(&s_async, &s_screen_redraw_task);

  // Upon receiving a character on the serial port, schedule a call to the
  // keyboard_monitor_task to read and process the character.
  mu_async_set_read_cb(&s_async, &s_keyboard_monitor_task);

  // set up application defaults
  set_low_power_mode(false);

  ext_irq_register(PIN_PA15, button_on_PA15_pressed);

  // queue initial tasks
  led_task_start();
  screen_update_task_start();

  // Start the scheduler
  while (1) {
    mu_sched_step(&s_sched);
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
  (void)self;
  // Peek at the first task in the scheduler to find the desired wake time.
  // Note that we could have used &s_sched to get a pointer to the scheduler,
  // but it's also passed as the *arg parameter.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_task_t *next_task = mu_sched_get_tasks(sched);

  if (!is_low_power_mode()) {
    // Don't sleep if we are not in low-power mode
    return;
  } else if (mu_async_is_busy(&s_async)) {
  // Don't sleep until the serial port finishes writing data
    return;
  }

  // TODO: bcast "will sleep" to inform other that we're just about to sleep
  if (next_task == NULL) {
    mu_sleep_indefinitely();
  } else {
    mu_sleep_until(mu_task_time(next_task));
  }
  // TODO: bcast "did wake" to inform others that we just woke up
  asm("nop");
}

// ==========
// LED task
// ==========

static void led_task_start() {
  start_periodic_task(&s_led_task);
}

static void led_task_stop() {
  gpio_set_pin_level(LED0, true);  // turn off LED (low-true logic)
  stop_periodic_task(&s_led_task);
}

// This is the function that gets called whenever the s_led_task is triggered.
static void led_task_fn(void *self, void *arg) {
  // Inhibit compiler warning about unused variables
  (void)self;
  (void)arg;

  // Toggle the LED pin
  gpio_toggle_pin_level(LED0);

  // Reschedule the LED task to trigger LED_UPDATE_INTERVAL seconds in the
  // future. Note that in order to prevent timing drift, the task time is
  // computed as (prev_task_time + interval) rather than (now + interval).
  mu_task_advance_time(&s_led_task,
                       mu_time_seconds_to_duration(LED_UPDATE_INTERVAL));

  mu_sched_add(&s_sched, &s_led_task);
}

// ==========
// Screen Update task
// ==========

static void screen_update_task_start() {
  start_periodic_task(&s_screen_update_task);
}

static void screen_update_task_stop() {
  stop_periodic_task(&s_screen_update_task);
}

static void screen_update_fn(void *self, void *arg) {
  screen_redraw_task_start();

  // Reschedule the screen update task to trigger SCREEN_UPDATE_INTERVAL
  // seconds in the future.
  mu_task_advance_time(&s_screen_update_task,
                       mu_time_seconds_to_duration(SCREEN_UPDATE_INTERVAL));
  mu_sched_add(&s_sched, &s_screen_update_task);
 }

// ==========
// Screen Redraw Task
// ==========

static void screen_redraw_task_start() {
  s_screen_state = 0;
  mu_task_set_immediate(&s_screen_redraw_task);
  mu_sched_add(&s_sched, &s_screen_redraw_task);
}

/**
 * The screen redraw task.  Uses s_screen_state to control behavior.
 */
static void screen_redraw_fn(void *self, void *arg) {
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
  // Inhibit compiler warning about unused variables
  (void)self;
  (void)arg;

  if (s_screen_state == -1) {
    // waiting for someone to start this task.
    return;
  }

  // From here: get a buffer to write into, write into it and post it.
  // The async driver will print the contents and re-trigger this task
  // when the next buffer comes available.

  if (MU_ASYNC_ERR_NONE != mu_async_reserve_write_buffer(&s_async, &buf)) {
    // no write buffer is available yet.  A callback will restart the task.
    return;
  };

  if (s_screen_state < 4) {
    // display the first three lines
    mu_string_sprintf(buf, "%s", strings[s_screen_state]);
    s_screen_state += 1;

  } else if (s_screen_state < 4 + N_TASKS) {
    // display the task info for eacn of N tasks
    sprintf_task_status(buf, s_tasks[s_screen_state - 4]);
    s_screen_state += 1;

  } else if (is_low_power_mode()) {
      mu_string_sprintf(buf, "%s", strings[s_screen_state - N_TASKS]);
      s_screen_state = -1;  // no more text to display -- return to idle state

  } else {
      mu_string_sprintf(buf, "%s", strings[s_screen_state - N_TASKS + 1]);
      s_screen_state += 1;
      if (s_screen_state > N_TASKS + 9) {
        s_screen_state = -1;  // no more text to display -- return to idle state
      }
  }
  // Pass the buffer to mu_async for printing to the screen.
  mu_async_post_write_buffer(&s_async);
}

static void sprintf_task_status(mu_string_t *s, mu_task_t *task) {
  static char buf[128];

  mu_string_sprintf(s,
                    "\r\n%s: %c",
                    rjust(mu_task_name(task), 10, ' '),
                    get_task_state(task));
  if (mu_task_is_immediate(task)) {
    mu_string_sprintf(s, "%s", rjust("immediate", 10, ' '));
  } else {
    sprintf(buf, "%u", (int)mu_task_time(task));
    mu_string_sprintf(s, "%s", rjust(buf, 10, ' '));
  }
  sprintf(buf, " %u", mu_task_call_count(task));
  mu_string_sprintf(s, "%s", rjust(buf, 10, ' '));
  sprintf(buf, " %s", cheap_ftoa(mu_task_max_latency(task)));
  mu_string_sprintf(s, "%s", rjust(buf, 12, ' '));
  sprintf(buf, " %s", cheap_ftoa(mu_task_runtime(task)));
  mu_string_sprintf(s, "%s", rjust(buf, 12, ' '));
}

/*
 * A = active (current)
 * I = inactive (not in sched)
 * R = runnable (ready to run)
 * P = pending (not yet runnable)
 */
static char get_task_state(mu_task_t *task) {
  if (mu_sched_current_task(&s_sched) == task) {
    return 'A'; // this task is currently running
  } else if (!mu_sched_has_task(&s_sched, task)) {
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
 * Arrive here on a keyboard interrupt
 */
void keyboard_monitor_fn(void *self, void *arg) {
  char buf[READ_BUFFER_SIZE];
  mu_string_t s;
  mu_string_init(&s, buf, READ_BUFFER_SIZE);

  mu_async_err_t err = mu_async_read(&s_async, &s);
  if (MU_ASYNC_ERR_NONE != err) {
    // nothing to see here - move along...
    return;
  }

  int n_read = mu_string_length(&s);
  if (n_read == 0) {
	  return;
  }

  if (n_read > 0) {
	mu_string_buf(&s)[mu_string_end(&s)] = '\0';  // Null terminate
  }

  // In this example, we only pay attention to the first character
  switch(mu_string_buf(&s)[0]) {
    case 'b':  // activate blink
    led_task_start();
    echo_user_string("b: activate blink task");
    break;
    case 'B':  // deactivate blink
    led_task_stop(false);
    echo_user_string("B: suspend blink task");
    break;
    case 'd':  // activate display
    screen_update_task_start();
    echo_user_string("d: activate display task");
    break;
    case 'D':  // deactivate display
    screen_update_task_stop();
    echo_user_string("D: suspend display task");
    break;
    case 'p':  // low power mode
    set_low_power_mode(true);
    echo_user_string("p: enter low-power idle mode");
    break;
    default:   // echo typed char
    echo_user_string(mu_string_buf(&s));
  }
}

/**
 * If async is available, write now.
 */
static void echo_user_string(char *s) {
  // TBD...
}

static void set_low_power_mode(bool enable) {
  s_low_power_mode = enable;
}

static bool is_low_power_mode() {
  return s_low_power_mode;
}

static void start_periodic_task(mu_task_t *task) {
  if (mu_sched_has_task(&s_sched, task)) {
    // already in schedule -- no action needed
  } else {
    // set initial time and add to schedule
    mu_sched_add(&s_sched, mu_task_set_time(task, mu_time_now()));
  }
}

static void stop_periodic_task(mu_task_t *task) {
  // remove from schedule (if present)
  mu_sched_remove(&s_sched, task);
}

static void button_on_PA15_pressed(void) {
  set_low_power_mode(false);
  mu_sched_add_from_isr(&s_sched, &s_keyboard_monitor_task);
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
