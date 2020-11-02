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

#include "screen_update_task.h"
#include "mu_thunk_demo.h"
#include "mulib.h"
#include <stddef.h>

// =============================================================================
// local types and definitions

// character sequence to home cursor and clear to end of screen
#define CLEAR_SCREEN "\e[1;1H\e[2J"

// =============================================================================
// local (forward) declarations

static void *screen_update_task_fn(void *ctx, void *arg);
static void repaint_screen(screen_update_ctx_t *ctx, uint8_t slice);
static void repaint_task(screen_update_ctx_t *ctx, mu_thunk_t *thunk);
static char get_task_state(mu_thunk_t *thunk, mu_sched_t *sched);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_thunk_t *screen_update_task_init(mu_thunk_t *screen_update_task,
                                   screen_update_ctx_t *screen_update_ctx) {

  screen_update_ctx->screen_redraw_task = mu_thunk_demo_get_screen_redraw_task();
  screen_update_ctx->n_tasks = mu_thunk_demo_get_task_count();
  screen_update_ctx->tasks = mu_thunk_demo_get_tasks();
  screen_update_ctx->sched = mu_thunk_demo_get_scheduler();
  screen_update_ctx->screen_buffer = mu_thunk_demo_get_screen_buffer();
  screen_update_ctx->slice = 0;

  // initialize the screen update task
  mu_thunk_init(screen_update_task,
               screen_update_task_fn,
               screen_update_ctx,
               "Screen Update");
  return screen_update_task;
}

// =============================================================================
// local (static) code

static void *screen_update_task_fn(void *ctx, void *arg) {
  // screen_update_context is passed as the first argument, scheduler is second
  screen_update_ctx_t *screen_update_ctx = (screen_update_ctx_t *)ctx;
  mu_sched_t *sched = (mu_sched_t *)arg;

  if (screen_update_ctx->slice <= screen_update_ctx->n_tasks + 2) {
    // repaint the screen buffer in four slices in order to reduce latency for
    // other tasks.
    repaint_screen(screen_update_ctx, screen_update_ctx->slice);
    screen_update_ctx->slice += 1;
    mu_sched_reschedule_now(sched);

  } else {
    // screen buffer has been repainted.  Trigger screen_redraw_task.
    screen_update_ctx->slice = 0;
    mu_sched_task_now(sched, screen_update_ctx->screen_redraw_task);
  }

  return NULL;
}

static void repaint_screen(screen_update_ctx_t *ctx, uint8_t slice) {

  if (slice == 0) {
    mu_substr_clear(ctx->screen_buffer);
    mu_substr_puts(ctx->screen_buffer,
                   CLEAR_SCREEN
                   "mu_thunk_demo " MU_THUNK_DEMO_VERSION
                   ": https://github.com/rdpoor/mulib\r\n\r\n"
                   "          Name Stat  # Calls     Runtime     Max Dur\r\n"
                   "+-------------+-+-----------+-----------+-----------+\r\n");

  } else if (slice <= ctx->n_tasks) {
    repaint_task(ctx, &ctx->tasks[slice - 1]);

  } else if (slice == ctx->n_tasks + 1) {
    mu_substr_puts(ctx->screen_buffer,
                   "\r\nStatus: A=Active, I=Idle, R=Runnable, S=Scheduled\r\n");

  } else if (slice == ctx->n_tasks + 2) {
    if (mu_thunk_demo_is_low_power_mode()) {
      mu_substr_puts(ctx->screen_buffer,
                     "Push user button to exit low-power mode.\r\n");
    } else {
      mu_substr_puts(ctx->screen_buffer,
                     "Type 'p' to enter low-power mode.\r\n"
                     "Type 'B' to suspend LED task. 'b' to restart.\r\n"
                     "Type 'D' to suspend Screen task. 'd' to restart.\r\n");
    }

  } else {
    // shouldn't arrive here...
    asm("nop");
  }
}

static void repaint_task(screen_update_ctx_t *ctx, mu_thunk_t *thunk) {
  mu_substr_printf(ctx->screen_buffer,
                   "%14s %c %11u %11lu %11lu\r\n",
                   mu_thunk_name(task),
                   get_task_state(task, ctx->sched),
                   mu_thunk_call_count(task),
                   task->runtime,
                   task->max_duration);
}

static char get_task_state(mu_thunk_t *thunk, mu_sched_t *sched) {
  switch (mu_sched_get_task_status(sched, task)) {
  case MU_SCHED_TASK_STATUS_IDLE:
    return 'I';
  case MU_SCHED_TASK_STATUS_RUNNABLE:
    return 'R';
  case MU_SCHED_TASK_STATUS_ACTIVE:
    return 'A';
  case MU_SCHED_TASK_STATUS_SCHEDULED:
    return 'S';
  default:
    return '?';
  }
}
