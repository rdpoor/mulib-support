#include "../src/mulib_event.h"
#include "../src/mulib_task.h"
#include "test_utilities.h"
#include <stddef.h>
#include <stdio.h>

int s_task_i_called;
int s_task_t1_called;
int s_task_t2_called;

static void task_fn_i(void *d_arg, void *u_arg) {
  UTEST_ASSERT(d_arg == (void *)1);
  UTEST_ASSERT(u_arg == (void *)100);
  s_task_i_called += 1;
}

static void task_fn_t1(void *d_arg, void *u_arg) {
  UTEST_ASSERT(d_arg == (void *)2);
  UTEST_ASSERT(u_arg == (void *)200);
  s_task_t1_called += 1;
}

static void task_fn_t2(void *d_arg, void *u_arg) {
  UTEST_ASSERT(d_arg == (void *)3);
  UTEST_ASSERT(u_arg == (void *)300);
  s_task_t2_called += 1;
}

void mulib_event_test() {
  mulib_event_t event_i, event_t1, event_t2;
  port_time_t now = port_time_now();
  port_time_t then = port_time_offset(now, port_time_s_to_dt(5.0));

  mulib_event_init_immediate(&event_i, task_fn_i, (void *)100);
  mulib_event_init_timed(&event_t1, now, task_fn_t1, (void *)200);
  mulib_event_init_timed(&event_t2, then, task_fn_t2, (void *)300);

  UTEST_ASSERT(mulib_event_is_immediate(&event_i) == true);
  UTEST_ASSERT(mulib_event_is_immediate(&event_t1) == false);
  UTEST_ASSERT(mulib_event_is_immediate(&event_t2) == false);

  UTEST_ASSERT(mulib_event_time(&event_t1) == now);
  UTEST_ASSERT(mulib_event_time(&event_t2) == then);

  UTEST_ASSERT(mulib_event_is_runnable(&event_i) == true);
  UTEST_ASSERT(mulib_event_is_runnable(&event_t1) == true);
  // Beware: this test assumes less than 5 seconds have elapsed
  UTEST_ASSERT(mulib_event_is_runnable(&event_t2) == false);

  // Events can always be run even if not runnable
  mulib_event_run(&event_i, (void *)1);
  mulib_event_run(&event_t1, (void *)2);
  mulib_event_run(&event_t2, (void *)3);

  UTEST_ASSERT(s_task_i_called == 1);
  UTEST_ASSERT(s_task_t1_called == 1);
  UTEST_ASSERT(s_task_t2_called == 1);
}
