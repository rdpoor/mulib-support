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

#include "screen_update_task.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_time.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *screen_update_task_fn(void *ctx, void *arg);

// =============================================================================
// local storage

static mu_task_t s_screen_update_task;
static mu_task_t *s_screen_redraw_task;

// =============================================================================
// public code

mu_task_t *screen_update_task_init(mu_task_t *screen_redraw_task) {
  // capture a reference to the screen redraw task.
  s_screen_redraw_task = screen_redraw_task;

  // initialize the screen update task
  mu_task_init(&s_screen_update_task, screen_update_task_fn, NULL, "Screen Update");

  // return a pointer to the screen update task: the scheduler will schedule it
  // imediately.
  return &s_screen_update_task;
}

// =============================================================================
// local (static) code

// Called when the task triggers
static void *screen_update_task_fn(void *ctx, void *arg) {
  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;

  // schedule a call to the screen redraw task now
  mu_sched_task_now(sched, s_screen_redraw_task);

  // retrigger the screen update task.  Note that the scheduled time is computed
  // by adding SCREEN_UPDATE_INTERVAL to the current event's time -- this avoids
  // timing drift.
  mu_event_t *event = mu_sched_get_current_event(sched);
  mu_time_t this_time = mu_event_get_time(event);
  mu_time_t next_time = mu_time_offset(this_time, SCREEN_UPDATE_INTERVAL);
  mu_sched_task_at(sched, &s_screen_update_task, next_time);

  return NULL;
}
