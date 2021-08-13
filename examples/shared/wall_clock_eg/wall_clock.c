/*
 * @file wall_clock.c
 *
 *  Created on: Apr 18, 2021
 *      Author: andy
 */

// =============================================================================
// Includes

#include "wall_clock.h"
#include "ansi_big_font.h"
#include "ansi_nico_font.h"
#include "fb.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mulib.h"


// =============================================================================
// Local types and definitions

#define CLOCK_POLL_INTERVAL_MS (200)

#define TERM_WIDTH (255)
#define TERM_HEIGHT (25)


typedef struct {
  mu_task_t task;
  unsigned char key_char;
} clock_poll_ctx_t;

// =============================================================================
// Local (forward) declarations

static void clock_poll_fn(void *ctx, void *arg);

static char *local_time_string(void);

static void begin_polling_clock(void);


// =============================================================================
// Local storage

static clock_poll_ctx_t clock_poll_ctx;

static char _local_time_string[16];

static char s_backing_buf[TERM_WIDTH * TERM_HEIGHT];
static char s_display_buf[TERM_WIDTH * TERM_HEIGHT];

// =============================================================================
// Public code

void wall_clock_init(void) {
  // a l carte means we are responsible for calling these inits
  mu_rtc_init();
  mu_sched_init();
  mu_ansi_term_init();
  mu_ansi_term_set_cursor_visible(false);
  mu_ansi_term_clear_screen();
  fb_init(TERM_WIDTH, TERM_HEIGHT, s_backing_buf, s_display_buf);
  begin_polling_clock();
}

void wall_clock_step(void) {
  mu_sched_step();
}


// =============================================================================
// Local (private) code

// non-POSIX systems will need to use mu_rtc_now() / 1000 instead of time(&now)
static char *local_time_string() {
  struct tm  ts;
  time_t now;

  time(&now);
  //now = mu_rtc_now() / 1000;

  ts = *localtime(&now);  // Fill in the tm structure
  strftime(_local_time_string, sizeof(_local_time_string), "%I:%M:%S", &ts); // %H for 24H
  return _local_time_string;
}

static void clock_poll_fn(void *ctx, void *arg) {
  (void)ctx;  // unused
  (void)arg;  // unused

  //mu_ansi_term_clear_screen();
  //mu_ansi_term_set_cursor_position(0,0);
  print_string_using_nico_font(local_time_string());
  mu_duration_t delay = MU_TIME_MS_TO_DURATION(CLOCK_POLL_INTERVAL_MS);
  mu_sched_task_in(&clock_poll_ctx.task, delay);
}

static void begin_polling_clock() {
  mu_task_init(&clock_poll_ctx.task, clock_poll_fn, &clock_poll_ctx, "clock_poll");
  mu_sched_task_now(&clock_poll_ctx.task);
}
