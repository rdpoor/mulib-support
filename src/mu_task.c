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
#if (MU_TASK_PROFILING == 1)
#include "mu_time.h"
#endif
#include <stddef.h>

#if (MU_TASK_PROFILING != 1)
mu_task_t *mu_task_init(mu_task_t *task, mu_task_fn fn, void *self) {
  task->fn = fn;
  task->self = self;
  return task;
}

#else
mu_task_t *mu_task_init(mu_task_t *task,
                        mu_task_fn fn,
                        void *self,
                        const char *name) {
  task->fn = fn;
  task->self = self;
  task->call_count = 0;
  task->runtime = 0;
  return task;
}
#endif


#if (MU_TASK_PROFILING != 1)

void mu_task_call(mu_task_t *task, void *arg) {
  if (task && task->fn != NULL) {
    task->fn(task->self, arg);
  }
}

#else

void mu_task_call(mu_task_t *task, void *arg) {
  task->call_count += 1;
  task->called_at = mu_time_now();
  if (task && task->fn != NULL) {
    task->fn(task->self, arg);
  }
  task->runtime += mu_time_difference(mu_time_now(), task->called_at);
}

const char *mu_task_name(mu_task_t *task) {
  return task->name;
}

unsigned int mu_task_call_count(mu_task_t *task) {
  return task->call_count;
}

float mu_task_runtime(mu_task_t *task) {
  return mu_time_duration_to_seconds(task->runtime);
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
