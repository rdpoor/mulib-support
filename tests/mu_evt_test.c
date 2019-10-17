#include "../src/mu_evt.h"
#include "../src/mu_task.h"
#include "test_utilities.h"
#include <stddef.h>
#include <stdio.h>

int s_msg_i_called;
int s_msg_t1_called;
int s_msg_t2_called;

static void msg_fn_imm(void *self, void *arg) {
  UTEST_ASSERT(arg == (void *)1);
  UTEST_ASSERT(self == (void *)100);
  s_msg_i_called += 1;
}

static void msg_fn_t1(void *self, void *arg) {
  UTEST_ASSERT(arg == (void *)2);
  UTEST_ASSERT(self == (void *)200);
  s_msg_t1_called += 1;
}

static void msg_fn_t2(void *self, void *arg) {
  UTEST_ASSERT(arg == (void *)3);
  UTEST_ASSERT(self == (void *)300);
  s_msg_t2_called += 1;
}

void mu_evt_test() {
  mu_evt_t event_i, event_t1, event_t2;
  mu_time_t now = mu_time_now();
  mu_time_t then = mu_time_offset(now, mu_time_seconds_to_duration(5.0));


#if (MU_TASK_PROFILING)
  mu_evt_init_immed(&event_i, msg_fn_imm, (void *)100, "Event I");
  mu_evt_init_at(&event_t1, now, msg_fn_t1, (void *)200, "Event T1");
  mu_evt_init_at(&event_t2, then, msg_fn_t2, (void *)300, "Event T2");
#else
  mu_evt_init_immed(&event_i, msg_fn_imm, (void *)100);
  mu_evt_init_at(&event_t1, now, msg_fn_t1, (void *)200);
  mu_evt_init_at(&event_t2, then, msg_fn_t2, (void *)300);
#endif

  UTEST_ASSERT(mu_evt_is_immediate(&event_i) == true);
  UTEST_ASSERT(mu_evt_is_immediate(&event_t1) == false);
  UTEST_ASSERT(mu_evt_is_immediate(&event_t2) == false);

  UTEST_ASSERT(mu_evt_time(&event_t1) == now);
  UTEST_ASSERT(mu_evt_time(&event_t2) == then);

  UTEST_ASSERT(mu_evt_is_runnable(&event_i, now) == true);
  UTEST_ASSERT(mu_evt_is_runnable(&event_t1, now) == true);
  UTEST_ASSERT(mu_evt_is_runnable(&event_t2, now) == false);

  // Events can always be run even if time has not yet arrived
  mu_evt_call(&event_i, (void *)1);
  mu_evt_call(&event_t1, (void *)2);
  mu_evt_call(&event_t2, (void *)3);

  UTEST_ASSERT(s_msg_i_called == 1);
  UTEST_ASSERT(s_msg_t1_called == 1);
  UTEST_ASSERT(s_msg_t2_called == 1);

  UTEST_ASSERTEQ_BOOL(mu_evt_is_after(&event_i, &event_t1), false);
  UTEST_ASSERTEQ_BOOL(mu_evt_is_after(&event_t1, &event_i), true);

  UTEST_ASSERTEQ_BOOL(mu_evt_is_after(&event_t1, &event_t2), false);
  UTEST_ASSERTEQ_BOOL(mu_evt_is_after(&event_t2, &event_t1), true);

  UTEST_ASSERTEQ_BOOL(mu_evt_is_after(&event_i, &event_i), false);
  UTEST_ASSERTEQ_BOOL(mu_evt_is_after(&event_t1, &event_t1), false);
}
