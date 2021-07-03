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


#define VERSION "1.1"

// The min and max wait times before offering a new strategy
// on average once every 1/2 hour...
#define MIN_MS (10L * 1000L)
#define MAX_MS (60L * 60L * 1000L)

// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);
static void kbd_cb(unsigned char ch);


static void oblique_task_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

typedef struct {
  mu_task_t task;
} oblique_ctx_t;

static oblique_ctx_t s_oblique_ctx;
static bool user_was_impatient = false;
static char _most_recent_character = 'x';

// =============================================================================
// Public code

/**
 * @brief Seed the random number generator, then start a chain of tasks to print strategies
 * For the seed, we increment an int in a while loop, interrupted by either a button press or a mu_kbd_io key press
 * 
 */

void oblique_init() {
  uint32_t seed = 0;
  mulib_init();
  mu_platform_init();

  mu_task_init(&s_oblique_ctx.task, oblique_task_fn, &s_oblique_ctx, "Oblique");
  mu_button_io_set_callback(button_cb);
  mu_kbd_io_set_callback(kbd_cb);
  mu_ansi_term_clear_screen();
  printf("oblique_app v%s mulib v%s: Press user button or any key to start...\n", VERSION, MU_VERSION);
  user_was_impatient = false;
  while (!user_was_impatient) {
    seed += 1;
  }
  user_was_impatient = false;
  mu_random_seed(seed);
  mu_sched_task_now(&s_oblique_ctx.task); // initiates the periodic printing of a new strategy
}

void oblique_step() {
  mu_sched_step();
  if(user_was_impatient) {
    if(_most_recent_character == 'q') exit(0);
    strategies_choose_and_print(); // print an oblique strategy immediately
    user_was_impatient = false;
  }
}


// =============================================================================
// Local (static) code

static void oblique_task_fn(void *ctx, void *arg) {
  (void)ctx;
  (void)arg;
  strategies_choose_and_print(); // print an oblique strategy
  // re-schedule the oblique_task at some random time in the future.
  mu_sched_reschedule_in(MU_TIME_MS_TO_DURATION(mu_random_range(MIN_MS, MAX_MS)));
}

static void button_cb(uint8_t button_id, bool button_is_pressed) {
  (void)button_id;
  user_was_impatient = true;
}

static void kbd_cb(unsigned char ch) {
  // TODO -- calling this from POSIX thread violates the single thread thing -- really should just call sched_isr_task_now BUT we cant store ch without violating this...
  _most_recent_character = ch; 
  user_was_impatient = true;
}

