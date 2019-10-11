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

#ifndef MU_SCHED_H_
#define MU_SCHED_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "../port/port.h"
#include "mu_evt.h"
#include "mu_task.h"
#include <stdbool.h>

// =============================================================================
// types and definitions

typedef enum {
  MU_SCHED_ERR_NONE,
  MU_SCHED_ERR_FULL,
  MU_SCHED_ERR_NOT_FOUND,
} mu_sched_err_t;

// Signature for clock source function.  Returns a port_time_t value.
typedef port_time_t (*mu_clock_fn)(void);

// TODO:
// We want a way to queue events from interrupt level.  We can use a thread-
// safe circular buffer to queue events produced at interrupt level and consumed
// at foreground level.  For this, we've created mu_ring (q.v).
//
// So mu_sched_from_isr() will push an event on the queue.  Then mu_sched_step()
// will get all events from that queue and add them the regular event list
// before processing.

typedef struct {
  mu_evt_t *events;          // a linked list of events
  // mu_ring_t *isr_events; // see TODO above
  mu_clock_fn clock_source;
  mu_task_t *idle_task;
  mu_evt_t *current_event;
} mu_sched_t;

// =============================================================================
// declarations

mu_sched_t *mu_sched_init(mu_sched_t *sched);

mu_sched_t *mu_sched_reset(mu_sched_t *sched);

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn);

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *idle_task);

bool mu_sched_is_empty(mu_sched_t *sched);

mu_sched_err_t mu_sched_step(mu_sched_t *sched);

mu_evt_t *mu_sched_current_event(mu_sched_t *sched);

mu_sched_err_t mu_sched_add(mu_sched_t *sched, mu_evt_t *event);

mu_sched_err_t mu_sched_remove(mu_sched_t *sched, mu_evt_t *evt);

// see TODO
// mu_sched_err_t mu_sched_from_isr(mu_sched_t *sched, mu_evt_t *event);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_SCHED_H_
