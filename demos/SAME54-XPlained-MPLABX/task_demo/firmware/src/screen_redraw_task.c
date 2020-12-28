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

#include "screen_redraw_task.h"
#include "definitions.h"
#include "mu_sched.h"
#include "mu_task.h"
#include "mu_time.h"
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

// character sequence to home cursor and clear to end of screen
#define CLEAR_SCREEN "\e[1;1H\e[2J"

typedef struct {
  mu_task_t **tasks;
  int task_count;
  int state;            // selects which line to print
} redraw_ctx_t;

// =============================================================================
// local (forward) declarations

static void *redraw_task_fn(void *ctx, void *arg);
static void print_task_status(mu_task_t *task);

// =============================================================================
// local storage

static mu_task_t s_redraw_task;
static redraw_ctx_t s_redraw_ctx;

// =============================================================================
// public code

mu_task_t *screen_redraw_task_init(mu_task_t *tasks[], int task_count) {
  s_redraw_ctx.tasks = tasks;
  s_redraw_ctx.task_count = task_count;
  s_redraw_ctx.state = 0;

  // initialize the screen redraw task
  mu_task_init(&s_redraw_task, redraw_task_fn, &s_redraw_ctx, "Screen Redraw");
  return &s_redraw_task;
}

// =============================================================================
// local (static) code

// Called when the task triggers
static void *redraw_task_fn(void *ctx, void *arg) {
  // 'self' context is passed as first arg, scheduler is passed as the second.
  redraw_ctx_t *redraw_ctx = (redraw_ctx_t *)ctx;
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_task_t *task = mu_sched_get_current_task(sched);

  int state = redraw_ctx->state;

  if (state == 0) {
    printf("%s", CLEAR_SCREEN);
  } else if (state == 1) {
    printf("mulib task_demo v %s\n",  "1.0.0");
  } else if (state == 2) {
    printf("    n_calls     runtime max_dur task name\n");
  } else if (state == 3) {
    printf("+----------+-----------+-------+---------\n");
  } else if (state < 4 + redraw_ctx->task_count) {
    print_task_status(redraw_ctx->tasks[redraw_ctx->state-4]);
    redraw_ctx->state += 1;
  }

  if (state < 4 + redraw_ctx->task_count) {
    // bump state and reschedule
    redraw_ctx->state = state + 1;
    mu_sched_task_now(sched, task);

  } else {
    // reset state and wait for screen_update_task to restart this task.
    redraw_ctx->state = 0;
  }

  return NULL;
}

static void print_task_status(mu_task_t *task) {
  printf("%11d %11ld %7ld %s\n",
         task->call_count,
         task->runtime,
         task->max_duration,
         task->name);
}
