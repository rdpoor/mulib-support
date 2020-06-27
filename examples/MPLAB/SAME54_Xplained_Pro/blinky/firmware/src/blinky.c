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

#include "blinky.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_spscq.h"
#include "mu_event.h"
#include "mu_time.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

#define BINKY_VERSION "1.0.0"

#define EVENT_QUEUE_CAPACITY 10
#define ISR_Q_CAPACITY 8

#define LED_ON_TIME_MS 25
#define LED_OFF_TIME_MS 975

typedef enum { LED_OFF, LED_ON } led_state_t;

typedef struct {
  led_state_t state;
} led_ctx_t;

// =============================================================================
// local (forward) declarations

static void led_task_init(led_ctx_t *led_ctx);
static void *led_task_fn(void *self, void *arg);

static void button_task_init(mu_sched_t *sched);
static void *button_task_fn(void *self, void *arg);
static void button_cb(uintptr_t context);

// =============================================================================
// local storage

// the scheduler state
static mu_sched_t s_sched;

// the backing store for scheduled events
static mu_event_t s_event_queue[EVENT_QUEUE_CAPACITY];

// the queue to hold tasks queued from interrupt level, and its backing store
static mu_spscq_t s_isr_queue;
static mu_spscq_item_t s_isr_queue_items[ISR_Q_CAPACITY];

// the LED task
static mu_task_t s_led_task;
static led_ctx_t s_led_ctx;

// the Button task
static mu_task_t s_button_task;
// no context required...

// =============================================================================
// public code

void blinky_init() {
  printf("\n\n##############\n");
  printf("# blinky demo %s\n", BINKY_VERSION);

  // initialize the real-time clock
  mu_time_init();

  // set up the isr queue and the scheduler
  mu_spscq_init(&s_isr_queue, s_isr_queue_items, ISR_Q_CAPACITY);
  mu_sched_init(&s_sched, s_event_queue, EVENT_QUEUE_CAPACITY, &s_isr_queue);

  // initialize the LED context and task
  led_task_init(&s_led_ctx);
  mu_task_init(&s_led_task, led_task_fn, &s_led_ctx, "LED");

  // initialize the button task.  No context required.
  button_task_init(&s_sched);
  mu_task_init(&s_button_task, button_task_fn, NULL, "Button Interrupt");
  
  // schedule the initial call to the LED task
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
}

static void *led_task_fn(void *ctx, void *arg) {
  // "context" is passed as the first argument
  led_ctx_t *led_ctx = (led_ctx_t *)ctx;

  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;

  // Note that the current event time is the time at which the LED task was
  // scheduled to run -- the actual start time might be later due to latency or
  // other tasks.  To avoid drift, we increment the current event time by the
  // desired amount and use that as the next scheduled time.
  mu_event_t *event = mu_sched_get_current_event(sched);
  mu_time_t prev_time = mu_event_get_time(event);
  mu_time_t next_time;

  if (led_ctx->state == LED_OFF) {
    LED_On();
    led_ctx->state = LED_ON;
    next_time = mu_time_offset(prev_time, MU_TIME_MS_TO_DURATION(LED_ON_TIME_MS));

  } else {
    LED_Off();
    led_ctx->state = LED_OFF;
    next_time = mu_time_offset(prev_time, MU_TIME_MS_TO_DURATION(LED_OFF_TIME_MS));
  }

  mu_sched_task_at(sched, &s_led_task, next_time);

  return NULL;
}

static void button_task_init(mu_sched_t *sched) {
    // Register the button_cb function to be called upon an EIC interrupt on 
    // the user button.  Initialize with the scheduler object so the button_cb
    // can access the mu_sched.
  EIC_CallbackRegister(EIC_PIN_15 , button_cb, (uintptr_t)sched);
  EIC_InterruptEnable(EIC_PIN_15);
}

static void *button_task_fn(void *ctx, void *arg) {
  // "context" is unused in button task
  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_time_t now = mu_sched_get_current_time(sched);

  printf("button pressed at %lu\r\n", now);
  return NULL;
}

// button_cb is triggered when a button press generates an interrupt.
// From interrupt level, schedule the button task upon leaving interrupt level.
static void button_cb(uintptr_t context) {
  mu_sched_t *sched = (mu_sched_t *)context;
  mu_sched_task_from_isr(sched, &s_button_task);
}
