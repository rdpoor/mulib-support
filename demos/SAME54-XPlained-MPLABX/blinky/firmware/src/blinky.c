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

#include "blinky.h"
#include "definitions.h"
#include "mu_pstore.h"
#include "mu_sched.h"
#include "mu_time.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

#define BINKY_VERSION "1.0.0"

#define TASK_Q_CAPACITY 10
#define ISR_Q_CAPACITY 8

#define LED_ON_TIME_MS 100
#define LED_OFF_TIME_MS 900

typedef enum { LED_OFF, LED_ON } led_state_t;

typedef struct {
  led_state_t state;
  mu_time_dt on_duration;
  mu_time_dt off_duration;
} led_ctx_t;

// =============================================================================
// local (forward) declarations

static void led_task_init(led_ctx_t *led_ctx);
static void *led_task_fn(void *self, void *arg);

// =============================================================================
// local storage

// the scheduler state
static mu_sched_t s_sched;

// the queue to hold scheduled tasks, and its backing store
static mu_pstore_t s_task_queue;
static mu_pstore_item_t s_task_queue_items[TASK_Q_CAPACITY];

// the queue to hold tasks queued from interrupt level, and its backing store
static mu_spscq_t s_isr_queue;
static mu_spscq_item_t s_isr_queue_items[ISR_Q_CAPACITY];

// the LED task
static mu_task_t s_led_task;
static led_ctx_t s_led_ctx;

// =============================================================================
// public code

void blinky_init() {
  printf("binky %s\n", BINKY_VERSION);

  // initialize the real-time clock
  mu_time_init();

  // set up the task queue, the isr queue and the scheduler
  mu_pstore_init(&s_task_queue, s_task_queue_items, TASK_Q_CAPACITY);
  mu_spscq_init(&s_isr_queue, s_isr_queue_items, ISR_Q_CAPACITY);
  mu_sched_init(&s_sched, &s_task_queue, &s_isr_queue);

  // initialize the LED context and task
  led_task_init(&s_led_ctx);
  mu_task_init(&s_led_task, led_task_fn, &s_led_ctx, "LED");

  // schedule the inital call to the LED task
  mu_sched_task_now(&s_sched, &s_led_task);
}

void blinky_step() {
  // called repeatedly from main(): run the scheduler
  mu_sched_step(&s_sched);
}

// =============================================================================
// local (static) code

static void led_task_init(led_ctx_t *led_ctx) {
  led_ctx->state = LED_OFF;
  led_ctx->on_duration = mu_time_ms_to_duration(LED_ON_TIME_MS);
  led_ctx->off_duration = mu_time_ms_to_duration(LED_OFF_TIME_MS);
}

static void *led_task_fn(void *self, void *arg) {
  led_ctx_t *ctx = (led_ctx_t *)self;
  mu_sched_t *sched = (mu_sched_t *)arg;

  printf("%08ld LED ", mu_sched_get_current_time(sched));
  if (ctx->state == LED_OFF) {
    LED_On();
    printf("on\n");
    ctx->state = LED_ON;
    mu_sched_task_again(sched, &s_led_task, ctx->on_duration);

  } else {
    LED_Off();
    printf("off\n");
    ctx->state = LED_OFF;
    mu_sched_task_again(sched, &s_led_task, ctx->off_duration);
  }

  return NULL;
}
