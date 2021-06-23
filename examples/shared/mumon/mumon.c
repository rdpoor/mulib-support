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

#define VERSION "1.0"

typedef struct {
  mu_task_t task;
} mumon_ctx_t;

// =============================================================================
// Local (forward) declarations

static void mumon_task_fn(void *ctx, void *arg);

static uint32_t random_range(uint32_t min, uint32_t max);

// =============================================================================
// Local storage

static uint32_t s_random_seed = ((uint32_t)123456789);

static mumon_ctx_t s_mumon_ctx;

static mu_timer_t s_timer;

// =============================================================================
// Public code

void mumon_init(void) {
  mulib_init();
  mu_stddemo_init(NULL);

  printf("\r\nmumon v%s, seed = %ld\n", VERSION, s_random_seed);

  // Initialize the monitor task.
  mu_task_init(mumon_task(), mumon_task_fn, &s_mumon_ctx, "MuMonitor");

  // Start calling the monitor task once every second.
  mu_timer_start(mu_timer_periodic(&s_timer, mumon_task(),
                 MU_TIME_MS_TO_DURATION(1000));
}

void join_wto_eg_step(void) {
  // Voila: the main loop
  mu_sched_step();
}

mu_task_t *mumon_task(void) {
  return &s_mumon_ctx.task;
}

// =============================================================================
// Local (private) code

// Called once a second from a timer task.
static void mumon_task_fn(void *ctx, void *arg) {
  (void)ctx;  // unused
  (void)arg;  // unused
  char ch;

  // First check to see if a character has been typed, and if so, act on it.
  if (mu_stddemo_gets(1, &ch) == 1) {
    switch(ch) {
      case 't': {
        start_demo_task();
      } break;
      case 'R': {
        reset();
      }
    }
  }

  // start a task to update the screen
  mu_sched_now(screen_update_init());
}

static void start_demo_task(void) {
  if (s_task_index >= MAX_DEMO_TASKS) {
    s_task_index = 0;
  }
  mu_sched_now(demo_task_init(&s_demo_tasks[s_task_index], s_task_index);
  s_task_index += 1;
}


#define RAND_A ((uint32_t)1103515245)
#define RAND_C ((uint32_t)12345)

static uint32_t random_range(uint32_t min, uint32_t max) {
  s_random_seed = (uint32_t)((RAND_A * s_random_seed + RAND_C) & 0x7fffffff);
  return min + (s_random_seed % (max - min));
}
