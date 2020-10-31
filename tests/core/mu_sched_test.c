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

#include "mu_sched.h"
#include "mu_task.h"
#include "mu_spscq.h"
#include "mu_time.h"
#include "mu_test_utils.h"

// =============================================================================
// private types and definitions

#define IRQ_QUEUE_SIZE 2

#define RESCHEDULE_DELTA 42

typedef struct {
  mu_task_t task;
  int call_count;
} counting_task_t;

typedef enum {
  RESCHEDULE_NOW,
  RESCHEDULE_IN,
  DONT_RESCHEDULE
} reschedule_state_t;

typedef struct {
  mu_task_t task;
  reschedule_state_t state;
} reschedule_task_t;

typedef struct {
  mu_task_t task;
} idle_task_t;

// =============================================================================
// private declarations


/**
 * @brief counting_task: When run, increments the task's call count
 */
static counting_task_t *counting_task_init(counting_task_t *counting_task, char *name);
static void *counting_task_fn(void *ctx, void *arg);
static void increment_call_count(counting_task_t *counting_task);
static int get_call_count(counting_task_t *counting_task);
static void reset_call_count(counting_task_t *counting_task);

/**
 * @brief reschedule_task: When run, reschedules (or not) depending on
 * reschedule_task state.
 */
static reschedule_task_t *reschedule_task_init(reschedule_task_t *reschedule_task, char *name);
static void *reschedule_task_fn(void *ctx, void *arg);
static reschedule_state_t get_reschedule_state(reschedule_task_t *task);
static void set_reschedule_state(reschedule_task_t *task, reschedule_state_t state);

static idle_task_t *idle_task_init(idle_task_t *idle_task, char *name);
static void *idle_task_fn(void *ctx, void *arg);

static void setup(void);

// get_time() / set_time() allows us to control the time manually for testing.
static mu_time_t get_time(void);
static void set_time(mu_time_t now);

// =============================================================================
// local storage


static mu_spscq_item_t s_isr_queue_items[IRQ_QUEUE_SIZE];
static mu_spscq_t s_isr_queue;

static mu_time_t s_now;

static counting_task_t s_counting_task1;
static counting_task_t s_counting_task2;
static counting_task_t s_counting_task3;
static counting_task_t s_counting_task4;
static counting_task_t s_counting_task5;
static reschedule_task_t s_reschedule_task1;

// Use a counting task as the idle task.
static idle_task_t s_idle_task;

// =============================================================================
// public code

// mu_sched_test depends upon profiling functions.
#if (!MU_TASK_PROFILING)
#error mu_sched_test requires MU_TASK_PROFILING to be defined
#endif

void mu_sched_test() {
  mu_task_t *task;

  setup();

  mu_sched_reset();
  ASSERT(mu_sched_is_empty() == true);
  ASSERT(mu_sched_get_current_task() == NULL);
  ASSERT(mu_sched_task_count() == 0);
  ASSERT(mu_sched_get_next_task() == NULL);

  setup();

  // mu_sched_step(mu_sched_t *sched)
  set_time(100);  // no task yet...
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 0);
  ASSERT(get_call_count(&s_counting_task2) == 0);
  ASSERT(get_call_count(&s_counting_task3) == 0);
  ASSERT(get_call_count(&s_counting_task4) == 0);
  ASSERT(get_call_count(&s_counting_task5) == 0);
  ASSERT(mu_sched_is_empty() == false);
  ASSERT(mu_sched_get_current_task() == NULL);
  ASSERT(mu_sched_task_count() == 5);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_time(task) == 101);
  ASSERT(mu_task_get_context(task) == &s_counting_task1);

  // mu_sched_step(mu_sched_t *sched)
  set_time(101);  // counting_task1 arrives...
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 1);
  ASSERT(get_call_count(&s_counting_task2) == 0);
  ASSERT(get_call_count(&s_counting_task3) == 0);
  ASSERT(get_call_count(&s_counting_task4) == 0);
  ASSERT(get_call_count(&s_counting_task5) == 0);
  ASSERT(mu_sched_is_empty() == false);
  ASSERT(mu_sched_get_current_task() == NULL);
  ASSERT(mu_sched_task_count() == 4);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_time(task) == 102);
  ASSERT(mu_task_get_context(task) == &s_counting_task2);

  // mu_sched_step(mu_sched_t *sched)
  set_time(102);  // counting_task2 arrives...
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 1);
  ASSERT(get_call_count(&s_counting_task2) == 1);
  ASSERT(get_call_count(&s_counting_task3) == 0);
  ASSERT(get_call_count(&s_counting_task4) == 0);
  ASSERT(get_call_count(&s_counting_task5) == 0);
  ASSERT(mu_sched_is_empty() == false);
  ASSERT(mu_sched_get_current_task() == NULL);
  ASSERT(mu_sched_task_count() == 3);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_time(task) == 103);
  ASSERT(mu_task_get_context(task) == &s_counting_task3);

  // mu_sched_step(mu_sched_t *sched)
  set_time(103);  // counting_task3 arrives...
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 1);
  ASSERT(get_call_count(&s_counting_task2) == 1);
  ASSERT(get_call_count(&s_counting_task3) == 1);
  ASSERT(get_call_count(&s_counting_task4) == 0);
  ASSERT(get_call_count(&s_counting_task5) == 0);
  ASSERT(mu_sched_is_empty() == false);
  ASSERT(mu_sched_get_current_task() == NULL);
  ASSERT(mu_sched_task_count() == 2);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_time(task) == 104);
  ASSERT(mu_task_get_context(task) == &s_counting_task4);

  // mu_sched_step(mu_sched_t *sched)
  set_time(104);  // counting_task4 arrives...
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 1);
  ASSERT(get_call_count(&s_counting_task2) == 1);
  ASSERT(get_call_count(&s_counting_task3) == 1);
  ASSERT(get_call_count(&s_counting_task4) == 1);
  ASSERT(get_call_count(&s_counting_task5) == 0);
  ASSERT(mu_sched_is_empty() == false);
  ASSERT(mu_sched_get_current_task() == NULL);
  ASSERT(mu_sched_task_count() == 1);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_time(task) == 105);
  ASSERT(mu_task_get_context(task) == &s_counting_task5);

  // mu_sched_step(mu_sched_t *sched)
  set_time(105);  // counting_task5 arrives...
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 1);
  ASSERT(get_call_count(&s_counting_task2) == 1);
  ASSERT(get_call_count(&s_counting_task3) == 1);
  ASSERT(get_call_count(&s_counting_task4) == 1);
  ASSERT(get_call_count(&s_counting_task5) == 1);
  ASSERT(mu_sched_is_empty() == true);
  ASSERT(mu_sched_get_current_task() == NULL);
  ASSERT(mu_sched_task_count() == 0);
  task = mu_sched_get_next_task();
  ASSERT(task == NULL);


  setup();

  // remove task not in schedule
  ASSERT(mu_sched_remove_task(&s_reschedule_task1.task) == NULL);

  setup();

  // remove first
  ASSERT(mu_sched_remove_task(&s_counting_task1.task) == &s_counting_task1.task);
  ASSERT(mu_sched_task_count() == 4);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_counting_task2);

  setup();

  // remove not-first
  ASSERT(mu_sched_remove_task(&s_counting_task2.task) == &s_counting_task2.task);
  ASSERT(mu_sched_task_count() == 4);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_counting_task1);

  setup();

  // remove last
  ASSERT(mu_sched_remove_task(&s_counting_task5.task) == &s_counting_task5.task);
  ASSERT(mu_sched_task_count() == 4);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_counting_task1);

  setup();

  // reschedule first task to follow last
  ASSERT(mu_sched_task_at(&s_counting_task1.task, 106) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_count() == 5);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_counting_task2);
  ASSERT(mu_task_get_time(task) == 102);

  // reschedule last task to precede last
  ASSERT(mu_sched_task_at(&s_counting_task5.task, 100) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_count() == 5);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_counting_task5);
  ASSERT(mu_task_get_time(task) == 100);

  // mu_sched_err_t mu_sched_task_now(mu_sched_t *sched, mu_task_t *task);
  setup();
  set_time(100);
  ASSERT(mu_sched_task_now(&s_counting_task2.task) == MU_SCHED_ERR_NONE);
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_counting_task2);
  ASSERT(mu_task_get_time(task) == 100);

  // mu_sched_err_t mu_sched_task_in(mu_sched_t *sched, mu_task_t *task, mu_time_t in);
  setup();
  set_time(100);
  ASSERT(mu_sched_task_in(&s_counting_task2.task, 10) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_count() == 5);
  ASSERT(mu_task_get_time(&s_counting_task2.task) == 110);

  // mu_sched_err_t mu_sched_task_from_isr(mu_sched_t *sched, mu_task_t *task);
  setup();

  set_time(100);
  ASSERT(mu_spscq_count(mu_sched_isr_queue()) == 0);
  ASSERT(mu_sched_task_from_isr(&s_counting_task2.task) == MU_SCHED_ERR_NONE);
  ASSERT(mu_spscq_count(mu_sched_isr_queue()) == 1);
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  // verify that calling step():
  // - sets the task's time to the scheduler's current time
  // - called the task
  // - removed it from the isr queue
  ASSERT(get_call_count(&s_counting_task2) == 1);
  ASSERT(mu_spscq_count(mu_sched_isr_queue()) == 0);
  ASSERT(mu_sched_task_count() == 4);

  // scheduling tasks at same time: tasks are processed in FIFO order
  setup();
  mu_sched_reset();

  ASSERT(mu_sched_task_at(&s_counting_task3.task, 110) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_at(&s_counting_task2.task, 110) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_at(&s_counting_task1.task, 110) == MU_SCHED_ERR_NONE);

  set_time(110);

  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 0);
  ASSERT(get_call_count(&s_counting_task2) == 0);
  ASSERT(get_call_count(&s_counting_task3) == 1);

  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 0);
  ASSERT(get_call_count(&s_counting_task2) == 1);
  ASSERT(get_call_count(&s_counting_task3) == 1);

  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(get_call_count(&s_counting_task1) == 1);
  ASSERT(get_call_count(&s_counting_task2) == 1);
  ASSERT(get_call_count(&s_counting_task3) == 1);

  // mu_sched_err_t mu_sched_reschedule_in(mu_sched_t *sched, mu_time_dt in);
  setup();
  mu_sched_reset();

  set_time(100);
  ASSERT(mu_sched_task_at(&s_reschedule_task1.task, 100) == MU_SCHED_ERR_NONE);
  set_reschedule_state(&s_reschedule_task1, RESCHEDULE_NOW);
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  // s_reschedule_task1 should have been rescheduled "now"
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_reschedule_task1);
  ASSERT(mu_task_get_time(task) == 100);

  set_reschedule_state(&s_reschedule_task1, RESCHEDULE_IN);
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  // s_reschedule_task1 should have been rescheduled 10 ticks from now
  task = mu_sched_get_next_task();
  ASSERT(task != NULL);
  ASSERT(mu_task_get_context(task) == &s_reschedule_task1);
  ASSERT(mu_task_get_time(task) == 110);

  set_reschedule_state(&s_reschedule_task1, DONT_RESCHEDULE);
  set_time(110);
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  // s_reschedule_task1 should not have been rescheduled
  task = mu_sched_get_next_task();
  ASSERT(task == NULL);

  // mu_sched_task_status_t mu_sched_get_task_status(mu_sched_t *sched, mu_task_t *task);
  setup();
  set_time(100);
  ASSERT(mu_sched_get_task_status(&s_counting_task1.task) == MU_SCHED_TASK_STATUS_SCHEDULED);
  set_time(101);
  ASSERT(mu_sched_get_task_status(&s_counting_task1.task) == MU_SCHED_TASK_STATUS_RUNNABLE);
  ASSERT(mu_sched_step() == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_get_task_status(&s_counting_task1.task) == MU_SCHED_TASK_STATUS_IDLE);

}

// =============================================================================
// private code

// ============
// Counting Task

static counting_task_t *counting_task_init(counting_task_t *counting_task, char *name) {
  mu_task_init(&counting_task->task, counting_task_fn, counting_task, name);
  reset_call_count(counting_task);
  return counting_task;
}

static void *counting_task_fn(void *ctx, void *arg) {
  counting_task_t *counting_task = (counting_task_t *)ctx;

  increment_call_count(counting_task);
  ASSERT(&counting_task->task == mu_sched_get_current_task());
  ASSERT(mu_sched_get_task_status(&counting_task->task) == MU_SCHED_TASK_STATUS_ACTIVE);
  return NULL;
}

static void increment_call_count(counting_task_t *counting_task) {
  counting_task->call_count++;
}

static int get_call_count(counting_task_t *counting_task) {
  return counting_task->call_count;
}

static void reset_call_count(counting_task_t *counting_task) {
  counting_task->call_count = 0;
}

// ============
// Reschedule Task

static reschedule_task_t *reschedule_task_init(reschedule_task_t *reschedule_task, char *name) {
  mu_task_init(&reschedule_task->task, reschedule_task_fn, reschedule_task, name);
  reschedule_task->state = RESCHEDULE_NOW;
  return reschedule_task;
}

static void *reschedule_task_fn(void *ctx, void *arg) {
  reschedule_task_t *task = (reschedule_task_t *)ctx;

  switch(get_reschedule_state(task)) {
    case RESCHEDULE_NOW:
    mu_sched_reschedule_now();
    break;
    case RESCHEDULE_IN:
    mu_sched_reschedule_in(10);
    break;
    case DONT_RESCHEDULE:
    break;
  }
  return NULL;
}

static reschedule_state_t get_reschedule_state(reschedule_task_t *task) {
  return task->state;
}

static void set_reschedule_state(reschedule_task_t *task, reschedule_state_t state) {
  task->state = state;
}

static idle_task_t *idle_task_init(idle_task_t *idle_task, char *name) {
  mu_task_init(&idle_task->task, idle_task_fn, idle_task, name);
  return idle_task;
}

static void *idle_task_fn(void *ctx, void *arg) {
  return NULL;
}

// ============
// General setup

static void setup(void) {
  // set up scheduler
  mu_spscq_init(&s_isr_queue, s_isr_queue_items, IRQ_QUEUE_SIZE);
  mu_sched_init(&s_isr_queue);

  // set up clock for unit testing
  set_time(0);
  mu_sched_set_clock_source(get_time);

  idle_task_init(&s_idle_task, "Idle Task");
  mu_sched_set_idle_task(&s_idle_task.task);

  // initialize the tasks
  counting_task_init(&s_counting_task1, "Counting Task 1");
  counting_task_init(&s_counting_task2, "Counting Task 2");
  counting_task_init(&s_counting_task3, "Counting Task 3");
  counting_task_init(&s_counting_task4, "Counting Task 4");
  counting_task_init(&s_counting_task5, "Counting Task 5");
  reschedule_task_init(&s_reschedule_task1, "Reschedule Task 1");

  // schedle tasks, delibrately not in time order
  ASSERT(mu_sched_task_at(&s_counting_task3.task, 103) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_at(&s_counting_task1.task, 101) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_at(&s_counting_task2.task, 102) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_at(&s_counting_task5.task, 105) == MU_SCHED_ERR_NONE);
  ASSERT(mu_sched_task_at(&s_counting_task4.task, 104) == MU_SCHED_ERR_NONE);
}

static mu_time_t get_time(void) {
  return s_now;
}

static void set_time(mu_time_t now) {
  s_now = now;
}
