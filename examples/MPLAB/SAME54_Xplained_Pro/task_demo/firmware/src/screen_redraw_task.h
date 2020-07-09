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

#ifndef _SCREEN_REDRAW_TASK_H_
#define _SCREEN_REDRAW_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_sched.h"
#include "mu_task.h"

// =============================================================================
// types and definitions

typedef struct {
  int s1;             // major state (steps through lines to display)
  int s2;             // minor state (steps through tasks to display)
  mu_task_t *tasks;
  size_t n_tasks;
} screen_redraw_ctx_t;

// =============================================================================
// declarations

mu_task_t *screen_redraw_task_init(mu_task_t *screen_redraw_task,
                                   screen_redraw_ctx_t *screen_redraw_ctx,
                                   mu_task_t *tasks,
                                   size_t n_tasks);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _SCREEN_REDRAW_TASK_H_ */
