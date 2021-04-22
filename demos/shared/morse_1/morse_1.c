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

#include "morse_1.h"

#include "mu_platform.h"  // must precede #include mulib.h
#include "mulib.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

#define ON_TIME_MS 10
#define OFF_TIME_MS (1000 - ON_TIME_MS)

// Define the context for a morse_1 task.  When morse_1_fn is called, this
// context is passed in as an argument and gives morse_1_fn all of the
// information it needs to run the task.
typedef struct {
  mu_task_t task;
  mu_duration_t on_time;
  mu_duration_t off_time;
  bool led_is_on;
} morse_1_ctx_t;

// =============================================================================
// Local (forward) declarations

/**
 * @brief Initialize a morse_task to turn an LED on and off periodically.
 *
 * @param on_time_ms How long the LED is on, in milliseconds.
 * @param off_time_ms How long the LED is off, in milliseconds.
 * @return An initialized mu_task, ready to be scheduled.
 */
static mu_task_t *morse_1_task_init(uint16_t on_time_ms, uint16_t off_time_ms);

static void morse_1_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

static morse_1_ctx_t s_morse_1_ctx;

// =============================================================================
// Public code

void morse_1_init(void) {
  mulib_init();

  printf("\r\nmorse_1 v%s\n", VERSION);

  mu_task_t *morse_task = morse_1_task_init(ON_TIME_MS, OFF_TIME_MS);
  mu_sched_task_now(morse_task);
}

void morse_1_step(void) {
  mu_sched_step();
}

// =============================================================================
// Local (private) code

mu_task_t *morse_1_task_init(uint16_t on_time_ms, uint16_t off_time_ms) {

  // initialize the mu_task to associate blink_basic_fn with s_blink_basic_ctx
  mu_task_init(&s_morse_1_ctx.task, morse_1_fn, &s_morse_1_ctx, "Morse 1");

  // Initialize s_morse_1_ctx
  s_morse_1_ctx.on_time = mu_time_ms_to_duration(on_time_ms);
  s_morse_1_ctx.off_time = mu_time_ms_to_duration(off_time_ms);
  s_morse_1_ctx.led_is_on = false;

  // Make sure the LED is initially off
  mu_stddemo_led_set(false);

  // Return the task object, ready to be passed to the scheduler
  return &s_morse_1_ctx.task;
}

static void morse_1_fn(void *ctx, void *arg) {
  // Recast the void * argument to a blink_basic_ctx_t * argument.
  morse_1_ctx_t *self = (morse_1_ctx_t *)ctx;
  (void)arg;  // unused

  // Toggle the internal state and make the LED match the internal state
  self->led_is_on = !self->led_is_on;
  mu_stddemo_led_set(self->led_is_on);

  // Reschedule the blink_basic task in the specified amount of time.
  mu_sched_reschedule_in(self->led_is_on ? self->on_time : self->off_time);
}
