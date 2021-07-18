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

#include "pong_1.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <mulib.h>
#include "mu_platform.h"

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

#define FRAME_PERIOD_MS 100

// Define the context for a pong_1 task.  When task_fn is called, this
// context is passed in as an argument and gives task_fn all of the
// information it needs to run the task.
typedef struct {
  mu_task_t task;
  mu_duration_t frame_period;
  bool led_is_on;
} ctx_t;

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

static ctx_t s_ctx;

// =============================================================================
// Public code

void morse_1_init(void) {
  mulib_init();
  mu_platform_init();
  mu_ansi_term_clear_screen();
  // initialize the mu_task to associate function (task_fn) with context (s_ctx)
  mu_task_init(&s_ctx.task, task_fn, &s_ctx, "Pong 1");

  // Initialize the context's initial state
  s_ctx.frame_period = MU_TIME_MS_TO_DURATION(FRAME_PERIOD_MS);

  // Make the first call to the scheduler to start things off.  The task_fn will
  // reschedule itself upon completion.
  mu_sched_task_now(&s_ctx.task);
}


void morse_1_step(void) {
  // Just run the scheduler
  mu_sched_step();
}

// =============================================================================
// Local (private) code

// task_fn is invoked whenever morse_1's task is triggered.  It is called with
// two arguments: morse_1's state ('context') and a user-supplied argument
// (currently unused).
//
// If the LED is off, turn it on for on_time.  If the LED is on, turn it off
// for off_time.
//
static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a morse_1 ctx_t * argument.
  ctx_t *self = (ctx_t *)ctx;
  (void)arg;  // unused

  mu_sched_reschedule_in(s_ctx.frame_period);
}
