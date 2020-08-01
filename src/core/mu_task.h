/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

#ifndef _MU_TASK_H_
#define _MU_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_config.h"
#include "mu_time.h"

// =============================================================================
// types and definitions

/**
 * A `mu_task` is a function that can be called later.  It comprises a function
 * pointer (`mu_task_fn`) and a context (`void *ctx`).  When called, the
 * function is passed the ctx argument and a `void *` argument.
 */

// Ths signature of a mu_task function.
typedef void *(*mu_task_fn)(void *ctx, void *arg);

typedef struct _mu_task {
  mu_task_fn fn;           // function to call
  void *ctx;              // context to pass when called
#if (MU_TASK_PROFILING)
  const char *name;        // user defined task name
  unsigned int call_count; // # of time task is called
  mu_time_dt runtime;      // accumulated time spent running the task
  mu_time_dt max_duration; // max time spend running the task
#endif
} mu_task_t;

mu_task_t *mu_task_init(mu_task_t *task,
                        mu_task_fn fn,
                        void *ctx,
                        const char *name);

const char *mu_task_name(mu_task_t *task);

void *mu_task_call(mu_task_t *task, void *arg);

#if (MU_TASK_PROFILING)

  unsigned int mu_task_call_count(mu_task_t *task);

  mu_time_ms_dt mu_task_runtime_ms(mu_task_t *task);

  mu_time_ms_dt mu_task_max_duration_ms(mu_task_t *task);

  #ifdef MU_VM_FLOAT
    mu_time_seconds_dt mu_task_runtime_s(mu_task_t *task);

    mu_time_seconds_dt mu_task_max_duration_s(mu_task_t *task);
  #endif

#endif

#ifdef __cplusplus
}
#endif

#endif // #ifndef _MU_TASK_H_
