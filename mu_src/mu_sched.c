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
#include "mu_port.h"
#include "mu_evt.h"
#include "mu_ring.h"
#include "mu_task.h"
#include <stdbool.h>
#include <string.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static mu_evt_t *remove_runnable_event(mu_sched_t *sched, mu_time_t now);

// Transfer ISR-initiated events from the ISR queue to the schedule
static void process_isr_queue(mu_sched_t *sched);

// Return true if evt is in the list of scheduled events
static bool is_scheduled(mu_sched_t *sched, mu_evt_t *evt);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_sched_t *mu_sched_init(mu_sched_t *sched,
                          mu_ring_obj_t *isr_queue_pool,
                          unsigned int isr_queue_pool_size) {
  mu_ring_err_t err;
  mu_ring_t *q = &(sched->isr_queue);

  (void)err;
  sched->events = (mu_evt_t *)NULL;     // event queue starts out empty
  err = mu_ring_init(q, isr_queue_pool, isr_queue_pool_size);
  MU_ASSERT(err == MU_RING_ERR_NONE);
  sched->clock_source = mu_time_now;  // default clock source
  sched->idle_task = NULL;
  return mu_sched_reset(sched);
}

mu_sched_t *mu_sched_reset(mu_sched_t *sched) {
  mu_evt_t *evt;

  while ((evt = sched->events) != NULL) {
    sched->events = evt->next;
    evt->next = NULL;
  }

  if (sched->current_event) {
    sched->current_event->next = NULL;
    sched->current_event = NULL;
  }

  mu_ring_reset(&(sched->isr_queue));

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
  return sched->events == (mu_evt_t *)NULL;
}

bool mu_sched_has_event(mu_sched_t *sched, mu_evt_t *event) {
	mu_evt_t *evts = mu_sched_get_events(sched);
	while(evts) {
		if (evts == event) return true;
		evts = evts->next;
	}
	return false;
}

unsigned int mu_sched_task_count(mu_sched_t *sched) {
	unsigned int count = 0;
	mu_evt_t *evts = mu_sched_get_events(sched);
	while(evts) {
		count += 1;
    evts = evts->next;
	}
	return count;
}

mu_evt_t *mu_sched_get_events(mu_sched_t *sched) {
	return sched->events;
}

mu_sched_err_t mu_sched_step(mu_sched_t *sched) {
  mu_evt_t *runnable;
  mu_time_t now = mu_sched_get_time(sched);

  // add any events that have arrived on the ISR queue
  process_isr_queue(sched);

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

mu_time_t mu_sched_get_time(mu_sched_t *sched) {
  return sched->clock_source();
}

mu_evt_t *mu_sched_current_event(mu_sched_t *sched) {
  return sched->current_event;
}

mu_sched_err_t mu_sched_queue(mu_sched_t *sched, mu_evt_t *event) {
  if (is_scheduled(sched, event)) {
    // If you schedule an event that's already in the schedule, you get a
    // circular list.  Since traversing the list adds time, a future version
    // might set a bit in the event to indicate if it's in the schedule or not.
    asm("nop");

  } else if (mu_evt_is_immediate(event)) {
    // push onto head of event list
    event->next = sched->events;
    sched->events = event;

  } else {
    // run down the linked list until we find the insertion point.
    mu_evt_t *prev = NULL;
    mu_evt_t *curr = sched->events;
    while (curr && mu_evt_is_after(event, curr)) {
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

mu_sched_err_t mu_sched_from_isr(mu_sched_t *sched, mu_evt_t *event) {
  mu_ring_t *q = &(sched->isr_queue);

  mu_ring_err_t err = mu_ring_put(q, event);
  (void)err;
  MU_ASSERT(err == MU_RING_ERR_NONE);
  return MU_SCHED_ERR_NONE;
}

// =============================================================================
// private code

// Find the "soonest" runnable event, remove it and return it.  If nothing
// is runnable, return NULL.
//
// Since the event list is always sorted with the "soonest" event first, we
// only need to check the first element.  Fast.
static mu_evt_t *remove_runnable_event(mu_sched_t *sched, mu_time_t now) {
  mu_evt_t *curr = sched->events;

  if (curr == NULL) {
    // no events
    return NULL;

  } else if (!mu_evt_is_runnable(curr, now)) {
    // first event not (yet) runnable
    return NULL;

  } else {
    // first element is runnable.  pop and return.
    sched->events = curr->next;
    curr->next = NULL;
    return curr;
  }
}

// slurp any events that have been added to the ISR queue and add them to the
// scheduler's event list.
static void process_isr_queue(mu_sched_t *sched) {
  mu_evt_t *evt;
  mu_ring_t *q = &(sched->isr_queue);

  while (MU_RING_ERR_NONE == mu_ring_get(q, (mu_ring_obj_t *)(&evt))) {
    mu_sched_queue(sched, evt);
  }
}

// Return true if evt is present in the scheduled events.
static bool is_scheduled(mu_sched_t *sched, mu_evt_t *evt) {
  mu_evt_t *curr = sched->events;
  while (curr) {
    if (curr == evt) return true;
    curr = curr->next;
  }
  return false;
}
