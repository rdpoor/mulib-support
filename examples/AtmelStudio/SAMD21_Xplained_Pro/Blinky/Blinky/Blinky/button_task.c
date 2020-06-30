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

#include <atmel_start.h>
#include "button_task.h"
#include "mu_sched.h"
#include "mu_task.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

typedef struct {
  mu_task_t *task;
  mu_sched_t *sched;
} button_ctx_t;

// =============================================================================
// local (forward) declarations

static void *button_task_fn(void *self, void *arg);
static void button_cb(void);

// =============================================================================
// local storage

static button_ctx_t s_button_ctx;

// =============================================================================
// public code

mu_task_t *button_task_init(mu_task_t *button_task,
                            mu_sched_t *sched) {
  // Register the button_cb function to be called upon an EIC interrupt on
  // the user button.  Save references to the task and scheduler objects so you
  // can access them from within the interrupt-level callback function.
  s_button_ctx.task = button_task;
  s_button_ctx.sched = sched;
  ext_irq_register(PIN_PA15, button_cb);

  mu_task_init(button_task, button_task_fn, NULL, "Button Interrupt");
  return button_task;
}

// =============================================================================
// local (static) code

static void *button_task_fn(void *ctx, void *arg) {
  // "context" is unused in button task
  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_time_t now = mu_sched_get_current_time(sched);

  printf("button pressed at %lu\r\n", now);
  return NULL;
}

// button_cb is triggered when a button press generates an interrupt.
// From interrupt level, schedule the button task upon leaving interrupt level.
static void button_cb() {
  mu_sched_task_from_isr(s_button_ctx.sched, s_button_ctx.task);
}
