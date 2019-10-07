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
#include "../port/port.h"
#include "mu_evt.h"
#include "mu_task.h"
#include <stdbool.h>
#include <string.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static mu_evt_t *remove_runnable_event(mu_sched_t *sched, port_time_t now);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_sched_t *mu_sched_init(mu_sched_t *sched) {
  sched->events = (mu_evt_t *)NULL;
  sched->clock_source = port_time_now;  // default clock source
  sched->idle_task = NULL;
  return mu_sched_reset(sched);
}

mu_sched_t *mu_sched_reset(mu_sched_t *sched) {
  sched->current_event = NULL;
  return sched;
}

mu_sched_t *mu_sched_set_clock_source(mu_sched_t *sched, mu_clock_fn clock_fn) {
  sched->clock_source = clock_fn;
  return sched;
}

mu_sched_t *mu_sched_set_idle_task(mu_sched_t *sched, mu_task_t *idle_task) {
  sched->idle_task = idle_task;
  return sched;
}

bool mu_sched_is_empty(mu_sched_t *sched) {
  return sched->events == (mu_evt_t *)NULL;
}

mu_sched_err_t mu_sched_step(mu_sched_t *sched) {
  mu_evt_t *runnable;
  port_time_t now = sched->clock_source();

  if ((runnable = remove_runnable_event(sched, now)) != NULL) {
    // We've found an event that's runnable and have removed it from the
    // event queue.  Set current event and call the task.
    sched->current_event = runnable;
    mu_evt_call(runnable, sched);
    sched->current_event = NULL;

  } else {
    // Nothing ready to run.  Call the idle task.  NOTE: mu_evt_call() handles
    // the case that sched->idle_task is null, so we don't need to check.
    mu_task_call(sched->idle_task, sched);
  }
  return MU_SCHED_ERR_NONE;
}

mu_evt_t *mu_sched_current_event(mu_sched_t *sched) {
  return sched->current_event;
}

mu_sched_err_t mu_sched_add(mu_sched_t *sched, mu_evt_t *event) {
  if (mu_evt_is_immediate(event)) {
    // push onto head of event list
    event->next = sched->events;
    sched->events = event;

  } else {
    mu_evt_t *prev = NULL;
    mu_evt_t *curr = sched->events;
    while((curr != NULL) && mu_evt_is_after(event, curr)) {
      prev = curr;
      curr = curr->next;
    }
    // endgame: if prev is null, insert at head.  else insert after prev
    event->next = curr;
    if (prev == NULL) {
      sched->events = event;
    } else {
      prev->next = event;
    }
  }
  return MU_SCHED_ERR_NONE;
}

mu_sched_err_t mu_sched_remove(mu_sched_t *sched, mu_evt_t *evt) {
  mu_evt_t *prev = NULL;
  mu_evt_t *curr = sched->events;
  while((curr != NULL) && (curr != evt)) {
    prev = curr;
    curr = curr->next;
  }
  // endgame
  if (curr == NULL) {
    // ran out of list without finding evt
    return MU_SCHED_ERR_NOT_FOUND;

  } else if (prev == NULL) {
    // delete from head of list
    sched->events = curr->next;

  } else {
    // delete from mid-list
    prev->next = curr->next;
  }

  evt->next = NULL; // tidy up.
  return MU_SCHED_ERR_NONE;
}

// =============================================================================
// private code

// Consider writing a traversal function, use it for:
// mu_sched_add()
// mu_sched_remove()
// remove_runnable_event

static mu_evt_t *remove_runnable_event(mu_sched_t *sched, port_time_t now) {
  mu_evt_t *prev = NULL;
  mu_evt_t *curr = sched->events;
  while((curr != NULL) && (!mu_evt_is_runnable(curr, now))) {
    prev = curr;
    curr = curr->next;
  }
  // endgame
  if (curr == NULL) {
    // ran out of list without finding a runnable event
    return NULL;

  } else if (prev == NULL) {
    // delete from head of list
    sched->events = curr->next;

  } else {
    // delete from mid-list
    prev->next = curr->next;
  }

  curr->next = NULL; // tidy up.
  return curr;
}
