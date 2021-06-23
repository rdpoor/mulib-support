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

#include "join_eg.h"

#include "mu_platform.h"  // must precede #include mulib.h
#include "mulib.h"
#include "joiner.h"
#include "sleeper.h"
#include <stdio.h>
#include <stddef.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

#define MIN_MS 350
#define MAX_MS 3500

typedef struct {
  mu_task_t task;
} ctx_t;

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

static void start_sleeper(sleeper_ctx_t *sleeper,
                          const char *name,
                          mu_time_t wake_at);

static uint32_t random_range(uint32_t min, uint32_t max);

// =============================================================================
// Local storage

static uint32_t s_random_seed = 123456789;

static ctx_t s_ctx;
static sleeper_ctx_t s_sleeper_a;
static sleeper_ctx_t s_sleeper_b;
static sleeper_ctx_t s_sleeper_c;
static joiner_ctx_t s_joiner;

// =============================================================================
// Public code

void join_eg_init(void) {
  mulib_init();
  mu_stddemo_init(NULL);

  printf("\r\njoin_eg v%s, seed = %ld\n", VERSION, s_random_seed);

  mu_task_init(&s_ctx.task, task_fn, &s_ctx, "Join Eg");
  mu_sched_task_now(&s_ctx.task);
}

void join_eg_step(void) {
  mu_sched_step();
}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a ctx_t * argument.
  ctx_t *self = (ctx_t *)ctx;
  (void)arg;  // unused
  mu_time_t now = mu_time_now();
  mu_time_t at;

  // initialize the joiner object.  Upon completion (when all tasks have
  // joined), call this task again to restart the process.
  joiner_init(&s_joiner, &self->task);
  // Initialize three sleeper object, each with its own random delay.  Upon
  // completing, each sleeper object will notify the joiner.  After all three
  // sleeper tasks have completed, the joiner task will call its on_completion
  // task (which is this task), and the process will repeatt.
  printf("-----\n");
  at = mu_time_offset(now, mu_time_ms_to_duration(random_range(MIN_MS, MAX_MS)));
  start_sleeper(&s_sleeper_a, "Sleeper A", at);
  at = mu_time_offset(now, mu_time_ms_to_duration(random_range(MIN_MS, MAX_MS)));
  start_sleeper(&s_sleeper_b, "Sleeper B", at);
  at = mu_time_offset(now, mu_time_ms_to_duration(random_range(MIN_MS, MAX_MS)));
  start_sleeper(&s_sleeper_c, "Sleeper C", at);

  ctx->sleepers_are_iniitalied = true;
}

static void start_sleeper(sleeper_ctx_t *sleeper,
                          const char *name,
                          mu_time_t wake_at) {
  mu_task_t *task = sleeper_init(sleeper, name, joiner_add_task(&s_joiner));
  printf("%s sleeping until %ld tics\n", name, wake_at);
  mu_sched_task_at(task, wake_at);
}

#define RAND_A 1103515245
#define RAND_C 12345

static uint32_t random_range(uint32_t min, uint32_t max) {
  s_random_seed = (uint32_t)((RAND_A * s_random_seed + RAND_C) & 0x7fffffff);
  return min + (s_random_seed % (max - min));
}
