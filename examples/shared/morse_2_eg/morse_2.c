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

#include "morse_2.h"
#include "morse_char.h"

#include <mulib.h>
#include <stdio.h>
#include <stddef.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

typedef struct {
  mu_task_t task;
  char ascii;
} ctx_t;

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

static ctx_t s_ctx;

// =============================================================================
// Public code

void morse_2_init(void) {
  mulib_init();

  printf("\r\nmorse_2 v%s, mulib v%s\n", VERSION, MU_VERSION);

  // initialize the mu_task to associate task_fn with s_ctx
  mu_task_init(&s_ctx.task, task_fn, &s_ctx, "Morse 2");

  // Initialize s_ctx
  s_ctx.ascii = 'Y';

  mu_sched_task_now(&s_ctx.task);
}

void morse_2_step(void) {
  mu_sched_step();
}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a morse_2 ctx_t * argument.
  ctx_t *self = (ctx_t *)ctx;
  (void)arg;  // unused

  // Schedule sub-task to blink the ascii and upon completion, call this task.
  mu_sched_task_now(morse_char_init(self->ascii, &self->task));
}
