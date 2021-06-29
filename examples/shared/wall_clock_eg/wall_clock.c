/*
 * platform_test.c
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 */

// =============================================================================
// Includes

#include "wall_clock.h"
#include "ansi_big_font.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


// here we include mu modules a la carte 
#include "mu_rtc.h"
#include "mu_ansi_term.h"
//#include "mu_sched.h"


// =============================================================================
// Local types and definitions
#define CLOCK_POLL_INTERVAL_MS (200)

// =============================================================================
// Local (forward) declarations

void mu_begin_polling_clock();
static void clock_poll_fn(void *ctx, void *arg);

typedef struct {
  mu_task_t task;
  unsigned char key_char;
} clock_poll_ctx_t;

static clock_poll_ctx_t clock_poll_ctx;

// =============================================================================
// Public code

void wall_clock_init(void) {
  mu_time_init();
  mu_sched_init();
  mu_ansi_term_clear_screen();
  mu_begin_polling_clock();
}

void wall_clock_step(void) {
  mu_sched_step();
}
  
 static char _local_time_string[16];

char *local_time_string() {
 struct tm  ts;
  time_t now;

  time(&now);
  //now = mu_rtc_now() / 1000;

  ts = *localtime(&now);  // Fill in the tm structure
  strftime(_local_time_string, sizeof(_local_time_string), "%I:%M:%S", &ts); // %H for 24H
  return _local_time_string;
}

void mu_begin_polling_clock() {
  mu_task_init(&clock_poll_ctx.task, clock_poll_fn, &clock_poll_ctx, "clock_poll");
  mu_sched_task_now(&clock_poll_ctx.task);
}

static void clock_poll_fn(void *ctx, void *arg) {
  (void)ctx;  // unused
  (void)arg;  // unused
 
  mu_ansi_term_clear_screen();
  mu_ansi_term_set_cursor_position(0,0);
  print_string_using_big_font(local_time_string());
  mu_duration_t delay = MU_TIME_MS_TO_DURATION(CLOCK_POLL_INTERVAL_MS);
  mu_sched_task_in(&clock_poll_ctx.task, delay);
}


// =============================================================================
// Local (private) code

