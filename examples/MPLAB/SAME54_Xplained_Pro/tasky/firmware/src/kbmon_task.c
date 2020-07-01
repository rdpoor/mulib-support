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

#include "kbmon_task.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_task.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *kbmon_task_fn(void *ctx, void *arg);
static void kbmon_cb(uintptr_t context);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_task_t *kbmon_task_init(mu_task_t *kbmon_task,
                            kbmon_ctx_t *kbmon_ctx,
                            mu_sched_t *sched) {
  // Configure the kbmon_cb function to be called upon a UART interrupt.
  // Register the callback with a reference to kbmon_ctx so we can access
  // the task and scheduler objects from within the interrupt.
  kbmon_ctx->task = kbmon_task;
  kbmon_ctx->sched = sched;
  SERCOM2_USART_ReadCallbackRegister(kbmon_cb, (uintptr_t)kbmon_ctx);
  SERCOM2_USART_Read(&kbmon_ctx->chr, 1);
  mu_task_init(kbmon_task, kbmon_task_fn, NULL, "KB Monitor");

  return kbmon_task;
}

// =============================================================================
// local (static) code

static void *kbmon_task_fn(void *ctx, void *arg) {
  // context is passed as the first argument, scheduler as the second
  kbmon_ctx_t *kbmon_ctx = (kbmon_ctx_t *)ctx;
  // mu_sched_t *sched = (mu_sched_t *)arg;

  printf("kbmon received '%c'\r\n", kbmon_ctx->chr);
  // set up to read another char
  SERCOM2_USART_Read(&kbmon_ctx->chr, 1);

  return NULL;
}

// kbmon_cb is triggered when a button press generates an interrupt.
// From interrupt level, schedule the button task upon leaving interrupt level.
static void kbmon_cb(uintptr_t context) {
  kbmon_ctx_t *kbmon_ctx = (kbmon_ctx_t *)context;
  mu_task_t *task = kbmon_ctx->task;
  mu_sched_t *sched = kbmon_ctx->sched;
  mu_sched_task_from_isr(sched, task);
}
