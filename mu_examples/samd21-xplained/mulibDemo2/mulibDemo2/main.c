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
#include "mu_task.h"
#include "mu_time.h"
#include "mu_port.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// types and definitions

// Define repetition interval for toggling LED and updating display
#define LED_UPDATE_INTERVAL 0.75

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



// =============================================================================
// local storage

/**
 * \brief s_sched is an instance of the mulib scheduler.  You schedule an task
 * to trigger at some point in the future through a call to mu_sched_queue().
 */
static mu_sched_t s_sched;

/*
 * \brief s_idle_task is the task that the scheduler runs when nothing else is
 * runnable.  In this case, we put the processor into stanby mode until an
 * external interrupt or RTC match interrupt wakes the processor.
 */
static mu_task_t s_idle_task;

/**
 * \brief s_led_task is the sole task scheduled in this simple demo.  When
 * triggered by the scheduler, it will call led_task_fn().
 */
static mu_task_t s_led_task;

// =============================================================================
// public code

int main(void) {
  // Perform board-specific initialization
  atmel_start_init();

  // Perform port-specific initialization needed by mulib (RTC)
  mu_port_init();

  // Initialize the scheduler.  In this demo, we don't need to provide a pool
  // for ISR tasks so we pass NULL and 0.
  mu_sched_init(&s_sched, NULL, 0);

  // Initialize and install the idle task in the scheduler.
  mu_task_init_immed(&s_idle_task, idle_task_fn, NULL, "Idle");
  mu_sched_set_idle_task(&s_sched, &s_idle_task);

  // Get the current time.  This demo uses the RTC as a common time source for
  // scheduled tasks.
  mu_time_t now = mu_time_now();

  // Start the LED task.  Setting its task time to "now" means that it will
  // trigger as soon as the scheduler starts running.  The led_task_fn will
  // re-schedule itself to create a periodic timer.
  mu_task_init_at(&s_led_task, now, led_task_fn, NULL, "LED");
  mu_sched_add(&s_sched, &s_led_task);

  // Start the scheduler
  while (1) {
    mu_sched_step(&s_sched);
  }
  // Like Charlie on the MTA, it will never return...
  return 0;
}

// =============================================================================
// private code

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

  // TODO: bcast will sleep to inform other that we're just about to sleep
  if (next_task == NULL) {
    mu_sleep_indefinitely();
  } else {
    mu_sleep_until(mu_task_time(next_task));
  }
  // TODO: bcast did waketo inform others that we just woke up
  asm("nop");
}
