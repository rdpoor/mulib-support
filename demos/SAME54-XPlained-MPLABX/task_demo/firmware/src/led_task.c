/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
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
// includes

#include "led_task.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_time.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

typedef enum { LED_OFF, LED_ON } led_state_t;

typedef struct {
  led_state_t state;
} led_ctx_t;


// =============================================================================
// local (forward) declarations

static void *led_task_fn(void *ctx, void *arg);

// =============================================================================
// local storage

// the LED task blinks the LED
static mu_task_t s_led_task;
static led_ctx_t s_led_ctx;

// =============================================================================
// public code

mu_task_t *led_task_init(void) {
  s_led_ctx.state = LED_OFF;
  mu_task_init(&s_led_task, led_task_fn, &s_led_ctx, "LED");
  return &s_led_task;
}

// =============================================================================
// local (static) code

static void *led_task_fn(void *ctx, void *arg) {
  // "context" is passed as the first argument
  led_ctx_t *led_ctx = (led_ctx_t *)ctx;

  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;

  // Note that the current event time is the time at which the LED task was
  // ready to run -- the actual start time might be later due to latency or
  // other tasks.  To avoid drift, we increment the current event time by the
  // desired amount and use that as the next scheduled time.
  mu_event_t *event = mu_sched_get_current_event(sched);
  mu_time_t t_prev = mu_event_get_time(event);
  mu_time_t t_next;

  if (led_ctx->state == LED_OFF) {
    LED_On();
    led_ctx->state = LED_ON;
    t_next = mu_time_offset(t_prev, MU_TIME_MS_TO_DURATION(LED_ON_TIME_MS));

  } else {
    LED_Off();
    led_ctx->state = LED_OFF;
    t_next = mu_time_offset(t_prev, MU_TIME_MS_TO_DURATION(LED_OFF_TIME_MS));
  }

  mu_sched_task_at(sched, &s_led_task, t_next);

  return NULL;
}
