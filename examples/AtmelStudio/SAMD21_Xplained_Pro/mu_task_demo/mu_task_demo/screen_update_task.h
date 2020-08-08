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

#ifndef _SCREEN_UPDATE_TASK_H_
#define _SCREEN_UPDATE_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"

// =============================================================================
// types and definitions

#define SCREEN_UPDATE_INTERVAL_MS 2500

typedef struct {
  mu_task_t *screen_redraw_task;
  size_t n_tasks;
  mu_task_t *tasks;
  mu_sched_t *sched;
  mu_substr_t *screen_buffer;
  uint8_t slice;
} screen_update_ctx_t;

// =============================================================================
// declarations

mu_task_t *screen_update_task_init(mu_task_t *screen_update_task,
                                   screen_update_ctx_t *screen_update_ctx);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _SCREEN_UPDATE_TASK_H_ */
