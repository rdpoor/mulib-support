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

#include "kbd_task.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_task.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *kbd_task_fn(void *ctx, void *arg);
static void kbd_cb(uintptr_t context);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_task_t *kbd_task_init(mu_task_t *kbd_task,
                            kbd_ctx_t *kbd_ctx,
                            mu_sched_t *sched) {
  // Register the kbd_cb function to be called upon an EIC interrupt on
  // the user button.  Register the callback with a reference to kbd_ctx
  // so we can access the task and scheduler objects from within the interrupt.
  kbd_ctx->task = kbd_task;
  kbd_ctx->sched = sched;

  SERCOM2_USART_ReadCallbackRegister(kbd_cb, (uintptr_t)kbd_ctx);
  mu_task_init(kbd_task, kbd_task_fn, NULL, "Button Push");

  return kbd_task;
}

// =============================================================================
// local (static) code

static void *kbd_task_fn(void *ctx, void *arg) {
  // "context" is unused in kbd task
  // scheduler is passed as the second argument.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_time_t now = mu_sched_get_current_time(sched);

  printf("button pressed at %lu\r\n", now);
  return NULL;
}

// kbd_cb is triggered when a button press generates an interrupt.
// From interrupt level, schedule the button task upon leaving interrupt level.
static void kbd_cb(uintptr_t context) {
  kbd_ctx_t *kbd_ctx = (kbd_ctx_t *)context;
  mu_task_t *task = kbd_ctx->task;
  mu_sched_t *sched = kbd_ctx->sched;
  mu_sched_task_from_isr(sched, task);
}
