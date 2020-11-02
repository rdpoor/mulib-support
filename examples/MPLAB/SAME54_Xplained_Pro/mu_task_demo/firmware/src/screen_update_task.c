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

// =============================================================================
// includes

#include "definitions.h"
#include "screen_update_task.h"
#include "mulib.h"
#include <stddef.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *screen_update_task_fn(void *ctx, void *arg);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_thunk_t *screen_update_task_init(mu_thunk_t *screen_update_task, screen_update_ctx_t *screen_update_ctx, mu_thunk_t *screen_redraw_task) {
  // capture a pointer to the screen redraw task, needed in screen_update_task_fn
  screen_update_ctx->screen_redraw_task = screen_redraw_task;

  // initialize the screen update task
  mu_thunk_init(screen_update_task, screen_update_task_fn, screen_update_ctx, "Screen Update");
  return screen_update_task;
}

// =============================================================================
// local (static) code

static void *screen_update_task_fn(void *ctx, void *arg) {
  // screen_update_context is passed as the first argument, scheduler is second
  screen_update_ctx_t *screen_update_ctx = (screen_update_ctx_t *)ctx;
  mu_sched_t *sched = (mu_sched_t *)arg;

  // schedule a call to the screen_redraw task now...
  mu_sched_task_now(sched, screen_update_ctx->screen_redraw_task);
  // ... and schedule a subsequent call to the screen_update_task
  mu_sched_reschedule_in(sched, mu_time_ms_to_duration(SCREEN_UPDATE_INTERVAL_MS));

  return NULL;
}
