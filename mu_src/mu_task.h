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

#ifndef MU_TASK_H_
#define MU_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MU_TASK_PROFILING (1)

#include "mu_time.h"

/**
 * A `mu_task` is essentially a function that can be called later.  It comprises
 * a function pointer (`mu_task_fn`) and a context (`void *self`).  When called,
 * the function is passed the self argument and a `void *` argument.
 */

// Ths signature of o mu_task function.
typedef void (*mu_task_fn)(void *self, void *arg);

typedef struct _mu_task {
  struct _mu_task *next;  // link to next older event
  mu_task_fn fn;
  void *self;
  mu_time_t time;          // time of this event (ignored if is_immediate)
  bool is_immediate;       // true if this event is scheduled for "now".
#if (MU_TASK_PROFILING)
  const char *name;
  unsigned int call_count;
  mu_time_dt runtime;
  mu_time_dt max_latency;
#endif
} mu_task_t;

mu_task_t *mu_task_init_immed(mu_task_t *task,
                            mu_task_fn fn,
                            void *self,
                            const char *name);

mu_task_t *mu_task_init_at(mu_task_t *task,
                         mu_time_t time,
                         mu_task_fn fn,
                         void *self,
                         const char *name);

bool mu_task_is_immediate(mu_task_t *task);

mu_task_t *mu_task_set_immediate(mu_task_t *task);

mu_task_t *mu_task_set_time(mu_task_t *task, mu_time_t time);

// Offset the event's time by dt
mu_task_t *mu_task_advance_time(mu_task_t *task, mu_time_dt dt);

mu_time_t mu_task_time(mu_task_t *task);

// Return true if t1 occurs after t2
bool mu_task_is_after(mu_task_t *t1, mu_task_t *t2);

bool mu_task_is_runnable(mu_task_t *task, mu_time_t at);

void mu_task_call(mu_task_t *task, void *arg);

#if (MU_TASK_PROFILING)

const char *mu_task_name(mu_task_t *task);

unsigned int mu_task_call_count(mu_task_t *task);

mu_time_seconds_t mu_task_runtime(mu_task_t *task);

mu_time_seconds_t mu_task_max_latency(mu_task_t *task);

#endif

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_TASK_H_
