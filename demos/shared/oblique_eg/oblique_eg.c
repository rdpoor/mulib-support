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
} oblique_ctx_t;

#define MIN_MS (10*1000)
// On average, once every 18 hours...
#define MAX_MS (36*60*60*1000)

// =============================================================================
// Local (forward) declarations

static void button_cb(bool button_is_pressed);
static void oblique_setup(void);
static void oblique_printer_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

static oblique_ctx_t s_oblique_ctx;

// =============================================================================
// Public code


void oblique_eg_init() {
  mulib_init();
  mu_stddemo_init(button_cb);
  mu_stddemo_led_set(false);

  mu_task_init(&s_oblique_ctx.task, oblique_printer_fn, &s_oblique_ctx, "Oblique");
  mu_stddemo_printf("oblique_eg: Press user button or just be patient...\n");
  oblique_setup();
}

void oblique_eg_step() {
  mu_sched_step();
}

// =============================================================================
// Local (static) code

static void oblique_setup(void) {
  // Schedule the printing task at some random time in the future.
  mu_duration_t delay = MU_TIME_MS_TO_DURATION(mu_random_range(MIN_MS, MAX_MS));
  mu_sched_task_in(&s_oblique_ctx.task, delay);
}

static void oblique_printer_fn(void *ctx, void *arg) {
  (void)ctx;
  (void)arg;

  mu_stddemo_led_set(true);
  strategies_choose_and_print();
  oblique_setup();
  mu_stddemo_led_set(false);
}

static void button_cb(bool button_is_pressed) {
  // BUGFIX: Potential race condition here.  See BUGFIX note in mu_sched.
  mu_sched_remove_task(&s_oblique_ctx.task);
  mu_sched_task_from_isr(&s_oblique_ctx.task);
}
