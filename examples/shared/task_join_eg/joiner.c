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
// Includes

#include "joiner.h"

#include <mulib.h>
#include <stdio.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

static void timeout_task_fn(void *ctx, void *arg);

static void endgame(joiner_wto_ctx_t *self, void *arg, const char *cause);

// =============================================================================
// Local storage

// =============================================================================
// Public code

mu_task_t *joiner_init(joiner_ctx_t *ctx, mu_task_t *on_completion) {
  // initialize the mu_task to associate task_fn with the joiner_ctx
  mu_task_init(&ctx->task, task_fn, ctx, "Joiner");
  ctx->on_completion = on_completion;
  ctx->pending_count = 0;

  return &ctx->task;
}

mu_task_t *joiner_add_task(joiner_ctx_t *ctx) {
  ctx->pending_count += 1;
  return &ctx->task;
}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  joiner_ctx_t *self = (joiner_ctx_t *)ctx;
  (void)arg;  // unused

  self->pending_count -= 1;

  printf("Task completed, pending count = %d\n", self->pending_count);

  if (self->pending_count == 0) {
    mu_led_io_set(MU_LED_0, false);  // turn off LED when all sleepers complete
  	printf("All tasks have joined at %ld tics.\n\n", mu_time_now());
    // all joined tasks have completed -- invoke the on_completion task
    if (self->on_completion != NULL) {
      mu_sched_task_now(self->on_completion);
    }
  }
}
