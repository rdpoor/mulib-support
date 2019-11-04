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

#include "mu_queue.h"
#include "mu_task.h"
#include "mu_time.h"
#include <stdbool.h>

// =============================================================================
// types and definitions

typedef enum {
  MU_SCHED_ERR_NONE,
  MU_SCHED_ERR_FULL,
  MU_SCHED_ERR_NOT_FOUND,
  MU_SCHED_ERR_ALREADY_SCHEDULED,
} mu_sched_err_t;

// Signature for clock source function.  Returns the current time.
typedef mu_time_t (*mu_clock_fn)(void);

// To handle tasks from interrupt level, we use a thread-safe mu_queue circular
// buffer to queue tasks produced at interrupt level and consumed at foreground
// level.
//
// mu_sched_queue_from_isr() pushes an task on the queue, then mu_sched_step()
// moves any tasks from the queue and adds them to the regular scheduler list
// before processing.

typedef struct {
  mu_task_t *tasks;    // a linked list of tasks
  mu_queue_t isr_queue; // interrupt-safe queue of tasks to be added
  mu_clock_fn clock_source;
  mu_task_t *idle_task;
  mu_task_t *current_task;
} mu_sched_t;

// =============================================================================
// declarations

// isr_queue_size must be a power of two
mu_sched_t *mu_sched_init(mu_sched_t *sched,
                          mu_queue_obj_t *isr_queue_pool,
                          unsigned int isr_queue_pool_size);

mu_sched_t *mu_sched_reset(mu_sched_t *sched);

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn);

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *idle_task);

bool mu_sched_is_empty(mu_sched_t *sched);

bool mu_sched_has_task(mu_sched_t *sched, mu_task_t *task);

unsigned int mu_sched_task_count(mu_sched_t *sched);

mu_task_t *mu_sched_get_tasks(mu_sched_t *sched);

mu_sched_err_t mu_sched_step(mu_sched_t *sched);

mu_time_t mu_sched_get_time(mu_sched_t *sched);

mu_task_t *mu_sched_current_task(mu_sched_t *sched);

mu_sched_err_t mu_sched_add(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_remove(mu_sched_t *sched, mu_task_t *task);

mu_sched_err_t mu_sched_add_from_isr(mu_sched_t *sched, mu_task_t *task);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_SCHED_H_
