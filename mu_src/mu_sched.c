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

#include "mu_assert.h"
#include "mu_sched.h"
#include "mu_queue.h"
#include "mu_task.h"
#include <stdbool.h>
#include <string.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static mu_task_t *remove_runnable_task(mu_sched_t *sched, mu_time_t now);

// Transfer ISR-initiated tasks from the ISR queue to the schedule
static void process_isr_queue(mu_sched_t *sched);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_sched_t *mu_sched_init(mu_sched_t *sched,
                          mu_queue_obj_t *isr_task_pool,
                          unsigned int isr_task_pool_size) {
  mu_queue_err_t err;
  mu_queue_t *q = &(sched->isr_queue);

  (void)err;
  sched->tasks = (mu_task_t *)NULL;     // task queue starts out empty
  err = mu_queue_init(q, isr_task_pool, isr_task_pool_size);
  MU_ASSERT(err == MU_QUEUE_ERR_NONE);
  sched->clock_source = mu_time_now;  // default clock source
  sched->idle_task = NULL;
  return mu_sched_reset(sched);
}

mu_sched_t *mu_sched_reset(mu_sched_t *sched) {
  mu_task_t *task;

  while ((task = sched->tasks) != NULL) {
    sched->tasks = task->next;
    task->next = NULL;
  }

  if (sched->current_task) {
    sched->current_task->next = NULL;
    sched->current_task = NULL;
  }

  mu_queue_reset(&(sched->isr_queue));

  return sched;
}

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn) {
  MU_ASSERT(clock_fn != NULL);
  sched->clock_source = clock_fn;
  return sched;
}

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *idle_task) {
  sched->idle_task = idle_task;
  return sched;
}

bool mu_sched_is_empty(mu_sched_t *sched) {
  return sched->tasks == (mu_task_t *)NULL;
}

bool mu_sched_has_task(mu_sched_t *sched, mu_task_t *task) {
	mu_task_t *tasks = sched->tasks;
	while(tasks) {
		if (tasks == task) return true;
		tasks = tasks->next;
	}
	return false;
}

unsigned int mu_sched_task_count(mu_sched_t *sched) {
	unsigned int count = 0;
	mu_task_t *tasks = mu_sched_get_tasks(sched);
	while(tasks) {
		count += 1;
    tasks = tasks->next;
	}
	return count;
}

mu_task_t *mu_sched_get_tasks(mu_sched_t *sched) {
	return sched->tasks;
}

mu_sched_err_t mu_sched_step(mu_sched_t *sched) {
  mu_task_t *runnable;
  mu_time_t now = mu_sched_get_time(sched);

  // add any tasks that have arrived on the ISR queue
  process_isr_queue(sched);

  if ((runnable = remove_runnable_task(sched, now)) != NULL) {
    // We've found an task that's runnable and have removed it from the
    // task queue.  Set current task and call the task.
    sched->current_task = runnable;
    mu_task_call(runnable, sched);
    sched->current_task = NULL;

  } else {
    // Nothing ready to run.  Call the idle task.  NOTE: mu_task_call() handles
    // the case that sched->idle_task is null, so we don't need to check.
    mu_task_call(sched->idle_task, sched);
  }
  return MU_SCHED_ERR_NONE;
}

mu_time_t mu_sched_get_time(mu_sched_t *sched) {
  return sched->clock_source();
}

mu_task_t *mu_sched_current_task(mu_sched_t *sched) {
  return sched->current_task;
}

mu_sched_err_t mu_sched_add(mu_sched_t *sched, mu_task_t *task) {
  if (mu_sched_has_task(sched, task)) {
    // Do not schedule task if already scheduled: it will lead to a circular
    // list.
    return MU_SCHED_ERR_ALREADY_SCHEDULED;

  } else if (mu_task_is_immediate(task)) {
    // push onto head of task list
    task->next = sched->tasks;
    sched->tasks = task;

  } else {
    // run down the linked list until we find the insertion point.
    mu_task_t *prev = NULL;
    mu_task_t *curr = sched->tasks;
    while (curr && mu_task_is_after(task, curr)) {
      prev = curr;
      curr = curr->next;
    }
    // endgame: if prev is null, insert at head.  else insert after prev
    task->next = curr;
    if (prev == NULL) {
      sched->tasks = task;
    } else {
      prev->next = task;
    }
  }
  return MU_SCHED_ERR_NONE;
}

mu_sched_err_t mu_sched_remove(mu_sched_t *sched, mu_task_t *task) {
  mu_task_t *prev = NULL;
  mu_task_t *curr = sched->tasks;
  while((curr != NULL) && (curr != task)) {
    prev = curr;
    curr = curr->next;
  }
  // endgame
  if (curr == NULL) {
    // ran out of list without finding task
    return MU_SCHED_ERR_NOT_FOUND;

  } else if (prev == NULL) {
    // delete from head of list
    sched->tasks = curr->next;

  } else {
    // delete from mid-list
    prev->next = curr->next;
  }

  task->next = NULL; // tidy up.
  return MU_SCHED_ERR_NONE;
}

mu_sched_err_t mu_sched_add_from_isr(mu_sched_t *sched, mu_task_t *task) {
  mu_queue_t *q = &(sched->isr_queue);

  mu_queue_err_t err = mu_queue_put(q, task);
  (void)err;
  MU_ASSERT(err == MU_QUEUE_ERR_NONE);
  return MU_SCHED_ERR_NONE;
}

// =============================================================================
// private code

// Find the "soonest" runnable task, remove it and return it.  If nothing
// is runnable, return NULL.
//
// Since the task list is always sorted with the "soonest" task first, we
// only need to check the first element.  Fast.
static mu_task_t *remove_runnable_task(mu_sched_t *sched, mu_time_t now) {
  mu_task_t *curr = sched->tasks;

  if (curr == NULL) {
    // no tasks
    return NULL;

  } else if (!mu_task_is_runnable(curr, now)) {
    // first task not (yet) runnable
    return NULL;

  } else {
    // first element is runnable.  pop and return.
    sched->tasks = curr->next;
    curr->next = NULL;
    return curr;
  }
}

// slurp any tasks that have been added to the ISR queue and add them to the
// scheduler's task list.
static void process_isr_queue(mu_sched_t *sched) {
  mu_task_t *task;
  mu_queue_t *q = &(sched->isr_queue);

  while (MU_QUEUE_ERR_NONE == mu_queue_get(q, (mu_queue_obj_t *)(&task))) {
    mu_sched_add(sched, task);
  }
}
