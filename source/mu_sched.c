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

#include "mu_sched.h"
#include <stdbool.h>
#include "mu_time.h"
#include "mu_task.h"
#include "mu_pstore.h"
#include "mu_spscq.h"

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *idle_task_fn(void *self, void *arg);

static int sched_ordering_fn(void *a, void *b);

// =============================================================================
// local storage

static mu_task_t s_default_idle_task;

// =============================================================================
// public code

mu_sched_t *mu_sched_init(mu_sched_t *sched, mu_pstore_t *task_queue, mu_spscq_t *isr_queue) {
  sched->task_queue = task_queue;
  sched->isr_queue = isr_queue;
  sched->clock_fn = mu_time_now;
  sched->idle_task = &s_default_idle_task;

  mu_task_init(&s_default_idle_task, idle_task_fn, NULL, "Idle");
  return mu_sched_reset(sched);
}

mu_sched_t *mu_sched_reset(mu_sched_t *sched) {
  mu_pstore_reset(sched->task_queue);
  mu_spscq_reset(sched->isr_queue);
  sched->current_task = NULL;

  return sched;
}

mu_sched_err_t mu_sched_step(mu_sched_t *sched) {
  // first, transfer any tasks in the isr queue into the main queue
  mu_task_t *task;
  while (mu_spscq_get(sched->isr_queue, (void **)&task) != MU_CQUEUE_ERR_EMPTY) {
    mu_sched_err_t err = mu_sched_task_now(sched, task);
    if (err != MU_SCHED_ERR_NONE) {
      return err;
    }
  }
  // process one event in the main queue
  if ((mu_pstore_peek(sched->task_queue, (void **)&task) == MU_PSTORE_ERR_NONE) &&
      (mu_task_is_runnable(task, mu_sched_get_current_time(sched)))) {
      mu_pstore_pop(sched->task_queue, (void *)&sched->current_task);
      mu_task_call(sched->current_task, sched);
  } else {
    sched->current_task = sched->idle_task;
    mu_task_call(sched->idle_task, sched);
  }
  sched->current_task = NULL;

  return MU_SCHED_ERR_NONE;
}

mu_pstore_t *mu_sched_task_queue(mu_sched_t *sched) {
  return sched->task_queue;
}

mu_spscq_t *mu_sched_isr_queue(mu_sched_t *sched) {
  return sched->isr_queue;
}

mu_task_t *mu_sched_get_idle_task(mu_sched_t *sched) {
  return sched->idle_task;
}

mu_task_t *mu_sched_get_default_idle_task(mu_sched_t *sched) {
  return &s_default_idle_task;
}

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *task) {
  sched->idle_task = task;
  return sched;
}

mu_clock_fn mu_sched_get_clock_source(mu_sched_t *sched) {
  return sched->clock_fn;
}

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn) {
  sched->clock_fn = clock_fn;
  return sched;
}

bool mu_sched_is_empty(mu_sched_t *sched) {
  return mu_pstore_is_empty(sched->task_queue);
}

size_t mu_sched_task_count(mu_sched_t *sched) {
  return mu_pstore_count(sched->task_queue);
}

mu_time_t mu_sched_get_current_time(mu_sched_t *sched) {
  return sched->clock_fn();
}

mu_task_t *mu_sched_get_current_task(mu_sched_t *sched) {
  return sched->current_task;
}

mu_sched_err_t mu_sched_get_next_time(mu_sched_t *sched, mu_time_t *time) {
  mu_task_t *task;
  if ((mu_pstore_peek(sched->task_queue, (void **)&task) != MU_PSTORE_ERR_NONE)) {
    return MU_SCHED_ERR_NOT_FOUND;
  } else {
    *time = mu_task_get_time(task);
    return MU_SCHED_ERR_NONE;
  }
}

bool mu_sched_task_is_scheduled(mu_sched_t *sched, mu_task_t *task) {
  return mu_pstore_contains(sched->task_queue, task);
}

mu_sched_err_t mu_sched_task(mu_sched_t *sched, mu_task_t *task) {
  mu_sched_remove_task(sched, task);  // not an error if not present...
  return mu_sched_task_fast(sched, task);
}

mu_sched_err_t mu_sched_task_fast(mu_sched_t *sched, mu_task_t *task) {
  // here when we know for certain that task is not already in the queue
  if (mu_pstore_insert_sorted(sched->task_queue,
                              task,
                              sched_ordering_fn) == MU_PSTORE_ERR_FULL) {
    return MU_SCHED_ERR_FULL;
  } else {
    return MU_SCHED_ERR_NONE;
  }
}

mu_sched_err_t mu_sched_remove_task(mu_sched_t *sched, mu_task_t *task) {
  if (mu_pstore_delete(sched->task_queue, task) == NULL) {
    return MU_SCHED_ERR_NOT_FOUND;
  } else {
    return MU_SCHED_ERR_NONE;
  }
}

// convenience functions (tbd).  if task is in aleady scheduled, will remove
// and re-add.
mu_sched_err_t mu_sched_task_now(mu_sched_t *sched, mu_task_t *task) {
  mu_time_t time = mu_sched_get_current_time(sched);
  return mu_sched_task(sched, mu_task_set_time(task, time));
}

mu_sched_err_t mu_sched_task_at(mu_sched_t *sched, mu_task_t *task, mu_time_t at) {
  return mu_sched_task(sched, mu_task_set_time(task, at));
}

mu_sched_err_t mu_sched_task_in(mu_sched_t *sched, mu_task_t *task, mu_time_t in) {
  mu_time_t time = mu_time_offset(mu_sched_get_current_time(sched), in);
  return mu_sched_task(sched, mu_task_set_time(task, time));
}

// like mu_sched_task_in, but time is relative to task's previous time
mu_sched_err_t mu_sched_task_again(mu_sched_t *sched, mu_task_t *task, mu_time_t dt) {
  return mu_sched_task(sched, mu_task_advance_time(task, dt));
}

mu_sched_err_t mu_sched_task_from_isr(mu_sched_t *sched, mu_task_t *task) {
  if (mu_spscq_put(sched->isr_queue, task) == MU_CQUEUE_ERR_FULL) {
    return MU_SCHED_ERR_FULL;
  } else {
    return MU_SCHED_ERR_NONE;
  }
}

// =============================================================================
// local (static) code

static void *idle_task_fn(void *self, void *arg) {
  // the default idle task doesn't do much...
  return self;
}

static int sched_ordering_fn(void *a, void *b) {
  return mu_task_compare_times((mu_task_t *)b, (mu_task_t *)a);
}
