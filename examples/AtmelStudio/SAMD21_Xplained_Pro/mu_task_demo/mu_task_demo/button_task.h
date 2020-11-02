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

#ifndef _BUTTON_TASK_H_
#define _BUTTON_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"

// =============================================================================
// types and definitions

// Most tasks won't need to keep a reference to both the task and scheduler
// objects.  However, the button task is triggered from an interrupt, and the
// interrupt registration function accepts a single pointer.  From within the
// interrupt context, we need to access both the task and the scheduler.

typedef struct {
  mu_thunk_t *thunk;
  mu_sched_t *sched;
} button_ctx_t;

// =============================================================================
// declarations

mu_thunk_t *button_task_init(mu_thunk_t *button_task, button_ctx_t *button_ctx);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _BUTTON_TASK_H_ */
