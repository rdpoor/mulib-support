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

/**
 * About the idle task:
 *
 * The scheduler calls the idle task when no other tasks are available to run.
 * Some sensible options for the idle task include:
 *
 * Do nothing.  Left unset, the idle task will return immediately to the
 * scheduler.
 *
 * Do something until there is a task to run, along these lines:
 *   scheduler_will_idle();
 *   while (!mulib_sched_has_task()) {
 *     // do idle things
 *   }
 *   scheduler_will_run();
 *
 * Sleep until something happens, along these lines:
 *   scheduler_will_idle();
 *   if ((next_event = mulib_sched_next_event()) != NULL) {
 *     port_sleep_until(mulib_event_time(next_event));
 *   } else {
 *     port_sleep();
 *   }
 *   scheduler_will_run();
 * NOTE: This has a race condition because an event may get scheduled after the
 * call to mulib_sched_next_event() and before the port_sleep...().  Rethink...
 */
#ifndef MULIB_SCHED_H
#define MULIB_SCHED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "mulib_event.h"
#include "mulib_task.h"
#include "../port/port_time.h"

typedef enum {
  MULIB_SCHED_ERR_NONE,
  MULIB_SCHED_ERR_FULL,
} mulib_sched_err_t;

typedef struct _mulib_sched {
  mulib_event_t *events;        // array of events, latest at events[0]
  size_t capacity;              // max number of events
  size_t count;                 // next available event slot
  mulib_event_t *current_event; // event currently being processed
  mulib_task_t idle_task;       // task to run if nothing else is runnable
} mulib_sched_t;

/**
 * @brief initialize the scheduler.
 *
 * Must be called prior to other mulib_sched functions.
 */
void mulib_sched_init(mulib_sched_t *sched, mulib_event_t *events, size_t capacity);

void mulib_sched_reset(mulib_sched_t *sched);

void mulib_sched_set_idle_task(mulib_sched_t *sched, mulib_task_fn fn, void *u_arg);

void mulib_sched_process(mulib_sched_t *sched);

mulib_sched_err_t mulib_sched_immediate(mulib_sched_t *sched, mulib_task_fn fn, void *u_arg);

mulib_sched_err_t mulib_sched_at(mulib_sched_t *sched, port_time_t at, mulib_task_fn fn, void *u_arg);

mulib_sched_err_t mulib_sched_in(mulib_sched_t *sched, port_time_dt in, mulib_task_fn fn, void *u_arg);

mulib_event_t *mulib_sched_current_event(mulib_sched_t *sched);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_SCHED_H
