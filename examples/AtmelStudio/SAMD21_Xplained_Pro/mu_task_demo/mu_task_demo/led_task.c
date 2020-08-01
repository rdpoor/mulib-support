/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

#include "atmel_start.h"
#include "led_task.h"
#include "mulib.h"
#include "mu_vm.h"
#include <stddef.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *led_task_fn(void *self, void *arg);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_task_t *led_task_init(mu_task_t *led_task, led_ctx_t *led_ctx) {
  led_ctx->state = LED_OFF;
  mu_task_init(led_task, led_task_fn, led_ctx, "LED Task");
  return led_task;
}

// =============================================================================
// local (static) code

static void *led_task_fn(void *ctx, void *arg) {
  // "context" is passed as the first argument, scheduler is second
  led_ctx_t *led_ctx = (led_ctx_t *)ctx;
  mu_sched_t *sched = (mu_sched_t *)arg;

  if (led_ctx->state == LED_OFF) {
    mu_vm_led_set(true);
    led_ctx->state = LED_ON;
    mu_sched_reschedule_in(sched, mu_time_ms_to_duration(LED_ON_TIME_MS));

  } else {
    mu_vm_led_set(false);
    led_ctx->state = LED_OFF;
    mu_sched_reschedule_in(sched, mu_time_ms_to_duration(LED_OFF_TIME_MS));
  }

  return NULL;
}
