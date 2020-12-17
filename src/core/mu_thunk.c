/**
 * MIT License
 *
 * Copyright (c) 2020 R. D. Poor <rdpoor@gmail.com>
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
<<<<<<< HEAD
// includes

<<<<<<<< HEAD:src/core/mu_thunk.c
#include "mulib.h"
========
#include "mu_config.h"
#include "mu_task.h"
#include "mu_thunk.h"
#include "mu_time.h"
>>>>>>>> develop-rdp:src/core/mu_task.c
#include <stddef.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

mu_thunk_t *mu_thunk_init(mu_thunk_t *thunk,
                        mu_thunk_fn fn,
                        void *ctx,
                        const char *name) {
<<<<<<<< HEAD:src/core/mu_thunk.c
  thunk->link.next = NULL;
  thunk->time = 0;
  thunk->fn = fn;
  thunk->ctx = ctx;
#if (MU_THUNK_PROFILING)
  thunk->name = name;
  thunk->call_count = 0;
  thunk->runtime = 0;
  thunk->max_duration = 0;
========
  task->link.next = NULL;
  task->time = 0;
  mu_thunk_init(task->thunk, fn, ctx);
#if (MU_TASK_PROFILING)
  task->name = name;
  task->call_count = 0;
  task->runtime = 0;
  task->max_duration = 0;
>>>>>>>> develop-rdp:src/core/mu_task.c
#endif
  return thunk;
}

mu_list_t mu_thunk_link(mu_thunk_t *thunk) {
  return thunk->link;
}

mu_time_t mu_thunk_get_time(mu_thunk_t *thunk) {
  return thunk->time;
}

void mu_thunk_set_time(mu_thunk_t *thunk, mu_time_t time) {
  thunk->time = time;
}

<<<<<<<< HEAD:src/core/mu_thunk.c
mu_thunk_fn mu_thunk_get_fn(mu_thunk_t *thunk) {
  return thunk->fn;
}

void *mu_thunk_get_context(mu_thunk_t *thunk) {
  return thunk->ctx;
========
mu_thunk_t *mu_test_get_thunk(mu_task_t *task) {
  return &task->thunk;
}

mu_task_fn mu_task_get_fn(mu_task_t *task) {
  return mu_thunk_get_fn(task->thunk);
}

void *mu_task_get_ctx(mu_task_t *task) {
  return mu_thunk_get_ctx(task->thunk);
>>>>>>>> develop-rdp:src/core/mu_task.c
}

const char *mu_thunk_name(mu_thunk_t *thunk) {
#if (MU_THUNK_PROFILING)
  return thunk->name;
#else
  return "";
#endif
}

void *mu_thunk_call(mu_thunk_t *thunk, void *arg) {

#if (MU_THUNK_PROFILING)
  mu_time_t called_at = mu_time_now();
#endif
<<<<<<<< HEAD:src/core/mu_thunk.c
  void *result = thunk->fn(thunk->ctx, arg);
#if (MU_THUNK_PROFILING)
  thunk->call_count += 1;
========
  void *result = mu_thunk_call(task->thunk);
#if (MU_TASK_PROFILING)
  task->call_count += 1;
>>>>>>>> develop-rdp:src/core/mu_task.c
  mu_time_dt duration = mu_time_difference(mu_time_now(), called_at);
  thunk->runtime += duration;
  if (duration > thunk->max_duration) thunk->max_duration = duration;
#endif
  return result;
}

#if (MU_THUNK_PROFILING)

unsigned int mu_thunk_call_count(mu_thunk_t *thunk) {
  return thunk->call_count;
}

mu_time_ms_dt mu_thunk_runtime_ms(mu_thunk_t *thunk) {
  return mu_time_duration_to_ms(thunk->runtime);
}

mu_time_ms_dt mu_thunk_max_duration_ms(mu_thunk_t *thunk) {
  return mu_time_duration_to_ms(thunk->max_duration);
}

#ifdef MU_FLOAT
mu_time_s_dt mu_thunk_runtime_s(mu_thunk_t *thunk) {
  return mu_time_duration_to_s(thunk->runtime);
}

mu_time_s_dt mu_thunk_max_duration_s(mu_thunk_t *thunk) {
  return mu_time_duration_to_s(thunk->max_duration);
}
#endif

#endif

// =============================================================================
// private functions
=======
// Includes

#include "mu_thunk.h"
#include "mu_config.h"
#include <stddef.h>

// =============================================================================
// Private types and definitions

// =============================================================================
// Private declarations

// =============================================================================
// Local storage

// =============================================================================
// Public code

mu_thunk_t *mu_thunk_init(mu_thunk_t *thunk, mu_thunk_fn fn, void *ctx) {
  thunk->fn = fn;
  thunk->ctx = ctx;
  return thunk;
}

mu_thunk_fn mu_thunk_get_fn(mu_thunk_t *thunk) { return thunk->fn; }

void *mu_thunk_get_ctx(mu_thunk_t *thunk) { return thunk->ctx; }

void *mu_thunk_call(mu_thunk_t *thunk, void *arg) {
  return thunk->fn(thunk->ctx, arg);
}

// =============================================================================
// Private functions
>>>>>>> develop-rdp
