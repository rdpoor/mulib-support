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

#include "join_wto_eg.h"

#include "mu_platform.h"  // must precede #include mulib.h
#include "mulib.h"
#include "joiner_wto.h"
#include "sleeper.h"
#include <stdio.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

#define MIN_DUR MU_TIME_MS_TO_DURATION(350)
#define MAX_DUR MU_TIME_MS_TO_DURATION(3500)
#define TIMEOUT_DUR MU_TIME_MS_TO_DURATION(2500)

// NOTE: we don't strictly need the typedef here -- we could use the mu_task_t
// object directly -- but convention argues for putting it inside a ctx struct.
// Besides, we may want to add additional state to the context at some point.
typedef struct {
  mu_task_t task;
} join_wto_eg_ctx_t;

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

static void start_sleeper(sleeper_ctx_t *sleeper,
                          const char *name,
                          mu_time_t wake_at);

static mu_duration_t random_delay(mu_duration_t min, mu_duration_t max);

static mu_duration_t random();

// =============================================================================
// Local storage

static mu_duration_ms_t s_random_seed = 123456789;

static join_wto_eg_ctx_t s_join_wto_eg_ctx;
static sleeper_ctx_t s_sleeper_a;
static sleeper_ctx_t s_sleeper_b;
static sleeper_ctx_t s_sleeper_c;
static joiner_wto_ctx_t s_joiner_wto;

// =============================================================================
// Public code

void join_wto_eg_init(void) {
  mulib_init();

  printf("\r\njoin_wto_eg v%s, seed = %ld\n", VERSION, s_random_seed);

  mu_task_init(&s_join_wto_eg_ctx.task, task_fn, &s_join_wto_eg_ctx, "Join Wto Eg");

  mu_sched_task_now(&s_join_wto_eg_ctx.task);
}

void join_wto_eg_step(void) {
  mu_sched_step();
}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a join_wto_eg_ctx_t * argument.
  join_wto_eg_ctx_t *self = (join_wto_eg_ctx_t *)ctx;
  (void)arg;  // unused
  mu_time_t now = mu_time_now();
  mu_time_t timeout_at;

  // initialize the joiner object.  Upon completion (when all tasks have
  // joined), call this task again to restart the process.
  joiner_wto_init(&s_joiner_wto, &self->task);
  // Initialize three sleeper object, each with its own random delay.  Upon
  // completing, each sleeper object will notify the joiner.  After all three
  // sleeper tasks have completed, the joiner task will call its on_completion
  // task (which is this task), and the process will repeat.
  printf("-----\n");
  start_sleeper(&s_sleeper_a,
                "Sleeper A",
                mu_time_offset(now, random_delay(MIN_DUR, MAX_DUR)));
  start_sleeper(&s_sleeper_b,
                "Sleeper B",
                mu_time_offset(now, random_delay(MIN_DUR, MAX_DUR)));
  start_sleeper(&s_sleeper_c,
                "Sleeper C",
                mu_time_offset(now, random_delay(MIN_DUR, MAX_DUR)));

  // Instruct the joiner to timeout after 2500 mSec, whether or not the
  // sleeper tasks have completed.
  timeout_at = mu_time_offset(now, TIMEOUT_DUR);
  joiner_wto_set_timeout_at(&s_joiner_wto, timeout_at);
  printf("Joiner set to time out at %ld\n", timeout_at);
}

static void start_sleeper(sleeper_ctx_t *sleeper,
                          const char *name,
                          mu_time_t wake_at) {
  mu_task_t *task = sleeper_init(sleeper, name, joiner_wto_add_task(&s_joiner_wto));
  printf("%s sleeping until %ld tics\n", name, wake_at);
  mu_sched_task_at(task, wake_at);
}

#define RAND_A 1103515245
#define RAND_C 12345

static mu_duration_t random_delay(mu_duration_t min, mu_duration_t max) {
  mu_duration_t r = random();
  return min + (r % (max - min));
}


static mu_duration_t random() {
  s_random_seed = (uint32_t)((RAND_A * s_random_seed + RAND_C) & 0x7fffffff);
  return s_random_seed;
}
