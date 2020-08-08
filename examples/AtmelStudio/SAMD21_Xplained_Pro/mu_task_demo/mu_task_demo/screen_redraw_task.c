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

#include "screen_redraw_task.h"
#include "mulib.h"
#include "mu_task_demo.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void *screen_redraw_task_fn(void *ctx, void *arg);
static void serial_write_cb(void *arg);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_task_t *screen_redraw_task_init(mu_task_t *screen_redraw_task,
                                   screen_redraw_ctx_t *screen_redraw_ctx) {
  screen_redraw_ctx->in_progress = false;
  screen_redraw_ctx->sched = mu_task_demo_get_scheduler();
  screen_redraw_ctx->task = screen_redraw_task;

  mu_task_init(screen_redraw_task,
               screen_redraw_task_fn,
               screen_redraw_ctx,
               "Screen Redraw");
  return screen_redraw_task;
}

// =============================================================================
// local (static) code

/**
 * @brief Repaint the screen.
 *
 * Print one character from the screen buffer (a mu_substring).  If there are
 * any characters remaining, reschedule immediately to print the next char.
 */
static void *screen_redraw_task_fn(void *ctx, void *arg) {
  // screen_redraw_context is passed as the first argument, scheduler is second
  screen_redraw_ctx_t *screen_redraw_ctx = (screen_redraw_ctx_t *)ctx;

  mu_substr_t *screen_buffer = mu_task_demo_get_screen_buffer();

  if (mu_substr_length(screen_buffer) > 0) {
    if (!screen_redraw_ctx->in_progress) {
      // First time redraw_task is called: enable write callbacks, which will
      // cause redraw_task to be called whenever the UART can accept another
      // byte.
      screen_redraw_ctx->in_progress = true;
      mu_vm_serial_set_write_cb(serial_write_cb, screen_redraw_ctx);

    } else {
      // Arrive here from a write callback.  UART is ready to accept another
      // byte: fetch from the screen buffer and send it to the UART.
      mu_str_data_t ch;
      // CODE SMELL ALERT: mu_substr_get() fetches a character relative to
      // strbuf->start.  But we don't provide a way to modify start.  So...
      mu_substr_err_t err = mu_substr_get(screen_buffer, 0, &ch);
      screen_buffer->start += 1;

      if (err == MU_SUBSTR_ERR_NONE) {
        mu_vm_serial_write(ch);
      }
    }

  } else {
    // no chars remaining to be sent: turn off write callbacks.
    mu_vm_serial_set_write_cb(NULL, NULL);
    screen_redraw_ctx->in_progress = false;
  }

  return NULL;
}

/**
 * Called from interrupt level when the USART is reay to accept a char.
 * Schedule a call to screen_redraw_task_fn() to write the char.
 */
static void serial_write_cb(void *arg) {
    screen_redraw_ctx_t *screen_redraw_ctx = (screen_redraw_ctx_t *)arg;
    mu_sched_task_from_isr(screen_redraw_ctx->sched, screen_redraw_ctx->task);
}
