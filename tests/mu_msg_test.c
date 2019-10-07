#include "../src/mu_task.h"
#include "test_utilities.h"
#include <stddef.h>
#include <stdio.h>

int s_msg_1_called;
int s_msg_2_called;

static void test_fn_1(void *self, void *arg) {
  UTEST_ASSERT(self == (void *)100);
  UTEST_ASSERT(arg == (void *)1);
  s_msg_1_called += 1;
}

static void test_fn_2(void *self, void *arg) {
  UTEST_ASSERT(self == (void *)200);
  UTEST_ASSERT(arg == (void *)2);
  s_msg_2_called += 1;
}

void mu_task_test() {
  mu_task_t msg1, msg2;

  mu_task_init(&msg1, test_fn_1, (void *)100);
  mu_task_init(&msg2, test_fn_2, (void *)200);

  mu_task_call(&msg1, (void *)1);
  mu_task_call(&msg2, (void *)2);

  UTEST_ASSERT(s_msg_1_called == 1);
  UTEST_ASSERT(s_msg_2_called == 1);

  // should not error
  mu_task_call(NULL, (void *)3);
}
