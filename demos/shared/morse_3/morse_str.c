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

#include "morse_str.h"

#include "mu_platform.h"  // must precede #include mulib.h
#include "mulib.h"
#include "morse_char.h"
#include <stdio.h>

// =============================================================================
// Local types and definitions

typedef struct {
  mu_task_t task;
  const char *str;
  mu_task_t *on_completion;
} ctx_t;

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

static ctx_t s_ctx;

// =============================================================================
// Public code

mu_task_t *morse_str_init(const char *str, mu_task_t *on_completion) {
  mu_task_init(&s_ctx.task, task_fn, &s_ctx, "Morse Str");

  // Initialize s_morse_2_ctx
  s_ctx.str = str;
  s_ctx.on_completion = on_completion;

  // Return the task object, ready to be passed to the scheduler
  return &s_ctx.task;

}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a ctx_t * argument.
  ctx_t *self = (ctx_t *)ctx;
  (void)arg;  // unused

  // Fetch the next character in the string
  char ch = *self->str++;

  if (ch != '\0') {
    // Schedule sub-task to blink the ascii as morse code, and upon completion,
    // call this task again.
    mu_sched_task_now(morse_char_init(ch, &self->task));

  } else {
    // Completed the string.   Call the on_completion task (if provided) after
    // a one second delay.
    if (self->on_completion != NULL) {
      mu_sched_task_in(self->on_completion, MU_TIME_MS_TO_DURATION(1000));
    }
  }
}
