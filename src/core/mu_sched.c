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
#include "mu_spscq.h"
#include "mu_task.h"
#include "mu_time.h"
#include <stdbool.h>
#include <string.h>     // memmove
#include <stddef.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static mu_sched_event_t *event_init(mu_sched_event_t *event, mu_task_t *task, mu_time_t time);

static void *default_idle_fn(void *self, void *arg);

static mu_sched_event_t *event_pop(mu_sched_t *sched);

static int find_event_index(mu_sched_t *sched, mu_task_t *task);

static mu_sched_event_t *find_task_event(mu_sched_t *sched, mu_task_t *task);

static void delete_event_at(mu_sched_t *sched, int index);

static mu_sched_err_t sched_task_at(mu_sched_t *sched,
                                    mu_task_t *task,
                                    mu_time_t time);

static size_t find_insertion_index(mu_sched_t *sched, mu_time_t time);

// =============================================================================
// local storage

static mu_task_t s_default_idle_task;

// =============================================================================
// public code

mu_sched_t *mu_sched_init(mu_sched_t *sched,
                          mu_sched_event_t *event_queue,
                          size_t event_queue_capacity,
                          mu_spscq_t *isr_queue) {
  sched->event_queue = event_queue;
  sched->event_queue_capacity = event_queue_capacity;
  sched->isr_queue = isr_queue;
  sched->clock_fn = mu_time_now;
  sched->idle_task = &s_default_idle_task;
  mu_task_init(&s_default_idle_task, default_idle_fn, NULL, "Idle");

  return mu_sched_reset(sched);
}

mu_sched_t *mu_sched_reset(mu_sched_t *sched) {
  sched->event_queue_count = 0;
  mu_spscq_reset(sched->isr_queue);
  sched->current_event.task = NULL;

  return sched;
}

mu_sched_err_t mu_sched_step(mu_sched_t *sched) {
  mu_task_t *task;
  mu_sched_event_t *event;
  mu_time_t now = mu_sched_get_current_time(sched);

  // first, transfer any tasks in the isr queue into the main queue
  while (mu_spscq_get(sched->isr_queue, (void **)&task) != MU_CQUEUE_ERR_EMPTY) {
    mu_sched_err_t err = mu_sched_task_at(sched, task, now);
    if (err != MU_SCHED_ERR_NONE) {
      return err;
    }
  }

  // process one event in the main queue
  event = mu_sched_get_next_event(sched);
  if (event != NULL) {
    if (!mu_time_follows(mu_sched_event_get_time(event), now)) {
      // time to run the task: grab copy of the event and removed it from queue
      memcpy(&(sched->current_event), event, sizeof(mu_sched_event_t));
      event_pop(sched);
      // invoke the task
      mu_task_call(sched->current_event.task, sched);
      // set current event task to null to signify "not running task"
      sched->current_event.task = NULL;
      return MU_SCHED_ERR_NONE;
    }
  }

  // arrive here if there was nothing to run
  mu_task_call(sched->idle_task, sched);
  return MU_SCHED_ERR_NONE;
}

mu_sched_event_t *mu_sched_event_queue(mu_sched_t *sched) { return sched->event_queue; }

mu_spscq_t *mu_sched_isr_queue(mu_sched_t *sched) { return sched->isr_queue; }

mu_task_t *mu_sched_get_idle_task(mu_sched_t *sched) {
  return sched->idle_task;
}

mu_task_t *mu_sched_get_default_idle_task(mu_sched_t *sched) {
  return &s_default_idle_task;
}

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *task) {
  sched->idle_task = task;
  return sched;
}

mu_clock_fn mu_sched_get_clock_source(mu_sched_t *sched) {
  return sched->clock_fn;
}

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn) {
  sched->clock_fn = clock_fn;
  return sched;
}

mu_time_t mu_sched_get_current_time(mu_sched_t *sched) {
  return sched->clock_fn();
}

bool mu_sched_is_empty(mu_sched_t *sched) { return sched->event_queue_count == 0; }

size_t mu_sched_event_count(mu_sched_t *sched) { return sched->event_queue_count; }

mu_sched_event_t *mu_sched_get_current_event(mu_sched_t *sched) {
  if (sched->current_event.task == NULL) {
    return NULL;
  } else {
    return &sched->current_event;
  }
}
mu_sched_event_t *mu_sched_get_next_event(mu_sched_t *sched) {
  if (sched->event_queue_count == 0) {
    return NULL;
  } else {
    return &sched->event_queue[sched->event_queue_count - 1];
  }
}

mu_task_t *mu_sched_event_get_task(mu_sched_event_t *event) {
  return event->task;
}

mu_time_t mu_sched_event_get_time(mu_sched_event_t *event) {
  return event->time;
}

mu_sched_err_t mu_sched_remove_task(mu_sched_t *sched, mu_task_t *task) {
  int index = find_event_index(sched, task);
  if (index < 0) {
    return MU_SCHED_ERR_NOT_FOUND;
  } else {
    delete_event_at(sched, index);
    return MU_SCHED_ERR_NONE;
  }
}

mu_sched_err_t mu_sched_task_now(mu_sched_t *sched, mu_task_t *task) {
  return sched_task_at(sched, task, mu_sched_get_current_time(sched));
}

mu_sched_err_t mu_sched_task_at(mu_sched_t *sched, mu_task_t *task, mu_time_t at) {
  return sched_task_at(sched, task, at);
}

mu_sched_err_t mu_sched_task_in(mu_sched_t *sched, mu_task_t *task, mu_time_dt in) {
  return sched_task_at(sched, task, mu_time_offset(mu_sched_get_current_time(sched), in));
}

mu_sched_err_t mu_sched_reschedule_in(mu_sched_t *sched, mu_time_dt in) {
  mu_sched_event_t *event = mu_sched_get_current_event(sched);
  if (!event) {
    return MU_SCHED_ERR_NOT_FOUND;
  }
  mu_task_t *task = event->task;
  mu_time_t time = event->time;
  return sched_task_at(sched, task, mu_time_offset(time, in));
}

mu_sched_err_t mu_sched_reschedule_now(mu_sched_t *sched) {
  mu_sched_event_t *event = mu_sched_get_current_event(sched);
  if (!event) {
    return MU_SCHED_ERR_NOT_FOUND;
  }
  return mu_sched_task_now(sched, event->task);
}

mu_sched_err_t mu_sched_task_from_isr(mu_sched_t *sched, mu_task_t *task) {
  if (mu_spscq_put(sched->isr_queue, task) == MU_CQUEUE_ERR_FULL) {
    return MU_SCHED_ERR_FULL;
  } else {
    return MU_SCHED_ERR_NONE;
  }
}

mu_sched_task_status_t mu_sched_get_task_status(mu_sched_t *sched,
                                                mu_task_t *task) {
  mu_sched_event_t *event;

  event = mu_sched_get_current_event(sched);
  if (event && mu_sched_event_get_task(event) == task) {
    // task is the current task
    return MU_SCHED_TASK_STATUS_ACTIVE;
  }

  event = find_task_event(sched, task);
  if (event == NULL) {
    // task is not in the schedule at all
    return MU_SCHED_TASK_STATUS_IDLE;
  }

  mu_time_t now = mu_sched_get_current_time(sched);
  if (!mu_time_follows(mu_sched_event_get_time(event), now)) {
    // task's time has arrived, but it's not yet running
    return MU_SCHED_TASK_STATUS_WAITING;

  } else {
    // task is scheduled for some pont in the future
    return MU_SCHED_TASK_STATUS_SCHEDULED;
  }
}

// =============================================================================
// local (static) code

static mu_sched_event_t *event_init(mu_sched_event_t *event, mu_task_t *task, mu_time_t time) {
  event->task = task;
  event->time = time;
  return event;
}

static void *default_idle_fn(void *self, void *arg) {
  // the default idle task doesn't do much...
  return self;
}

static mu_sched_event_t *event_pop(mu_sched_t *sched) {
  // assume > 1 element in queue!
  return &sched->event_queue[--sched->event_queue_count];
}

static int find_event_index(mu_sched_t *sched, mu_task_t *task) {
  for (int i = sched->event_queue_count - 1; i >= 0; i--) {
    mu_sched_event_t *event = &sched->event_queue[i];
    if (event->task == task)
      return i;
  }
  return -1;
}

static mu_sched_event_t *find_task_event(mu_sched_t *sched, mu_task_t *task) {
  int i = find_event_index(sched, task);
  if (i < 0) {
    return NULL;
  } else {
    return &sched->event_queue[i];
  }
}

static void delete_event_at(mu_sched_t *sched, int index) {
  // assumes valid argument!
  int to_move = sched->event_queue_count - index - 1;
  if (to_move > 0) {
    // move existing items down
    mu_sched_event_t *dst = &sched->event_queue[index];
    mu_sched_event_t *src = &sched->event_queue[index + 1];
    memmove(dst, src, sizeof(mu_sched_event_t) * to_move);
  }
  sched->event_queue_count -= 1;
}

static mu_sched_err_t sched_task_at(mu_sched_t *sched, mu_task_t *task, mu_time_t time) {
  mu_sched_event_t *event;
  size_t index;

  if (sched->event_queue_count >= sched->event_queue_capacity) {
    return MU_SCHED_ERR_FULL;
  }
  if (sched->event_queue_count == 0) {
    index = 0;
  } else {
    index = find_insertion_index(sched, time);
  }
  int to_move = sched->event_queue_count - index;
  if (to_move > 0) {
    // carve a hole at index
    mu_sched_event_t *src = &sched->event_queue[index];
    mu_sched_event_t *dst = &sched->event_queue[index + 1];
    memmove(dst, src, sizeof(mu_sched_event_t) * to_move);
  }
  // fill in the event
  event = &sched->event_queue[index];
  event_init(event, task, time);
  sched->event_queue_count += 1;
  return MU_SCHED_ERR_NONE;
}

#if MU_SCHED_BINARY_SEARCH
// binary search for insertion point
static size_t find_insertion_index(mu_sched_t *sched, mu_time_t time) {
  mu_sched_event_t *events = sched->event_queue;
  int low = 0;
  int high = sched->event_queue_count - 1;
  while (low <= high) {
    int mid = (low + high) / 2;
    // events furthest in the future tend towards index=0
    if (!mu_time_follows(events[mid].time, time)) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return low;
}

#else
// linear search for insertion point
static size_t find_insertion_index(mu_sched_t *sched, mu_time_t time) {
  mu_sched_event_t *events = sched->event_queue;

  for (size_t i = sched->event_queue_count - 1; i >= 0; i--) {
    if (mu_time_precedes(time, events[i].time)) {
      return i+1;    // precedes incumbent
    } else if (i == 0) {
      return 0;      // follows all
    }
  }
}
#endif
