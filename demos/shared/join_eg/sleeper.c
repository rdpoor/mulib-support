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

#include "sleeper.h"

#include "mu_platform.h"  // must precede #include mulib.h
#include "mulib.h"
#include <stdio.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

// =============================================================================
// Public code

mu_task_t *sleeper_init(sleeper_ctx_t *ctx,
                        const char *name,
                        mu_task_t *on_completion) {
  // initialize the mu_task to associate task_fn with the sleeper_ctx
  mu_task_init(&ctx->task, task_fn, ctx, "Sleeper");
  ctx->name = name;
  ctx->on_completion = on_completion;

  return &ctx->task;
}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a morse_char_task_ctx_t * argument.
  sleeper_ctx_t *self = (sleeper_ctx_t *)ctx;
  (void)arg;  // unused

  mu_stddemo_led_set(true);  // turn on LED when any sleeper wakes
  printf("Sleeper %s waking at %ld\n", self->name, mu_time_now());
  if (self->on_completion != NULL) {
    mu_sched_task_now(self->on_completion);
  }

}
