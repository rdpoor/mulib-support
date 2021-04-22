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

#include "morse_2.h"

#include "mu_platform.h"  // must precede #include mulib.h
#include "mulib.h"
#include "morse_char.h"
#include <stdio.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

#define ON_TIME_MS 10
#define OFF_TIME_MS (1000 - ON_TIME_MS)

typedef struct {
  mu_task_t task;
  char ascii;
} morse_2_ctx_t;

// =============================================================================
// Local (forward) declarations

/**
 * @brief Initialize a morse_task to turn an LED on and off periodically.
 *
 * @param on_time_ms How long the LED is on, in milliseconds.
 * @param off_time_ms How long the LED is off, in milliseconds.
 * @return An initialized mu_task, ready to be scheduled.
 */
static mu_task_t *morse_2_task_init(char ascii);

static void morse_2_fn(void *ctx, void *arg);

// =============================================================================
// Local storage

static morse_2_ctx_t s_morse_2_ctx;

// =============================================================================
// Public code

void morse_2_init(void) {
  mulib_init();

  printf("\r\nmorse_2 v%s\n", VERSION);

  mu_task_t *task = morse_2_task_init('A');
  mu_sched_task_now(task);
}

void morse_2_step(void) {
  mu_sched_step();
}

// =============================================================================
// Local (private) code

static mu_task_t *morse_2_task_init(char ascii) {

  // initialize the mu_task to associate blink_basic_fn with s_blink_basic_ctx
  mu_task_init(&s_morse_2_ctx.task, morse_2_fn, &s_morse_2_ctx, "Morse 2");

  // Initialize s_morse_2_ctx
  s_morse_2_ctx.ascii = ascii;

  // Return the task object, ready to be passed to the scheduler
  return &s_morse_2_ctx.task;
}

static void morse_2_fn(void *ctx, void *arg) {
  // Recast the void * argument to a blink_basic_ctx_t * argument.
  morse_2_ctx_t *self = (morse_2_ctx_t *)ctx;
  (void)arg;  // unused

  // Schedule sub-task to blink the ascii and upon completion, call this task.
  mu_sched_task_now(morse_char_init(self->ascii, &self->task));
}
