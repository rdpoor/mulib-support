#include "test_utilities.h"

void test_utilities_test() {
  int a, b;
  void *x, *y;

  printf("\r\nNote: some of these tests are intended to fail");
  UTEST_ASSERTEQ_BOOL(1 == 1, true);
  UTEST_ASSERTEQ_BOOL(1 == 1, false);
  UTEST_ASSERTEQ_BOOL(1 == 2, false);
  UTEST_ASSERTEQ_BOOL(1 == 2, true);

  a = 1;
  b = 2;
  UTEST_ASSERTEQ_INT(a, a);
  UTEST_ASSERTEQ_INT(a, b);

  x = (void *)100;
  y = (void *)200;
  UTEST_ASSERTEQ_PTR(x, x);
  UTEST_ASSERTEQ_PTR(x, y);
  printf("\r\n...");
}
