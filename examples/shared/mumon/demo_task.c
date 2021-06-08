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

#include "mumon.h"

#include "mu_platform.h"  // must precede #include mulib.h
#include "mulib.h"
#include <stdio.h>
#include <stddef.h>

// =============================================================================
// Local types and definitions

#define MAX_DEMO_TASKS 100

typedef struct {
  mu_task_t task;
  char name[11];    // DemoTaskXX\n
  int remaining_wakes;
  mu_time_t wake_at;
} demo_task_ctx_t;

// =============================================================================
// Local (forward) declarations

mu_time_t choose_wake_time(void);

// =============================================================================
// Local storage

static demo_task_ctx_t s_demo_tasks[MAX_DEMO_TASKS];

static int s_task_index;

// =============================================================================
// Public code

void demo_task_init(void) {
  for (int i=0; i<MAX_DEMO_TASKS; i++) {
    demo_task_ctx_t *demo_task_ctx = &s_demo_tasks[i];
    demo_task_init(demo_task_ctx);
  }
  s_task_index = 0;
}

void demo_task_start(void) {
  if (s_task_index >= MAX_DEMO_TASKS) {
    s_task_index = 0;
  }
  demo_task_ctx_t *ctx = &s_demo_tasks[s_task_index];
  sprintf(ctx->name, "DemoTask%02d", s_task_index);
  mu_task_init(&ctx->task, demo_task_fn, ctx, ctx->name);
  ctx->remaining_wakes = 10;
  mu_sched_task_at(&ctx->task, choose_wake_time());

  s_task_index += 1;

}

// =============================================================================
// Local (private) code

static void demo_task_fn(void *ctx, void *arg) {
  demo_task_ctx_t *self = (demo_task_ctx_t *)ctx;
  (void)arg;  // unused

  if (self->remaining_wakes > 0) {
    self->remaining_wakes -= 1;
    mu_sched_task_at(&ctx->task, choose_wake_time());
  }
}

static mu_time_t choose_wake_time(void) {
  mu_time_t at =
      mu_time_offset(mu_time_now(),
                     mu_time_ms_to_duration(random_range(MIN_MS, MAX_MS)));
  return at;
}

#define RAND_A ((uint32_t)1103515245)
#define RAND_C ((uint32_t)12345)

static uint32_t random_range(uint32_t min, uint32_t max) {
  s_random_seed = (uint32_t)((RAND_A * s_random_seed + RAND_C) & 0x7fffffff);
  return min + (s_random_seed % (max - min));
}
