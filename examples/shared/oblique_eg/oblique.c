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

/**
 * @brief Example demonstrating the use of the mu_scheduler to drive terminal animations.
 * Content from Oblique Strategies created by Brian Eno and Peter Schmidt
 */

// =============================================================================
// Includes

#include "oblique.h"
#include "strategies.h"
#include <mulib.h>
#include "mu_platform.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// =============================================================================
// Local types and definitions


#define VERSION "1.0"

// The min and max wait times before offering a new strategy
// on average once every 1/2 hour...
#define MIN_MS (10L * 1000L)
#define MAX_MS (60L * 60L * 1000L)

// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);

static void oblique_task_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

typedef struct {
  mu_task_t task;
} oblique_ctx_t;

static oblique_ctx_t s_oblique_ctx;

static bool button_was_pressed = false;

// =============================================================================
// Public code

/**
 * @brief Seed the random number generator, start a chain of tasks to print strategies
 * For the seed, we increment an int in a while loop, interrupted by either a button press of ansi term key press
 * 
 */

void oblique_init() {
  uint32_t seed = 0;
  mulib_init();
  mu_task_init(&s_oblique_ctx.task, oblique_task_fn, &s_oblique_ctx, "Oblique");
  mu_button_io_set_callback(button_cb);
  mu_begin_polling_for_keypress(); 

  printf("oblique_app v%s mulib v%s: Press user button or any key to start...\n", VERSION, MU_VERSION);
  while (!button_was_pressed && mu_term_get_current_keypress() == 0) {
    mu_sched_step(); // because no IRQ is available for reading the terminal, we called mu_begin_polling_for_keypress() to make the temrinal noncanonical and check it several times per second
    seed += 1;
  }

  mu_random_seed(seed);
  mu_sched_task_now(&s_oblique_ctx.task); // sets up the periodic printing of a new strategy -- independent of any subsequent keypresses
}

void oblique_step() {
  mu_sched_step();
  unsigned char kp = mu_term_get_current_keypress();
  if(kp == 'q') exit(0);
  if(kp) {
    strategies_choose_and_print(); // print an oblique strategy immediately
  }
}


// =============================================================================
// Local (static) code

static void oblique_task_fn(void *ctx, void *arg) {
  (void)ctx;
  (void)arg;
  strategies_choose_and_print(); // print an oblique strategy
  // re-schedule the oblique_task at some random time in the future.
  mu_duration_t delay = MU_TIME_MS_TO_DURATION(mu_random_range(MIN_MS, MAX_MS));
  mu_sched_task_in(&s_oblique_ctx.task, delay);
}

static void button_cb(uint8_t button_id, bool button_is_pressed) {
  (void)button_id;
  button_was_pressed = true;
}
