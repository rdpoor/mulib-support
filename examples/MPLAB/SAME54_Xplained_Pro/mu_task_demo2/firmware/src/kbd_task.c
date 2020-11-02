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
#include "mulib.h"
#include "mu_thunk_demo.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *kbd_task_fn(void *ctx, void *arg);

// If using ring buffer:
static void kbd_cb(void *context);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_thunk_t *kbd_task_init(mu_thunk_t *kbd_task, kbd_ctx_t *kbd_ctx) {
  // The keyboard callback function takes one user-supplied argument.  But in
  // the callback, we need a reference to both the scheduler and to the task,
  // so we store them both in the kbd_ctx structure and pass that as an argument
  // to mu_vm_serial_set_read_cb()
  kbd_ctx->task = kbd_task;
  kbd_ctx->sched = mu_thunk_demo_get_scheduler();

  // initialize the kbd_task object
  mu_thunk_init(kbd_task, kbd_task_fn, kbd_ctx, "Keyboard Task");

  // establish keyboard read interrupt callback.
  mu_vm_serial_set_read_cb(kbd_cb, kbd_ctx);

  return kbd_task;
}

void kbd_task_set_low_power_mode(bool low_power) {
  if (low_power) {
    // disable USART
    SERCOM2_REGS->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;
  } else {
    // enable USART
    SERCOM2_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;
    while (SERCOM2_REGS->USART_INT.SERCOM_SYNCBUSY)
      ;
  }
}

// =============================================================================
// local (static) code

/**
 * Arrive here when a character is available for reading.
 */
static void *kbd_task_fn(void *ctx, void *arg) {
  // keyoard context is passed as first argument, scheduler as the second, but
  // neither are used.
  // kbd_ctx_t *kbd_ctx = (kbd_ctx_t *)ctx;
  // mu_sched_t *sched = (mu_sched_t *)arg;
  uint8_t ch = mu_vm_serial_read();
  switch (ch) {
  case 'b':
    mu_thunk_demo_start_led_task();
    break;
  case 'B':
    mu_thunk_demo_stop_led_task();
    break;
  case 'd':
    mu_thunk_demo_start_screen_trigger_task();
    break;
  case 'D':
    mu_thunk_demo_stop_screen_trigger_task();
    break;
  case 'p':
    mu_thunk_demo_set_low_power_mode(true);
    break;
  }
  return NULL;
}

// kbd_cb is triggered when the UART receives a character and generates an
// interrupt.  From interrupt level, schedule the kbd task upon leaving
// interrupt level.
static void kbd_cb(void *context) {
  kbd_ctx_t *kbd_ctx = (kbd_ctx_t *)context;
  mu_thunk_t *thunk = kbd_ctx->task;
  mu_sched_t *sched = kbd_ctx->sched;
  mu_sched_task_from_isr(sched, task);
}
