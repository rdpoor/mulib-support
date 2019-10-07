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

#include "../src/mu_sched.h"
#include "test_utilities.h"

static port_time_t s_time = 0;
static int s_fn1_call_count;
static int s_fn2_call_count;
static int s_fn3_call_count;

static void test_reset() {
  s_time = 0;
  s_fn1_call_count = 0;
  s_fn2_call_count = 0;
  s_fn3_call_count = 0;
}

/**
 * Use a simulated clock for the scheduler instead of `port_time_now()`.
 * At each call to the idle task, time increments by one unit.
 */
static port_time_t now_fn() {
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
  e->time = port_time_offset(e->time, (port_time_dt)3);
  UTEST_ASSERTEQ_INT(mu_sched_add(s, e), MU_SCHED_ERR_NONE);
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

// =============================================================================
// main code.

void mu_sched_test() {
  // We can allocate all of these on the stack only because we know they only
  // exist during the call to mu_sched_test().  Normally some of theses would be
  // statically allocated.
  mu_sched_t sched;
  mu_sched_t *s = &sched;
  mu_task_t idle_task = {.fn = idle_task_fn, .self = NULL};
  mu_evt_t evt1, evt2, evt3;

  mu_sched_init(s);
  mu_sched_set_clock_source(s, now_fn); // use simulated clock
  mu_sched_set_idle_task(s, &idle_task);

  test_reset();

  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), true);

  // with no runnable task, sched_step() runs the idle task once
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 1));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // schedule fn1 for time=2.  pass the evt1 object as *self
  UTEST_ASSERTEQ_INT(
    mu_sched_add(s, mu_evt_init_at(&evt1, (port_time_t)2, fn1, &evt1)),
    MU_SCHED_ERR_NONE);

  // schedule fn2 for time=10.  This is not recommended practice, but for
  // testing, we pass _evt1_ as the "self" argument for the event. We will
  // use that argument in fn2 to remove evt1 from the schedule.  See fn2().
  UTEST_ASSERTEQ_INT(
    mu_sched_add(s, mu_evt_init_at(&evt2, (port_time_t)10, fn2, &evt1)),
    MU_SCHED_ERR_NONE);

  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), false);

  // step again: fn1 doesn't trigger yet
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 2));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // ... but now it does.  assure that fn1 ran instead of the idle task
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 2));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // step three more times to make fn1 trigger again...
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 3));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 4));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 5));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // fn1 should trigger again instead of idle task
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 5));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 6));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 0);

  // schedule fn3 as an immediate task.  pass evt3 as *self
  UTEST_ASSERTEQ_INT(
    mu_sched_add(s, mu_evt_init_immed(&evt3, fn3, &evt3)),
    MU_SCHED_ERR_NONE);

  // now there are three events in the scheduler
  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), false);

  // assure that fn3 ran rather than the idle task
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 6));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // stepping, stepping...
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 7));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 8));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 2);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 8));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);      // boom!
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 9));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 10));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 0);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // now evt2 will trigger and remove evt1 from the queue
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 10));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 1);      // fn2 fired (and removes evt1)
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // now there are no events in the scheduler
  UTEST_ASSERTEQ_BOOL(mu_sched_is_empty(s), true);

  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 11));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);

  // Normally evt1 would have triggered here.  But since it was
  // removed, the idle task runs instead.
  UTEST_ASSERTEQ_INT(mu_sched_step(s), MU_SCHED_ERR_NONE);
  UTEST_ASSERT(port_time_is_equal(s_time, 12));
  UTEST_ASSERTEQ_INT(s_fn1_call_count, 3);
  UTEST_ASSERTEQ_INT(s_fn2_call_count, 1);
  UTEST_ASSERTEQ_INT(s_fn3_call_count, 1);


}
