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

#include "task_join_wto.h"

#include "joiner_wto.h"
#include "sleeper.h"

#include <mulib.h>
#include <stdio.h>
#include <stddef.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

#define MIN_MS 350
#define MAX_MS 3500
#define TIMEOUT_MS 2500

typedef struct {
  mu_task_t task;
  bool sleepers_are_iniitalied;
} join_wto_eg_ctx_t;

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

static void start_sleeper(sleeper_ctx_t *sleeper,
                          const char *name,
                          mu_time_t wake_at);

// =============================================================================
// Local storage

static join_wto_eg_ctx_t s_task_join_wto_ctx;
static sleeper_ctx_t s_sleeper_a;
static sleeper_ctx_t s_sleeper_b;
static sleeper_ctx_t s_sleeper_c;
static joiner_wto_ctx_t s_joiner_wto;

// =============================================================================
// Public code

void task_join_wto_init(void) {
  mulib_init();

  printf("\r\ntask_join_wto_eg v%s, mulib v%s\n", VERSION, MU_VERSION);


  s_task_join_wto_ctx.sleepers_are_iniitalied = false;
  mu_task_init(&s_task_join_wto_ctx.task, task_fn, &s_task_join_wto_ctx, "Join Wto Eg");
  mu_sched_task_now(&s_task_join_wto_ctx.task);
}

void task_join_wto_step(void) {
  mu_sched_step();
}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a join_wto_eg_ctx_t * argument.
  join_wto_eg_ctx_t *self = (join_wto_eg_ctx_t *)ctx;
  (void)arg;  // unused
  mu_time_t now = mu_rtc_now();
  mu_time_t at;

  if (self->sleepers_are_iniitalied) {
    // If the timeout triggered before all the sleeper tasks ran, then one or
    // more of the sleeper tasks are still in the schedule queue.  Be sure to
    // remove them from the scheduler, since calling mu_task_init() on a
    // scheduled task will destroy the scheduler queue.
    mu_sched_remove_task(sleeper_task(&s_sleeper_a));
    mu_sched_remove_task(sleeper_task(&s_sleeper_b));
    mu_sched_remove_task(sleeper_task(&s_sleeper_c));
  }

  // initialize the joiner object.  Upon completion (when all tasks have
  // joined), call this task again to restart the process.
  joiner_wto_init(&s_joiner_wto, &self->task);
  // Initialize three sleeper object, each with its own random delay.  Upon
  // completing, each sleeper object will notify the joiner.  After all three
  // sleeper tasks have completed, the joiner task will call its on_completion
  // task (which is this task), and the process will repeat.
  printf("-----\n");
  at = mu_time_offset(now, mu_time_ms_to_duration(mu_random_range(MIN_MS, MAX_MS)));
  start_sleeper(&s_sleeper_a, "Sleeper A", at);
  at = mu_time_offset(now, mu_time_ms_to_duration(mu_random_range(MIN_MS, MAX_MS)));
  start_sleeper(&s_sleeper_b, "Sleeper B", at);
  at = mu_time_offset(now, mu_time_ms_to_duration(mu_random_range(MIN_MS, MAX_MS)));
  start_sleeper(&s_sleeper_c, "Sleeper C", at);

  self->sleepers_are_iniitalied = true;

  // Instruct the joiner to timeout after 2500 mSec, whether or not the
  // sleeper tasks have completed.
  at = mu_time_offset(now, mu_time_ms_to_duration(TIMEOUT_MS));
  joiner_wto_set_timeout_at(&s_joiner_wto, at);
  printf("Joiner set to time out at %ld\n", at);
}

static void start_sleeper(sleeper_ctx_t *sleeper,
                          const char *name,
                          mu_time_t wake_at) {
  mu_task_t *task = sleeper_init(sleeper, name, joiner_wto_add_task(&s_joiner_wto));
  printf("%s sleeping until %ld tics\n", name, wake_at);
  mu_sched_task_at(task, wake_at);
}
