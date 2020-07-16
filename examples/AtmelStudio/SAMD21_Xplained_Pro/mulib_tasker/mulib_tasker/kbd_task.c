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
#include "mulib.h"
#include "mulib_tasker.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *kbd_task_fn(void *ctx, void *arg);

// If using ring buffer:
// static void kbd_cb(SERCOM_USART_EVENT event, uintptr_t context);
static void kbd_cb(uintptr_t context);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_task_t *kbd_task_init(mu_task_t *kbd_task,
                         kbd_ctx_t *kbd_ctx,
                         mu_sched_t *sched) {
  // Register the kbd_cb function to be called upon an serial interrupt.
  kbd_ctx->task = kbd_task;
  kbd_ctx->sched = sched;

  SERCOM2_USART_ReadCallbackRegister(kbd_cb, (uintptr_t)kbd_ctx);
  SERCOM2_USART_Read(&kbd_ctx->ch, 1); // start initial read
  mu_task_init(kbd_task, kbd_task_fn, kbd_ctx, "Keyboard Task");

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

static void *kbd_task_fn(void *ctx, void *arg) {
  // keyoard context is passed as first argument, scheduler as the second.
  kbd_ctx_t *kbd_ctx = (kbd_ctx_t *)ctx;
  // mu_sched_t *sched = (mu_sched_t *)arg;
  USART_ERROR err = SERCOM2_USART_ErrorGet(); // clears error if any...
  if (err != USART_ERROR_NONE) {
    printf("kbd_task error %d\r\n", err);
  } else {
    switch (kbd_ctx->ch) {
    case 'b':
      mulib_tasker_start_led_task();
      break;
    case 'B':
      mulib_tasker_stop_led_task();
      break;
    case 'd':
      mulib_tasker_start_screen_update_task();
      break;
    case 'D':
      mulib_tasker_stop_screen_update_task();
      break;
    case 'p':
      mulib_tasker_set_low_power_mode(true);
      break;
    }
  }
  SERCOM2_USART_Read(&kbd_ctx->ch, 1); // start next read
  return NULL;
}

// kbd_cb is triggered when the UART receives a character and generates an
// interrupt.  From interrupt level, schedule the kbd task upon leaving
// interrupt level.
//
// Developer Note: If using ring buffer, event is one of:
//   SERCOM_USART_EVENT_READ_THRESHOLD_REACHED = 0,
//   SERCOM_USART_EVENT_READ_BUFFER_FULL,
//   SERCOM_USART_EVENT_READ_ERROR,
//   SERCOM_USART_EVENT_WRITE_THRESHOLD_REACHED,
//
// static void kbd_cb(SERCOM_USART_EVENT event, uintptr_t context)
//
// Arrive here at interrupt level when the SERCOM driver has received a
// character.
static void kbd_cb(uintptr_t context) {
  kbd_ctx_t *kbd_ctx = (kbd_ctx_t *)context;
  mu_task_t *task = kbd_ctx->task;
  mu_sched_t *sched = kbd_ctx->sched;
  mu_sched_task_from_isr(sched, task);
}
