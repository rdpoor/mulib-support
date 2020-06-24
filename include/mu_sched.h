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

#ifndef _MU_SCHED_H_
#define _MU_SCHED_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>
#include <stdbool.h>
#include "mu_event.h"
#include "mu_task.h"
#include "mu_time.h"
#include "mu_spscq.h"
#include "mu_task.h"

// =============================================================================
// types and definitions

typedef enum {
  MU_SCHED_ERR_NONE,
  MU_SCHED_ERR_EMPTY,
  MU_SCHED_ERR_FULL,
  MU_SCHED_ERR_NOT_FOUND,
} mu_sched_err_t;

// Signature for clock source function.  Returns the current time.
typedef mu_time_t (*mu_clock_fn)(void);

typedef struct {
  mu_event_t *event_queue;     // scheduled items
  size_t event_queue_capacity; // max number of items in the queue
  size_t event_queue_count;    // number of items in the queue
  mu_spscq_t *isr_queue;       // interrupt-safe queue of tasks to be added
  mu_clock_fn clock_fn;        // function to call to get the current time
  mu_task_t *idle_task;        // the idle task
  mu_event_t *current_event;   // the task currently being processed (or NULL)
} mu_sched_t;

// =============================================================================
// declarations

/**
 * \brief initialize the schedule module.
 */
mu_sched_t *mu_sched_init(mu_sched_t *sched, mu_event_t *event_queue, size_t event_queue_capacity, mu_spscq_t *isr_queue);

/**
 * \brief  Reset the schedule.
 */
mu_sched_t *mu_sched_reset(mu_sched_t *sched);

mu_sched_err_t mu_sched_step(mu_sched_t *sched);

mu_event_t *mu_sched_event_queue(mu_sched_t *sched);

mu_spscq_t *mu_sched_isr_queue(mu_sched_t *sched);

mu_task_t *mu_sched_get_default_idle_task(mu_sched_t *sched);

mu_task_t *mu_sched_get_idle_task(mu_sched_t *sched);

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *task);

mu_clock_fn mu_sched_get_clock_source(mu_sched_t *sched);

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn);

mu_time_t mu_sched_get_current_time(mu_sched_t *sched);

bool mu_sched_is_empty(mu_sched_t *sched);

size_t mu_sched_event_count(mu_sched_t *sched);

mu_event_t *mu_sched_get_current_event(mu_sched_t *sched);

mu_task_t *mu_sched_get_current_task(mu_sched_t *sched);

mu_event_t *mu_sched_get_next_event(mu_sched_t *sched);

mu_task_t *mu_sched_get_next_task(mu_sched_t *sched);

mu_sched_err_t mu_sched_get_next_time(mu_sched_t *sched, mu_time_t *time);

mu_sched_err_t mu_sched_remove_task(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_task_now(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_task_at(mu_sched_t *sched, mu_task_t *task, mu_time_t at);

mu_sched_err_t mu_sched_task_in(mu_sched_t *sched, mu_task_t *task, mu_time_t in);

mu_sched_err_t mu_sched_task_from_isr(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_task_at_safe(mu_sched_t *sched, mu_task_t *task, mu_time_t at);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_SCHED_H_ */
