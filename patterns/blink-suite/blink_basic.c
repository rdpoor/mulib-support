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

#include "blink_basic.h"
#include "mulib.h"
#include "blink_bsp.h"
#include <stdbool.h>

// =============================================================================
// Local types and definitions

// Define the context for a blink_basic task.  When blink_basic_fn is called,
// this context is passed in as an argument and gives blink_basic_fn all of the
// information it needs to run the task.
typedef struct {
  mu_task_t task;
  mu_time_dur_t on_time;
  mu_time_dur_t off_time;
  bool led_is_on;
} blink_basic_ctx_t;

// =============================================================================
// Local storage

// Allocate storage for the blink_basic context.
static blink_basic_ctx_t s_blink_basic_ctx;

// =============================================================================
// Local (forward) declarations

// Provide a forward declaration to the blink_basic_fn()
static void *blink_basic_fn(void *ctx, void *arg);

// =============================================================================
// Public code

mu_task_t *blink_basic_init(uint16_t on_time_ms, off_time_ms) {

  // initialize the mu_task to associate blink_basic_fn with s_blink_basic_ctx
  mu_task_init(&s_blink_basic_ctx.task, blink_basic_fn, &s_blink_basic_ctx, "Blink Basic");

  // Set up other state in s_blink_basic_ctx
  // TODO: refactor mu_time_ms_to_dt => mu_time_ms_to_dur_t
  s_blink_basic.on_time = mu_time_ms_to_dt(on_time_ms);
  s_blink_basic.off_time = mu_time_ms_to_dt(off_time_ms);
  s_blink_basic_ctx.led_is_on = false;

  // Make sure the LED is initially off
  blink_bsp_set_led(LED_A, false);

  // Return the task object, ready to be passed to the scheduler
  return &s_blink_basic_ctx.task;
}

// =============================================================================
// Local (static) code

static void blink_basic_fn(void *ctx) {
  // Recast the void * argument to a blink_basic_ctx_t * argument.
  blink_basic_ctx_t *self = (blink_basic_ctx_t *)ctx;

  // Toggle the internal state and make the LED match the internal state
  self->led_is_on = !self->led_is_on;
  blink_bsp_set_led(LED_A, self->led_is_on);

  // Reschedule the blink_basic task in the specified amount of time.
  mu_reschedule_in(self->led_is_on ? self->on_time : self->off_time);
}
