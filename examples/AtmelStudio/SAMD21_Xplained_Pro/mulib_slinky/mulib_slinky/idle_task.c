/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

#include "idle_task.h"
#include "mulib.h"
#include "mu_vm.h"
#include <atmel_start.h>
#include <stddef.h>
#include "mulib_slinky.h"

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *idle_task_fn(void *self, void *arg);
static bool is_ready_to_sleep(void);
static void will_sleep(void);
static void did_wake(void);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_task_t *idle_task_init(mu_task_t *idle_task, mu_sched_t *sched) {
  // Initialize the idle task and install as scheduler's idle task
  mu_task_init(idle_task, idle_task_fn, NULL, "Sleeping Idle");
  mu_sched_set_idle_task(sched, idle_task);

  return idle_task;
}

// =============================================================================
// local (static) code

static void *idle_task_fn(void *ctx, void *arg) {
  // ctx is unused in idle task
  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_sched_event_t *next_event = mu_sched_get_next_event(sched);

  if (is_ready_to_sleep()) {
    will_sleep();
    if (next_event) {
      // There is a future event: sleep until it arrives or skip sleeping if
      // the event is imminent.
      mu_vm_sleep_until(next_event->time);
    } else {
      // no future events are scheduled -- only an interrupt will wake us
      mu_vm_sleep();
    }
    did_wake();
  }
  return NULL;
}

static bool is_ready_to_sleep(void) {
  // If you have peripherals that are active or other factors that should
  // prevent the processor from going to sleep, you would check for it here
  // and return false to inhibit going to sleep.
  return true;
}

static void will_sleep(void) {
  // If you have any last-moment cleanup that needs to be done before the
  // processor goes to sleep, you would do it here.
  usart_async_disable(&USART_0);
  asm("nop");
}

static void did_wake(void) {
  // If you have anything that needs to be done when the processor wakes from
  // sleep, you would do it here.
  usart_async_enable(&USART_0);
  asm("nop");
}
