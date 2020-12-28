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
#include "mu_time.h"
#include "mu_pstore.h"
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
  mu_pstore_t *task_queue;   // a collection of tasks
  mu_spscq_t *isr_queue;     // interrupt-safe queue of tasks to be added
  mu_clock_fn clock_fn;      // function to call to get time
  mu_task_t *idle_task;      // the idle task
  mu_task_t *current_task;   // the task currently being processed (or NULL)
} mu_sched_t;

// =============================================================================
// declarations

/**
 * \brief initialize the schedule module.
 */
mu_sched_t *mu_sched_init(mu_sched_t *sched, mu_pstore_t *task_queue, mu_spscq_t *isr_queue);

/**
 * \brief  Reset the schedule.
 */
mu_sched_t *mu_sched_reset(mu_sched_t *sched);

mu_sched_err_t mu_sched_step(mu_sched_t *sched);

mu_pstore_t *mu_sched_task_queue(mu_sched_t *sched);

mu_spscq_t *mu_sched_isr_queue(mu_sched_t *sched);

mu_task_t *mu_sched_get_idle_task(mu_sched_t *sched);

mu_task_t *mu_sched_get_default_idle_task(mu_sched_t *sched);

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *task);

mu_clock_fn mu_sched_get_clock_source(mu_sched_t *sched);

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn);

bool mu_sched_is_empty(mu_sched_t *sched);

size_t mu_sched_task_count(mu_sched_t *sched);

mu_time_t mu_sched_get_current_time(mu_sched_t *sched);

mu_task_t *mu_sched_get_current_task(mu_sched_t *sched);

/**
 * @brief Get time of the next task, if present.
 *
 * Get (by reference) the time of the next event, returning MU_SCHED_ERR_NONE
 * if avaiable and MU_SCHED_ERR_NOT_FOUND if there is no next event.
 */
mu_sched_err_t mu_sched_get_next_time(mu_sched_t *sched, mu_time_t *time);

bool mu_sched_task_is_scheduled(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_task(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_task_fast(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_remove_task(mu_sched_t *sched, mu_task_t *task);

// convenience functions (tbd).
mu_sched_err_t mu_sched_task_now(mu_sched_t *sched, mu_task_t *task);
mu_sched_err_t mu_sched_task_at(mu_sched_t *sched, mu_task_t *task, mu_time_t at);
mu_sched_err_t mu_sched_task_in(mu_sched_t *sched, mu_task_t *task, mu_time_t in);

// like mu_sched_task_in, but time is relative to task's previous time
mu_sched_err_t mu_sched_task_again(mu_sched_t *sched, mu_task_t *task, mu_time_t dt);

mu_sched_err_t mu_sched_task_from_isr(mu_sched_t *sched, mu_task_t *task);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_SCHED_H_ */
