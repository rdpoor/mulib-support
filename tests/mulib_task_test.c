#include "../src/mulib_task.h"
#include "test_utilities.h"
#include <stddef.h>
#include <stdio.h>

int s_task_1_called;
int s_task_2_called;

static void test_fn_1(void *d_arg, void *u_arg) {
  UTEST_ASSERT(d_arg == (void *)1);
  UTEST_ASSERT(u_arg == (void *)100);
  s_task_1_called += 1;
}

static void test_fn_2(void *d_arg, void *u_arg) {
  UTEST_ASSERT(d_arg == (void *)2);
  UTEST_ASSERT(u_arg == (void *)200);
  s_task_2_called += 1;
}

void mulib_task_test() {
  mulib_task_t task1, task2;

  mulib_task_init(&task1, test_fn_1, (void *)100);
  mulib_task_init(&task2, test_fn_2, (void *)200);

  mulib_task_run(&task1, (void *)1);
  mulib_task_run(&task2, (void *)2);

  UTEST_ASSERT(s_task_1_called == 1);
  UTEST_ASSERT(s_task_2_called == 1);

  // should not error
  mulib_task_run(NULL, (void *)3);
}
