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

#include "mulib.h"
#include "mu_test_utils.h"

// =============================================================================
// private types and definitions

#define IRQ_QUEUE_SIZE 2

// =============================================================================
// private declarations

static void setup(void);

// get_now() / set_now() allows us to control the time manually for testing.
static mu_time_t get_now(void);
static void set_now(mu_time_t now);

// generic function to call from timer
static void task_fn(void *ctx, void *arg);

// =============================================================================
// local storage

static mu_time_t s_now;

static mu_spscq_item_t s_isr_queue_store[IRQ_QUEUE_SIZE];

static mu_task_t s_task;
static int s_task_call_count;

static mu_timer_t s_timer;

// =============================================================================
// public code

void mu_timer_test() {
  setup();

  ASSERT(mu_timer_init(&s_timer, &s_task) == &s_timer);
  ASSERT(mu_timer_is_running(&s_timer) == false);

  // start (from stopped mode) with repeat = false
  ASSERT(mu_timer_start(&s_timer, 10, false) == &s_timer);
  ASSERT(mu_timer_is_running(&s_timer) == true);

  // not triggered yet...
  set_now(5);
  mu_sched_step();
  ASSERT(s_task_call_count == 0);
  ASSERT(mu_timer_is_running(&s_timer) == true);

  // trigger time has arrived
  set_now(10);
  mu_sched_step();
  ASSERT(s_task_call_count == 1);
  ASSERT(mu_timer_is_running(&s_timer) == false);

  // trigger in 10 ticks from now, this time with repeat = true
  ASSERT(mu_timer_start(&s_timer, 10, true) == &s_timer);
  ASSERT(mu_timer_is_running(&s_timer) == true);

  // not triggered yet...
  set_now(15);
  mu_sched_step();
  ASSERT(s_task_call_count == 1);
  ASSERT(mu_timer_is_running(&s_timer) == true);

  // trigger time has arrived
  set_now(20);
  mu_sched_step();
  ASSERT(s_task_call_count == 2);
  ASSERT(mu_timer_is_running(&s_timer) == true);  // b/c repeat = true

  // not triggered yet...
  set_now(25);
  mu_sched_step();
  ASSERT(s_task_call_count == 2);
  ASSERT(mu_timer_is_running(&s_timer) == true);

  // trigger time has arrived
  set_now(30);
  mu_sched_step();
  ASSERT(s_task_call_count == 3);
  ASSERT(mu_timer_is_running(&s_timer) == true);  // b/c repeat = true

  // stop timer
  ASSERT(mu_timer_stop(&s_timer) == &s_timer);
  ASSERT(mu_timer_is_running(&s_timer) == false);  // now stopped

  // make sure it doesn't trigger...
  set_now(40);
  mu_sched_step();
  ASSERT(s_task_call_count == 3);

  // restarting timer...
  ASSERT(mu_timer_start(&s_timer, 10, true) == &s_timer);
  ASSERT(mu_timer_is_running(&s_timer) == true);

  set_now(45);
  mu_sched_step();
  ASSERT(s_task_call_count == 3);

  // restart at t = 45
  ASSERT(mu_timer_start(&s_timer, 10, false) == &s_timer);
  ASSERT(mu_timer_is_running(&s_timer) == true);

  // doesn't trigger at t=50
  set_now(50);
  mu_sched_step();
  ASSERT(s_task_call_count == 3);

  // does trigger at t = 55
  set_now(55);
  mu_sched_step();
  ASSERT(s_task_call_count == 4);
  ASSERT(mu_timer_is_running(&s_timer) == false);  // b/c repeat = false
}


// =============================================================================
// private code

static void setup(void) {
  set_now(0);
  mu_sched_init(s_isr_queue_store, IRQ_QUEUE_SIZE);
  mu_sched_set_clock_source(get_now);
  mu_task_init(&s_task, task_fn, &s_task, "Timed Task");
  s_task_call_count = 0;
}

static mu_time_t get_now(void) {
  return s_now;
}

static void set_now(mu_time_t now) {
  s_now = now;
}

static void task_fn(void *ctx, void *arg) {
  ASSERT(ctx == &s_task);
  s_task_call_count += 1;
}
