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

#include "task_demo.h"
#include "button_task.h"
#include "idle_task.h"
#include "kbd_task.h"
#include "led_task.h"
#include "mu_event.h"
#include "mu_port.h"
#include "mu_sched.h"
#include "mu_spscq.h"
#include "mu_task.h"
#include "screen_redraw_task.h"
#include "screen_update_task.h"
#include <definitions.h>
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

#define EVENT_QUEUE_CAPACITY 10
#define ISR_Q_CAPACITY 8 // must be a power of two

typedef enum {
  LED_TASK_IDX,           // blink the LED periodically
  BUTTON_TASK_IDX,        // monitor button push interrupts
  SCREEN_UPDATE_TASK_IDX, // periodically start the screen refresh task
  SCREEN_REDRAW_TASK_IDX, // update the screen, then stop.
  KBD_TASK_IDX,           // keyboard monitor
  IDLE_TASK_IDX,          // run when there is nothing else to run
  TASK_COUNT
} task_idx_t;

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

// the scheduler state
static mu_sched_t s_sched;

// the backing store for scheduled events
static mu_event_t s_event_queue[EVENT_QUEUE_CAPACITY];

// the queue to hold tasks queued from interrupt level, and its backing store
static mu_spscq_t s_isr_queue;
static mu_spscq_item_t s_isr_queue_items[ISR_Q_CAPACITY];

// Normally you would not need to keep the task objects in an array, but the
// point of this demo is to continually display the state of each task, so
// an array makes it easy to iterate over all tasks.

static mu_task_t s_tasks[TASK_COUNT];

static led_ctx_t s_led_ctx;
static button_ctx_t s_button_ctx;
static screen_update_ctx_t s_screen_update_ctx;
static screen_redraw_ctx_t s_screen_redraw_ctx;
static kbd_ctx_t s_kbd_ctx;

// =============================================================================
// public code

void task_demo_init() {
  printf("\n\n# ===========\n");
  printf("# task_demo %s: see https://github.com/rdpoor/mulib\n",
         TASK_DEMO_VERSION);

  // initialize the port-specific interface
  mu_port_init();

  // set up the isr queue and the scheduler
  mu_spscq_init(&s_isr_queue, s_isr_queue_items, ISR_Q_CAPACITY);
  mu_sched_init(&s_sched, s_event_queue, EVENT_QUEUE_CAPACITY, &s_isr_queue);

  // initialize tasks
  led_task_init(&s_tasks[LED_TASK_IDX], &s_led_ctx);
  button_task_init(&s_tasks[BUTTON_TASK_IDX], &s_button_ctx, &s_sched);
  screen_update_task_init(&s_tasks[SCREEN_UPDATE_TASK_IDX],
                          &s_screen_update_ctx,
                          &s_tasks[SCREEN_REDRAW_TASK_IDX]);
  screen_redraw_task_init(&s_tasks[SCREEN_REDRAW_TASK_IDX],
                          &s_screen_redraw_ctx,
                          s_tasks,
                          TASK_COUNT);
  kbd_task_init(&s_tasks[KBD_TASK_IDX], &s_kbd_ctx, &s_sched);
  idle_task_init(&s_tasks[IDLE_TASK_IDX]);

  // install the sleep-aware idle task as the scheduler's idle task
  mu_sched_set_idle_task(&s_sched, &s_tasks[IDLE_TASK_IDX]);

  // schedule the initial call to the LED and Screen Update tasks
  mu_sched_task_now(&s_sched, &s_tasks[LED_TASK_IDX]);
  mu_sched_task_now(&s_sched, &s_tasks[SCREEN_UPDATE_TASK_IDX]);
}

void task_demo_step() {
  // called repeatedly from main(): run the scheduler
  mu_sched_step(&s_sched);
}
