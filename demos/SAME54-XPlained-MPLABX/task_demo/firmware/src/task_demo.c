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

#include "task_demo.h"
#include "led_task.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_time.h"
#include "screen_redraw_task.h"
#include "screen_update_task.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

#define TASK_DEMO_VERSION "1.0.0"

#define EVENT_QUEUE_SIZE 10
#define ISR_Q_CAPACITY 8

typedef enum {
  IDLE_TASK_INDEX,
  LED_TASK_INDEX,
  REDRAW_TASK_INDEX,
  UPDATE_TASK_INDEX,
  N_TASKS
} task_indeces;

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

// the scheduler state
static mu_sched_t s_sched;

// the backing store for scheduled events
static mu_event_t s_event_queue[EVENT_QUEUE_SIZE];

// the queue to hold tasks queued from interrupt level, and its backing store
static mu_spscq_t s_isr_queue;
static mu_spscq_item_t s_isr_queue_items[ISR_Q_CAPACITY];

static mu_task_t *s_tasks[N_TASKS];

// =============================================================================
// public code

void task_demo_init() {
  printf("\n\n##############\n");
  printf("# task_demo %s\n", TASK_DEMO_VERSION);

  // initialize the real-time clock
  mu_time_init();

  // set up the task queue, the isr queue and the scheduler
  mu_spscq_init(&s_isr_queue, s_isr_queue_items, ISR_Q_CAPACITY);
  mu_sched_init(&s_sched, s_event_queue, EVENT_QUEUE_SIZE, &s_isr_queue);

  // initiate and install the tasks in the task array.
  s_tasks[IDLE_TASK_INDEX] = mu_sched_get_idle_task(&s_sched);
  s_tasks[LED_TASK_INDEX] = led_task_init();
  s_tasks[REDRAW_TASK_INDEX] = screen_redraw_task_init(s_tasks, N_TASKS);
  s_tasks[UPDATE_TASK_INDEX] = screen_update_task_init(s_tasks[2]);

  // schedule the initial tasks
  mu_sched_task_now(&s_sched, s_tasks[LED_TASK_INDEX]);
  mu_sched_task_now(&s_sched, s_tasks[UPDATE_TASK_INDEX]);
}

void task_demo_step() {
  // called repeatedly from main(): run the scheduler
  mu_sched_step(&s_sched);
}

// =============================================================================
// local (static) code
