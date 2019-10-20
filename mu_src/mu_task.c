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

#include "mu_task.h"
#if (MU_TASK_PROFILING)
#include "mu_time.h"
#endif
#include <stddef.h>
#include <stdio.h>

mu_task_t *mu_task_init(mu_task_t *task,
                        mu_task_fn fn,
                        void *self,
                        const char *name) {
  task->fn = fn;
  task->self = self;
#if (MU_TASK_PROFILING)
  task->name = name;
  task->call_count = 0;
  task->runtime = 0;
  task->max_latency = 0;
#else
  (void)name;
#endif
  return task;
}

void mu_task_call(mu_task_t *task, void *arg) {
  if (task == NULL) return;
#if (MU_TASK_PROFILING)
  mu_time_dt duration;
  mu_time_t called_at = mu_time_now();
#endif
  if (task->fn != NULL) {
    task->fn(task->self, arg);
  }
#if (MU_TASK_PROFILING)
  task->call_count += 1;
  duration = mu_time_difference(mu_time_now(), called_at);
  task->runtime += duration;
  if (duration > task->max_latency) task->max_latency = duration;
  asm("nop");
#endif
}

#if (MU_TASK_PROFILING)

const char *mu_task_name(mu_task_t *task) {
  return task->name;
}

unsigned int mu_task_call_count(mu_task_t *task) {
  return task->call_count;
}

mu_time_seconds_t mu_task_runtime(mu_task_t *task) {
  return mu_time_duration_to_seconds(task->runtime);
}

mu_time_seconds_t mu_task_max_latency(mu_task_t *task) {
  return mu_time_duration_to_seconds(task->max_latency);
}

void mu_task_print(mu_task_t *task, char *buf, int buflen) {
  snprintf(buf, buflen, "%s: %d %f\n",
           mu_task_name(task),
           mu_task_call_count(task),
           mu_task_runtime(task));
}

#endif

// =============================================================================
// private functions