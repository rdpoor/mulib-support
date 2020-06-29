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

#include "blinky_sleepy.h"
#include "definitions.h"
#include "mu_port.h"
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

// don't sleep for less than 1ms (32 RTC tics)
#define MIN_SLEEP_DURATION MU_TIME_MS_TO_DURATION(1)

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

static void idle_task_init(void);
static void *idle_task_fn(void *self, void *arg);
static void sleep_until(mu_time_t time);
static void sleep_until_interrupt(void);
static bool is_ready_to_sleep(void);
static void will_sleep(void);
static void go_to_sleep(void);
static void did_wake(void);
static void rtc_callback(RTC_TIMER32_INT_MASK intCause, uintptr_t context);

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

// the Idle task.
static mu_task_t s_idle_task;

// =============================================================================
// public code

void blinky_init() {
  printf("\n\n##############\n");
  printf("# blinky_sleepy demo %s\n", BINKY_VERSION);

  // initialize the port-specific interface
  mu_port_init();

  // set up the isr queue and the scheduler
  mu_spscq_init(&s_isr_queue, s_isr_queue_items, ISR_Q_CAPACITY);
  mu_sched_init(&s_sched, s_event_queue, EVENT_QUEUE_CAPACITY, &s_isr_queue);

  // initialize the LED context and task
  led_task_init(&s_led_ctx);
  mu_task_init(&s_led_task, led_task_fn, &s_led_ctx, "LED");

  // initialize the button task.  No context required.
  button_task_init(&s_sched);
  mu_task_init(&s_button_task, button_task_fn, NULL, "Button Interrupt");

  // initialize our custom idle task, register it with the scheduler
  idle_task_init();
  mu_task_init(&s_idle_task, idle_task_fn, NULL, "Sleeping Idle");
  mu_sched_set_idle_task(&s_sched, &s_idle_task);

  // schedule the initial call to the LED task
  mu_sched_task_now(&s_sched, &s_led_task);
}

void blinky_step() {
  // called repeatedly from main(): run the scheduler
  mu_sched_step(&s_sched);
}

// =============================================================================
// local (static) code

// ==========
// LED task

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

// ==========
// Button task

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

// ==========
// Idle task

static void idle_task_init(void) {
  // put ethernet interface into low-power state
  ETH_RESET_Clear();
  // use the low power regulator
  SUPC_SelectVoltageRegulator(SUPC_VREGSEL_BUCK);
  // regoster RTC callback and enable interrupts
  RTC_Timer32CallbackRegister(rtc_callback, (uintptr_t)NULL);
  RTC_Timer32InterruptEnable(RTC_TIMER32_INT_MASK_CMP0);
}

static void *idle_task_fn(void *ctx, void *arg) {
  // ctx is unused in idle task
  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_event_t *next_event = mu_sched_get_next_event(sched);

  if (is_ready_to_sleep()) {
    if (next_event) {
      // sleep until the next event's time arrives
      sleep_until(mu_event_get_time(next_event));
    } else {
      // no future events scheduled -- only and interrupt will wake us
      sleep_until_interrupt();
    }
  }
  return NULL;
}

void sleep_until(mu_time_t time) {
  mu_time_t sleep_duration = mu_time_difference(time, mu_time_now());
  will_sleep();
  if (sleep_duration >= MIN_SLEEP_DURATION) {
    RTC_Timer32Compare0Set(time);
    go_to_sleep();
  }
  did_wake();
}

void sleep_until_interrupt(void) {
  will_sleep();
  go_to_sleep();
  did_wake();
}

static bool is_ready_to_sleep(void) {
  return true;
}

static void will_sleep(void) {
  asm("nop");
}

static void go_to_sleep(void) {
  // There appears to be a bug in the E54 silicon that causes the DFLL to lose
  // the state of the .FINE bits across STANDBY / wake events.  As a workaround,
  // we save the DFLL state prior to STANDBY and restore it upon waking.
  uint32_t saved_dfllval = OSCCTRL_REGS->OSCCTRL_DFLLVAL;
  PM_StandbyModeEnter();
  OSCCTRL_REGS->OSCCTRL_DFLLVAL = saved_dfllval;
}

static void did_wake(void) {
  asm("nop");
}

static void rtc_callback(RTC_TIMER32_INT_MASK intCause, uintptr_t context) {
  // arrive here on an RTC interrupt, in this case, when the there is a compare
  // match.  No other action is required; we use the interrupt only to wake the
  // processor from Hibernate mode.  The system code clears the interrupt state.
  asm("nop");
}
