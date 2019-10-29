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
#include "mu_iostream.h"
#include "mu_sched.h"
#include "mu_sleep.h"
#include "mu_strbuf.h"
#include "mu_task.h"
#include "mu_time.h"
// #include "mu_port.h"
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
#define WRITE_STRBUF_POOL_SIZE (60*15)

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
static void led_task_fn(void *self, void *arg);

/**
 * \brief screen_update_fn is called when the s_screen_update task triggers.
 * It will update the display to show the state of the mulib tasks and then
 * reschedule itself to be called again.
 */
static void screen_update_fn(void *self, void *arg);

static void print_task_status(mu_strbuf_t *sb, mu_task_t *task);
static char get_task_state(mu_task_t *task);
static const char *rjust(const char *s, int width, char padchar);
static const char *cheap_ftoa(float x);

/**
 * \brief keyboard_monitor_fn is called whenever the s_keyboard_task is called.
 * It will read keyboard input and update the state of the demo program.
 */
static void keyboard_monitor_fn(void *self, void *arg);

/**
 * \brief Set/get low-power mode.
 *
 * When enabled, the idle task will put the processor into STANDBY mode for the
 * duration of the idle task.  In low-power mode, a button press is required to
 * start listening on the keybaord again.
 */
static void set_low_power_mode(bool enable);
static bool is_low_power_mode();

/**
 * \brief Set/get the enable flag for the LED task.
 *
 * When set, the LED task will run.  When cleared, the LED task is suspended.
 */
static void set_led_task_enable(bool enable);
static bool led_task_is_enabled();

/**
 * \brief Set/get the enable flag for the screen update task.
 *
 * When set, the screen update task will run.  When cleared, the screen update
 * task is suspended.
 */
static void set_screen_update_enable(bool enable);
static bool screen_update_is_enabled();

/**
 * \brief Enable or disable a task, adding it to or removing it from the
 * scheduler.
 */
static void enable_task(mu_task_t *task, bool enable);

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
static mu_ring_obj_t s_isr_queue_pool[ISR_QUEUE_POOL_SIZE];

/**
 * An iostream for serial I/O.  This works in conjunction with the scheduler
 * to handle events generated at interrupt level and trigger tasks in the
 * foreground.
 */
static mu_iostream_t s_iostream;

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
 * \brief s_screen_update_task is responsible for periodically updating the
 * screen.
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
static bool s_led_task_enabled;
static bool s_screen_update_enabled;

static char s_write_strbuf_pool[WRITE_STRBUF_POOL_SIZE];
static mu_strbuf_t s_write_strbuf;

static mu_task_t *s_tasks[] = {
  &s_idle_task,
  &s_led_task,
  &s_screen_update_task,
  &s_keyboard_monitor_task
};

const size_t N_TASKS = sizeof(s_tasks) / sizeof(mu_task_t *);

// =============================================================================
// public code

int main(void) {
  // Perform board-specific initialization
  atmel_start_init();

  // Perform port-specific initialization needed by mulib
  mu_port_init();

  // set up the string buffer for screen write operations
  mu_strbuf_init(&s_write_strbuf, s_write_strbuf_pool, WRITE_STRBUF_POOL_SIZE);

  // Initialize the scheduler along with storage for tasks queued from interrupt
  // level.
  mu_sched_init(&s_sched, s_isr_queue_pool, ISR_QUEUE_POOL_SIZE);

  // Initialize and install the idle task in the scheduler.
  mu_task_init_immed(&s_idle_task, idle_task_fn, NULL, "Idle");
  mu_sched_set_idle_task(&s_sched, &s_idle_task);

  // Initialize the LED task.  The task will be initially scheduled in a call to
  // set_led_task_enable(true).  Thereafter, led_task_fn() will reschedule it.
  mu_task_init_immed(&s_led_task, led_task_fn, NULL, "LED");

  // Initialize the screen update task.  The task will be initially be scheduled
  // by a call to set_screen_update_enable(true).  Thereafter, it will be
  // rescheduled whenever the write to the serial port completes via the
  // iostream write callback.
  mu_task_init_immed(&s_screen_update_task, screen_update_fn, NULL, "Screen");

  // Initialize the keyboard monitor task.  This task is called whenever a
  // serial character becomes available via the iostream read callback.
  mu_task_init_immed(&s_keyboard_monitor_task,
                     keyboard_monitor_fn,
                     NULL,
                     "Keyboard");

  // Initialize the I/O stream for the serial port.  TBD: port-specific hw arg.
  mu_iostream_init(&s_iostream, &s_sched, NULL);
  // Upon completion of a write to the serial port, schedule a call to the
  // screen_update_task to produce the next update.
  mu_iostream_set_write_callback(&s_iostream, &s_screen_update_task);
  // Upon receiving a character on the serial port, schedule a call to the
  // keyboard_monitor_task to read and process the character.
  mu_iostream_set_read_callback(&s_iostream, &s_keyboard_monitor_task);

  // set up application defaults
  set_low_power_mode(true);
  set_led_task_enable(true);        // start the led task
  set_screen_update_enable(true);   // start the screen update task

  // Start the scheduler
  while (1) {
    mu_sched_step(&s_sched);
  }
  // Like Charlie on the MTA, it will never return...
  return 0;
}

// =============================================================================
// private code

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
  } else if (mu_iostream_write_is_busy(&s_iostream)) {
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

// This is the function that gets called whenever the s_led_task is triggered.
static void led_task_fn(void *self, void *arg) {
  // Inhibit compiler warning about unused variables
  (void)self;
  (void)arg;

  if (led_task_is_enabled()) {
    // Toggle the LED pin
    gpio_toggle_pin_level(LED0);
    // Reschedule the LED task to trigger LED_UPDATE_INTERVAL seconds in the
    // future. Note that in order to prevent timing drift, the task time is
    // computed as (prev_task_time + interval) rather than (now + interval).
    mu_task_advance_time(&s_led_task,
                         mu_time_seconds_to_duration(LED_UPDATE_INTERVAL));
    mu_sched_add(&s_sched, &s_led_task);

  } else {
    // LED task is disabled.  Turn off the LED and don't reschedule.
    gpio_set_pin_level(LED0, false);
  }

}

// Implementation note: This function builds up the entire string to be written
// to the screen and then writes it in a single operation.  This requires a
// rather large buffer to contain the string.  A future version could use a
// simple state machine to output a line a time, so the buffer would only need
// to be one line long.
static void screen_update_fn(void *self, void *arg) {
  // Inhibit compiler warning about unused variables
  (void)self;
  (void)arg;

  if (!screen_update_is_enabled()) {
    // return immediately if not updating the screen
    return;

  } else if (mu_iostream_write_is_busy(&s_iostream)) {
    // return immediately if previous write operation is in progress.  This
    // task will be re-queued from interrupt level when the previous write
    // operation completes.
    return;
  }

  // Format the report in the write string buffer and then pass it to
  // mu_iostream_write() for printing to the screen.
  mu_strbuf_reset(&s_write_strbuf);
  mu_strbuf_append(&s_write_strbuf, CLEAR_SCREEN);
  mu_strbuf_append(
    &s_write_strbuf,
    "\r\n>     Task: State    At     Calls  MaxLatency     Runtime");
  mu_strbuf_append(
    &s_write_strbuf,
    "\r\n*********************************************************");
  for (int i=0; i<N_TASKS; i++) {
    print_task_status(&s_write_strbuf, s_tasks[i]);
  }
  if (is_low_power_mode()) {
    mu_strbuf_append(
      &s_write_strbuf,
      "\r\n- Low-power idle mode: press user button to enter full-power mode.");
  } else {
    mu_strbuf_append(
      &s_write_strbuf,
      "\r\n- Full-power mode. Select one:"
      "\r\n b: activate blink task"
      "\r\n B: suspend blink task"
      "\r\n d: activate display task"
      "\r\n D: suspend display task"
      "\r\n p: enter low-power idle mode"
      "\r\n others: Echo back"
      "\r\n# ");
  }
  // The write completion callback will queue s_screen_update_task (which will call this
  // function).  Update the scheduled time.
  mu_task_advance_time(&s_screen_update_task, mu_time_seconds_to_duration(SCREEN_UPDATE_INTERVAL));
  // output the whole string
  mu_iostream_write(&s_iostream, &s_write_strbuf);
}

static void print_task_status(mu_strbuf_t *sb, mu_task_t *task) {
  static char buf[128];

  mu_strbuf_printf(sb,
                   "\r\n%s: %c",
                   rjust(mu_task_name(task), 10, ' '),
                   get_task_state(task));
  if (mu_task_is_immediate(task)) {
    mu_strbuf_append(sb, rjust("immediate", 10, ' '));
  } else {
    sprintf(buf, "%u", (int)mu_task_time(task));
    mu_strbuf_append(sb, rjust(buf, 10, ' '));
  }
  sprintf(buf, " %u", mu_task_call_count(task));
  mu_strbuf_append(sb, rjust(buf, 10, ' '));
  sprintf(buf, " %s", cheap_ftoa(mu_task_max_latency(task)));
  mu_strbuf_append(sb, rjust(buf, 12, ' '));
  sprintf(buf, " %s", cheap_ftoa(mu_task_runtime(task)));
  mu_strbuf_append(sb, rjust(buf, 12, ' '));
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

// print six digits to the right of the decimal point
#define FTOA_PRECISION 1000000
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
  static char fbuf[7];                           // six digits plus null
  int c = (x * FTOA_PRECISION) + 0.5;            // handle rounding
  int d = c / FTOA_PRECISION;                    // the integer part
  int f = c - (d * FTOA_PRECISION);              // the fractional part
  sprintf(fbuf, "%u", f);                        // fractional part as ascii
  sprintf(buf, "%u.%s", d, rjust(fbuf, 6, '0')); // pad with leading '0'
  return buf;
}

void keyboard_monitor_fn(void *self, void *arg) {
  // Arrive here following a receive interrupt.
  asm("nop");
}

static void set_low_power_mode(bool enable) {
  s_low_power_mode = enable;
}

static bool is_low_power_mode() {
  return s_low_power_mode;
}

static void set_led_task_enable(bool enable) {
  s_led_task_enabled = enable;
  enable_task(&s_led_task, enable);
  if (!enable) {
    // make sure LED is off when deactivating the task
    gpio_set_pin_level(LED0, false);
  }
}

static bool led_task_is_enabled() {
  return s_led_task_enabled;
}

static void set_screen_update_enable(bool enable) {
  s_screen_update_enabled = enable;
  enable_task(&s_screen_update_task, enable);
}

static bool screen_update_is_enabled() {
  return s_screen_update_enabled;
}

static void enable_task(mu_task_t *task, bool enable) {
  // TODO: consider promoting this function to mu_sched().
  if (enable) {
    if (mu_sched_has_task(&s_sched, task)) {
      // already in schedule -- no action needed
    } else {
      // set initial time and add to schedule
      mu_sched_add(&s_sched, mu_task_set_time(task, mu_time_now()));
    }
  } else {
    // remove from schedule (if present)
    mu_sched_remove(&s_sched, task);
  }
}
