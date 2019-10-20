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

#include "mu_sched.h"
#include "mu_ring.h"
#include "mu_time.h"
#include "test_utilities.h"

// =============================================================================
// private types and definitions

#define ISR_QUEUE_POOL_SIZE 4

// =============================================================================
// private declarations

/**
 * Reset static counters.
 */
static void test_reset();

/**
 * Use a simulated clock for the scheduler instead of `mu_time_now()`.
 * At each call to the idle task, time increments by one unit.
 */
static mu_time_t now_fn();

/**
 * Called whenever the scheduler has nothing else to do.  In our test
 * implementation, it simply increments the simulated clock time.
 */
static void idle_task_fn(void *self, void *arg);

static void fn1(void *self, void *arg);
static void fn2(void *self, void *arg);
static void fn3(void *self, void *arg);
static void iisr_fn(void *self, void *arg); // immediate ISR function
static void disr_fn(void *self, void *arg); // deferred ISR function

static void queue_immed_from_interrupt(int s);
static void queue_deferred_from_interrupt(int s);

// =============================================================================
// local storage

static mu_sched_t s_sched;
static mu_ring_obj_t s_isr_queue_pool[ISR_QUEUE_POOL_SIZE];
static mu_time_t s_time = 0;
static int s_fn1_call_count;
static int s_fn2_call_count;
static int s_fn3_call_count;
static int s_iisr_call_count;
static int s_disr_call_count;

static mu_evt_t s_immed_event;
static mu_evt_t s_deferred_event;

// =============================================================================
// public code

void mu_sched_test() {
  // We can allocate all of these on the stack only because we know they only
  // exist during the call to mu_sched_test().  Normally some of theses would be
  // statically allocated.
  mu_sched_t *s = &s_sched;
  mu_task_t idle_task = {.fn = idle_task_fn, .self = NULL};
  mu_evt_t evt1, evt2, evt3;

  mu_sched_init(s, s_isr_queue_pool, ISR_QUEUE_POOL_SIZE);
  mu_sched_set_clock_source(s, now_fn); // use simulated clock
  mu_sched_set_idle_task(s, &idle_task);

  test_reset();

  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), true);

  // with no runnable task, sched_step() runs the idle task once
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 1));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // schedule fn1 for time=2.  pass the evt1 object as *self
  UTEST_ASSERTEQ_INT(
    mu_sched_queue(s, mu_evt_init_at(&evt1, (mu_time_t)2, fn1, &evt1, "E1")),
    MU_SCHED_ERR_NONE);

  // schedule fn2 for time=10.  This is not recommended practice, but for
  // testing, we pass _evt1_ as the "self" argument for the event. We will
  // use that argument in fn2 to remove evt1 from the schedule.  See fn2().
  UTEST_ASSERTEQ_INT(
    mu_sched_queue(s, mu_evt_init_at(&evt2, (mu_time_t)10, fn2, &evt1, "E2")),
    MU_SCHED_ERR_NONE);

  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), false);

  // step again: fn1 doesn't trigger yet
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 2));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // ... but now it does.  assure that fn1 ran instead of the idle task
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 2));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // step three more times to make fn1 trigger again...
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 3));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 4));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 5));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // fn1 should trigger again instead of idle task
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 5));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 6));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // schedule fn3 as an immediate task.  pass evt3 as *self
  UTEST_ASSERTEQ_INT(
    mu_sched_queue(s, mu_evt_init_immed(&evt3, fn3, &evt3, "E3")),
    MU_SCHED_ERR_NONE);

  // now there are three events in the scheduler
  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), false);

  // assure that fn3 ran rather than the idle task
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 6));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // stepping, stepping...
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 7));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 8));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 8));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);      // boom!
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 9));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 10));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // now evt2 will trigger and remove evt1 from the queue
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 10));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 1);      // fn2 fired (and removes evt1)
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // now there are no events in the scheduler
  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), true);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 11));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // Normally evt1 would have triggered here.  But since it was
  // removed, the idle task runs instead.
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 12));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // Assure ISR queued events are properly managed
  queue_deferred_from_interrupt(0);  // fires at now + 2
  queue_immed_from_interrupt(0);     // fires immediately

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 12)); // idle task has not run
  UTEST_ASSERTEQ_INT(s_iisr_call_count, 1);
  UTEST_ASSERTEQ_INT(s_disr_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 13)); // idle task ran
  UTEST_ASSERTEQ_INT(s_iisr_call_count, 1);
  UTEST_ASSERTEQ_INT(s_disr_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 14)); // idle task ran
  UTEST_ASSERTEQ_INT(s_iisr_call_count, 1);
  UTEST_ASSERTEQ_INT(s_disr_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 14)); // idle task has not run
  UTEST_ASSERTEQ_INT(s_iisr_call_count, 1);
  UTEST_ASSERTEQ_INT(s_disr_call_count, 1);     // dirq ran instead...

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(mu_time_is_equal(s_time, 15)); // idle task ran
  UTEST_ASSERTEQ_INT(s_iisr_call_count, 1);
  UTEST_ASSERTEQ_INT(s_disr_call_count, 1);

  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), true);
}

// =============================================================================
// private code

static void test_reset() {
  s_time = 0;
  s_fn1_call_count = 0;
  s_fn2_call_count = 0;
  s_fn3_call_count = 0;
  s_iisr_call_count = 0;
  s_disr_call_count = 0;
}

/**
 * Use a simulated clock for the scheduler instead of `mu_time_now()`.
 * At each call to the idle task, time increments by one unit.
 */
static mu_time_t now_fn() {
  return s_time;
}

/**
 * Called whenever the scheduler has nothing else to do.  In our test
 * implementation, it simply increments the simulated clock time.
 */
static void idle_task_fn(void *self, void *arg) {
  // mu_sched_t *s = (mu_sched_t *)arg; // sched is passed as *arg (but unused)
  s_time += 1;                       // increment simulated time
}

static void fn1(void *self, void *arg) {
  mu_evt_t *e = (mu_evt_t *)self;    // event is passed as *self
  mu_sched_t *s = (mu_sched_t *)arg; // sched is passed as *arg
  s_fn1_call_count += 1;

  // assure that mu_sched_current_event is properly set up.
  UTEST_ASSERTEQ_PTR(mu_sched_current_event(s), e);

  // reschedule in 3 ticks from now
  e->time = mu_time_offset(e->time, (mu_time_dt)3);
  UTEST_ASSERTEQ_INT(mu_sched_queue(s, e), MU_SCHED_ERR_NONE);
}

static void fn2(void *self, void *arg) {
  mu_evt_t *evt1 = (mu_evt_t *)self;    // NOTE: _evt1_ is passed as *self
  mu_sched_t *s = (mu_sched_t *)arg;    // sched is passed as *arg
  s_fn2_call_count += 1;

  // remove evt1 from the schedule
  UTEST_ASSERTEQ_INT(mu_sched_remove(s, evt1), MU_SCHED_ERR_NONE);
}

static void fn3(void *self, void *arg) {
  // mu_evt_t *e = (mu_evt_t *)self;    // event is passed as *self
  // mu_sched_t *s = (mu_sched_t *)arg; // sched is passed as *arg
  s_fn3_call_count += 1;
}

static void iisr_fn(void *self, void *arg) {
  s_iisr_call_count += 1;
}

static void disr_fn(void *self, void *arg) {
  s_disr_call_count += 1;
}

static void queue_immed_from_interrupt(int s) {
  (void)s;
  mu_evt_init_immed(&s_immed_event, iisr_fn, NULL, "I");
  mu_sched_from_isr(&s_sched, &s_immed_event);
}

static void queue_deferred_from_interrupt(int s) {
  (void)s;
  // set time 2 ticks from now
  mu_time_t t = mu_time_offset(mu_sched_get_time(&s_sched), (mu_time_t)2);
  mu_evt_init_at(&s_deferred_event, t, disr_fn, NULL, "D");
  mu_sched_from_isr(&s_sched, &s_deferred_event);
}
