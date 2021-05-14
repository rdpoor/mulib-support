/**
 * MIT License
 *
 * Copyright (c) 2021 Klatu Networks, Inc
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

#include "oblique_eg.h"
#include "strategies.h"
#include "mulib.h"
#include "mu_platform.h"
#include <stdbool.h>

// =============================================================================
// Local types and definitions

typedef struct {
  mu_task_t task;
  bool has_seed;
} oblique_ctx_t;

#define MIN_MS (10*1000)
// On average, once every 18 hours...
#define MAX_MS (36*60*60*1000)

// =============================================================================
// Local (forward) declarations

static void oblique_task_fn(void *ctx, void *arg);
static void button_cb(bool button_is_pressed);

// =============================================================================
// Local storage

static oblique_ctx_t s_oblique_ctx;

// =============================================================================
// Public code


void oblique_eg_init() {
  mulib_init();
  mu_stddemo_init(button_cb);
  mu_stddemo_led_set(false);

  mu_task_init(&s_oblique_ctx.task, oblique_task_fn, &s_oblique_ctx, "Oblique");
  s_oblique_ctx.has_seed = false;
  mu_stddemo_printf("oblique_eg: Press user button to start...\n");
}

void oblique_eg_step() {
  mu_sched_step();
}

// =============================================================================
// Local (static) code

static void oblique_task_fn(void *ctx, void *arg) {
  oblique_ctx_t *self = (oblique_ctx_t *)ctx;
  (void)arg;

  // On the first call, set the random seed from the clock, i.e. depending on how
  // much time has elapsed between startup and the button press.
  if (!self->has_seed) {
	  mu_random_seed(mu_time_now());
	  self->has_seed = true;
  }
  mu_stddemo_led_set(true);
  strategies_choose_and_print(); // print an oblique strategy
  // re-schedule the oblique_task at some random time in the future.
  mu_duration_t delay = MU_TIME_MS_TO_DURATION(mu_random_range(MIN_MS, MAX_MS));
  mu_sched_task_in(&s_oblique_ctx.task, delay);
  mu_stddemo_led_set(false);
}

static void button_cb(bool button_is_pressed) {
  mu_sched_task_from_isr(&s_oblique_ctx.task);
}
