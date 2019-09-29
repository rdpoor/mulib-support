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

#import "mulib_sched.h"
#import "../port/port_critical.h"
#import "../port/port_time.h"
#import <stddef.h>
#import <string.h> // memset

#define SCHED_WILL_RUN 1
#define SCHED_WILL_IDLE 2

static bool sched_has_runnable_event(mulib_sched_t *sched);
static bool sched_step(mulib_sched_t *sched);

/**
 * @brief initialize the scheduler.
 *
 * Must be called prior to other mulib_sched functions.
 */
void mulib_sched_init(mulib_sched_t *sched, mu_evt_t *events, size_t capacity) {
  sched->events = events;
  sched->capacity = capacity;
  mulib_sched_set_idle_task(sched, NULL, NULL);
  mulib_sched_reset(sched);
}

void mulib_sched_reset(mulib_sched_t *sched) {
  sched->count = 0;
  sched->current_event = NULL;
  memset(sched->events, 0, sched->capacity * sizeof(mu_evt_t));
}

void mulib_sched_set_idle_task(mulib_sched_t *sched, mu_msg_fn fn, void *self) {
  mu_msg_init(&sched->idle_task, fn, self);
}

void mulib_sched_process(mulib_sched_t *sched) {
  while (sched_step(sched)) {
    // just processed one event
  }
  while (!sched_has_runnable_event(sched)) {
    mu_msg_call(&sched->idle_task, sched);
  }
}

mulib_sched_err_t mulib_sched_immediate(mulib_sched_t *sched,
                                        mu_msg_fn fn,
                                        void *self) {
  if (sched->count == sched->capacity) {
    return MULIB_SCHED_ERR_FULL;
  }

  port_critical_begin();
  mu_evt_init_immediate(&(sched->events[sched->count++]), fn, self);
  port_critical_end();

  return MULIB_SCHED_ERR_NONE;
}

mulib_sched_err_t mulib_sched_at(mulib_sched_t *sched, port_time_t at,
                                 mu_msg_fn fn, void *self) {
  int i;
  mu_evt_t *event;

  if (sched->count == sched->capacity) {
    return MULIB_SCHED_ERR_FULL;
  }

  // Use insertion sort to place event in the schedule
  port_critical_begin();
  // We assume the list is relatively short, so we use linear search to find the
  // insertion point.  In the future, we could dynamically choose between linear
  // and binary search depending on sched->count.
  for (i=0; i<sched->count; i++) {
    event = &sched->events[i];
    if (mu_evt_is_immediate(event) ||
        port_time_is_after(at, mu_evt_time(event))) {
      break;
    }
  }

  // Now i is the index at which to insert.  Carve a hole by moving events up.
  int n = sched->count - i;            // number of events to move
  event = &(sched->events[i]);
  memmove(&(event[1]), event, n * sizeof(mu_evt_t));
  sched->count += 1;
  port_critical_end();

  // Initialize the event.
  mu_evt_init_timed(event, at, fn, self);

  return MULIB_SCHED_ERR_NONE;
}

mulib_sched_err_t mulib_sched_in(mulib_sched_t *sched,
                                 port_time_dt in,
                                 mu_msg_fn fn,
                                 void *self) {
  return mulib_sched_at(sched, port_time_offset(port_time_now(), in), fn, self);
}

mu_evt_t *mulib_sched_current_event(mulib_sched_t *sched) {
  return sched->current_event;
}

// =============================================================================
// local functions

// Return true if an event in the schedule is runnable
static bool sched_has_runnable_event(mulib_sched_t *sched) {
  bool is_runnable;

  port_critical_begin();
  if (sched->count == 0) {
    is_runnable = false;
  } else {
    mu_evt_t *event = &(sched->events[sched->count - 1]);
    is_runnable = mu_evt_has_arrived(event);  // peek at last event
  }
  port_critical_end();

  return is_runnable;
}

// If there is a runnable event, remove it from the schedule, run it, and
// return true.  Else return false;
static bool sched_step(mulib_sched_t *sched) {
  bool is_runnable;
  mu_evt_t *event;

  port_critical_begin();
  if (sched->count == 0) {
    is_runnable = false;
  } else {
    event = &(sched->events[sched->count - 1]);
    sched->count -= 1;
    is_runnable = true;
  }
  port_critical_end();

  if (is_runnable) {
    sched->current_event = event;
    mu_evt_call(event, sched);
    sched->current_event = NULL;
  }

  return is_runnable;
}
