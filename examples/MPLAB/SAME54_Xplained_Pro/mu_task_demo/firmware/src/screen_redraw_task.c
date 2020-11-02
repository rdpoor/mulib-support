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
#include "mu_thunk_demo.h"
#include <definitions.h>
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// character sequence to home cursor and clear to end of screen
#define CLEAR_SCREEN "\e[1;1H\e[2J"

// =============================================================================
// local (forward) declarations

static void *screen_redraw_task_fn(void *ctx, void *arg);
static void print_task(mu_thunk_t *thunk, mu_sched_t *sched);
static char get_task_state(mu_thunk_t *thunk, mu_sched_t *sched);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_thunk_t *screen_redraw_task_init(mu_thunk_t *screen_redraw_task,
                                   screen_redraw_ctx_t *screen_redraw_ctx,
                                   mu_thunk_t *thunks,
                                   size_t n_tasks) {
  screen_redraw_ctx->s1 = 0;
  screen_redraw_ctx->s2 = 0;
  screen_redraw_ctx->tasks = tasks;
  screen_redraw_ctx->n_tasks = n_tasks;

  mu_thunk_init(screen_redraw_task,
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
 * Note: Each status line is approximately 60 characters long, and there are
 * 11 lines to display.  If the entire display were output at once, the function
 * would require (60*11)/115200 = 57 milliseconds (assuming 115 KBaud, 8n1).
 * To shorten the latency, we only print one line at a time before returing to
 * the scheduler, so the function consumes at most about 5.2 milliseconds.
 */
static void *screen_redraw_task_fn(void *ctx, void *arg) {
  // screen_redraw_context is passed as the first argument, scheduler is second
  screen_redraw_ctx_t *screen_redraw_ctx = (screen_redraw_ctx_t *)ctx;
  mu_sched_t *sched = (mu_sched_t *)arg;
  bool done = false;

  switch (screen_redraw_ctx->s1) {
    case 0:
    printf("%s", CLEAR_SCREEN);
    screen_redraw_ctx->s1 += 1;
    screen_redraw_ctx->s2 = 0;
    break;

    case 1:
    printf("mu_thunk_demo %s: https://github.com/rdpoor/mulib\r\n\r\n",
            MU_THUNK_DEMO_VERSION);
    screen_redraw_ctx->s1 += 1;
    break;

    case 2:
    printf("          Name Stat  # Calls     Runtime     Max Dur\r\n");
    screen_redraw_ctx->s1 += 1;
    break;

    case 3:
    printf("+-------------+-+-----------+-----------+-----------+\r\n");
    screen_redraw_ctx->s1 += 1;
    break;

    case 4:
    print_task(&(screen_redraw_ctx->tasks[screen_redraw_ctx->s2++]), sched);
    // stay in this major state until all tasks have been displayed
    if (screen_redraw_ctx->s2 == screen_redraw_ctx->n_tasks) {
      screen_redraw_ctx->s1 += 1;
    }
    break;

    case 5:
    printf("\r\nStatus: A=Active, R=Runnable, S=Scheduled, I=Idle\r\n");
    screen_redraw_ctx->s1 += 1;
    break;

    case 6:
    if (mu_thunk_demo_is_low_power_mode()) {
      printf("Push user button to exit low-power mode.\r\n");
      done = true;
    } else {
      printf("Type 'p' to enter low-power mode.\r\n");
      screen_redraw_ctx->s1 += 1;
    }
    break;

    case 7:
    printf("Type 'b' to restart LED task. 'B' to suspend.\r\n");
    screen_redraw_ctx->s1 += 1;
    break;

    case 8:
    printf("Type 'd' to restart Screen task. 'D' to suspend.\r\n");
    done = true;
    break;
  }

  // endgame: if done, reset state variables and wait for screen_update_task
  // to restart this redraw task.  Otherwise reschedule right away.
  if (done) {
    screen_redraw_ctx->s1 = 0;
    screen_redraw_ctx->s2 = 0;
  } else {
    mu_sched_reschedule_now(sched);
  }

  return NULL;
}

static void print_task(mu_thunk_t *thunk, mu_sched_t *sched) {
  printf("%14s %c %11u %11lu %11lu\r\n",
         mu_thunk_name(task),
         get_task_state(task, sched),
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
