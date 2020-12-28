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

#include "mu_config.h"
#include "mu_task.h"
#include "mu_time.h"
#include <stddef.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

mu_task_t *mu_task_init(mu_task_t *task,
                        mu_task_fn fn,
                        void *self,
                        const char *name) {
  return mu_task_init_at(task, fn, self, 0, name);
}

mu_task_t *mu_task_init_at(mu_task_t *task,
                           mu_task_fn fn,
                           void *self,
                           mu_time_t at,
                           const char *name) {
  task->link.next = NULL;
  task->fn = fn;
  task->self = self;
  task->time = at;
  task->name = name;
#if (MU_TASK_PROFILING)
  task->call_count = 0;
  task->runtime = 0;
  task->max_latency = 0;
  task->max_duration = 0;
#endif
  return task;
}

const char *mu_task_name(mu_task_t *task) {
  return task->name;
}

mu_time_t mu_task_get_time(mu_task_t *task) { return task->time; }

// Set the task's time to t
mu_task_t *mu_task_set_time(mu_task_t *task, mu_time_t time) {
  task->time = time;
  return task;
}

bool mu_task_is_runnable(mu_task_t *task, mu_time_t time) {
  // true if time is equal to or before the task's time
  return !mu_time_is_after(task->time, time);
}

// Advance the task's time by dt
mu_task_t *mu_task_advance_time(mu_task_t *task, mu_time_dt dt) {
  task->time = mu_time_offset(task->time, dt);
  return task;
}

int mu_task_compare_times(mu_task_t *t1, mu_task_t *t2) {
  if (mu_time_is_equal(t1->time, t2->time)) {
    return 0;
  } else if (mu_time_is_before(t1->time, t2->time)) {
    return -1;
  } else {
    return 1;
  }
}

bool mu_task_is_after(mu_task_t *t1, mu_task_t *t2) {
  return mu_time_is_after(mu_task_get_time(t1), mu_task_get_time(t2));
}

void *mu_task_call(mu_task_t *task, void *arg) {

#if (MU_TASK_PROFILING)
  mu_time_t called_at = mu_time_now();
  mu_time_dt latency = mu_time_difference(called_at, task->time);
  if (latency > task->max_latency) task->max_latency = latency;
#endif
  void *result = task->fn(task->self, arg);
#if (MU_TASK_PROFILING)
  task->call_count += 1;
  mu_time_dt duration = mu_time_difference(mu_time_now(), called_at);
  task->runtime += duration;
  if (duration > task->max_duration) task->max_duration = duration;
#endif
  return result;
}

#if (MU_TASK_PROFILING)

unsigned int mu_task_call_count(mu_task_t *task) {
  return task->call_count;
}

mu_time_seconds_dt mu_task_runtime(mu_task_t *task) {
  return mu_time_duration_to_seconds(task->runtime);
}

mu_time_seconds_dt mu_task_max_latency(mu_task_t *task) {
  return mu_time_duration_to_seconds(task->max_latency);
}

mu_time_seconds_dt mu_task_max_duration(mu_task_t *task) {
  return mu_time_duration_to_seconds(task->max_duration);
}

#endif

// =============================================================================
// private functions
