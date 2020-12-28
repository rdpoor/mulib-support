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

#include "timer_task.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_time.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

void *timer_task_fn(void *ctx, void *arg);

// =============================================================================
// local storage

// =============================================================================
// public code

timer_task_t *timer_task_init(timer_task_t *timer_task,
                              mu_sched_t *sched,
                              mu_task_t *target_task,
                              mu_time_dt interval) {
  timer_task->sched = sched;
  timer_task->target_task = target_task;
  timer_task->interval = interval;
  return timer_task;
}

mu_task_t *timer_task_get_target_task(timer_task_t *timer_task) {
  return timer_task->target_task;
}

void timer_task_set_target_task(timer_task_t *timer_task, mu_task_t *target_task) {
  timer_task->target_task = target_task;
}

mu_time_dt timer_task_get_interval(timer_task_t *timer_task) {
  return timer_task->interval;
}

void timer_task_set_interval(timer_task_t *timer_task, mu_time_dt interval) {
  timer_task->interval = interval;
}

void timer_task_start(timer_task_t *timer_task) {
  timer_task->is_running = false;
  // NOTE: if timer_task was already scheduled, this will remove it and restart
  // it now.
  mu_sched_task_now(timer_task->sched, timer_task->task);
}

void timer_task_stop(timer_task_t *timer_task) {
  timer_task->is_running = false;
}

// =============================================================================
// local (static) code

void *timer_task_fn(void *ctx, void *arg) {
  timer_task_t *timer_task = (timer_task_t *ctx);
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_time_t now = mu_sched_get_current_time(sched);
  mu_task_t task = mu_sched_get_current_task(sched);

  if (timer_task->is_running) {
    mu_task_call(timer_task->target_task, arg);
    mu_sched_task_again(sched, task, timer_task->interval);
  }
}
