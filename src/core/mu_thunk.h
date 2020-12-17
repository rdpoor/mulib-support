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

#ifndef _MU_THUNK_H_
#define _MU_THUNK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

<<<<<<< HEAD
#include "mu_list.h"
#include "mu_thunk.h"
=======
#include "mu_config.h"
#include "mu_list.h"
>>>>>>> develop-rdp
#include "mu_time.h"

// =============================================================================
// types and definitions

/**
<<<<<<< HEAD
<<<<<<<< HEAD:src/core/mu_thunk.h
 * A `mu_thunk` is a function that can be called later.  It comprises a function
 * pointer (`mu_thunk_fn`) and a context (`void *ctx`).  When called, the
 * function is passed the ctx argument and a `void *` argument.
 *
 * It also includes a link field and a time field, which are actively used by
 * the scheduler.
 */

// Ths signature of a mu_thunk function.
typedef void *(*mu_thunk_fn)(void *ctx, void *arg);

typedef struct _mu_thunk {
  mu_list_t link; // next (older) event in the schedule
  mu_time_t time; // time at which this task fires
  mu_thunk_fn fn;  // function to call
  void *ctx;      // context to pass when called
#if (MU_THUNK_PROFILING)
========
 * A `mu_task` is a function that can be scheduled and be called later.  It
 * comprises a mu_thunk and a link field and a time field, the latter two of
 * which are used by the scheduler.
 */

// Ths signature of a mu_task function.
typedef mu_thunk_fn mu_task_fn;

typedef struct _mu_task {
  mu_list_t link;   // next (older) event in the schedule
  mu_time_t time;   // time at which this task fires
  mu_thunk_t thunk; // the underlying thunk
#if (MU_TASK_PROFILING)
>>>>>>>> develop-rdp:src/core/mu_task.h
  const char *name;        // user defined task name
  unsigned int call_count; // # of times task is called
  mu_time_dt runtime;      // accumulated time spent running the task
  mu_time_dt max_duration; // max time spend running the task
#endif
} mu_thunk_t;

mu_thunk_t *mu_thunk_init(mu_thunk_t *thunk,
                        mu_thunk_fn fn,
                        void *ctx,
                        const char *name);

mu_list_t mu_thunk_link(mu_thunk_t *thunk);

mu_time_t mu_thunk_get_time(mu_thunk_t *thunk);

void mu_thunk_set_time(mu_thunk_t *thunk, mu_time_t time);

<<<<<<<< HEAD:src/core/mu_thunk.h
mu_thunk_fn mu_thunk_get_fn(mu_thunk_t *thunk);

void *mu_thunk_get_context(mu_thunk_t *thunk);
========
mu_thunk_t *mu_test_get_thunk(mu_task_t *task);

mu_task_fn mu_task_get_fn(mu_task_t *task);

void *mu_task_get_ctx(mu_task_t *task);
>>>>>>>> develop-rdp:src/core/mu_task.h

const char *mu_thunk_name(mu_thunk_t *thunk);

void *mu_thunk_call(mu_thunk_t *thunk, void *arg);

#if (MU_THUNK_PROFILING)

unsigned int mu_thunk_call_count(mu_thunk_t *thunk);

mu_time_ms_dt mu_thunk_runtime_ms(mu_thunk_t *thunk);

mu_time_ms_dt mu_thunk_max_duration_ms(mu_thunk_t *thunk);

#ifdef MU_FLOAT
mu_time_s_dt mu_thunk_runtime_s(mu_thunk_t *thunk);

mu_time_s_dt mu_thunk_max_duration_s(mu_thunk_t *thunk);
#endif

#endif

=======
 * A `mu_thunk` is a function that can be called later.  It comprises a function
 * pointer (`mu_thunk_fn`) and a context (`void *ctx`).  When called, the
 * function is passed the ctx argument and a caller-supplied `void *` argument.
 */

// The signature of a mu_thunk function.
typedef void *(*mu_thunk_fn)(void *ctx, void *arg);

typedef struct _mu_thunk {
  mu_thunk_fn fn; // function to call
  void *ctx;      // context to pass when called
} mu_thunk_t;

mu_thunk_t *mu_thunk_init(mu_thunk_t *thunk, mu_thunk_fn fn, void *ctx);

mu_thunk_fn mu_thunk_get_fn(mu_thunk_t *thunk);

void *mu_thunk_get_ctx(mu_thunk_t *thunk);

void *mu_thunk_call(mu_thunk_t *thunk, void *arg);

>>>>>>> develop-rdp
#ifdef __cplusplus
}
#endif

#endif // #ifndef _MU_THUNK_H_
