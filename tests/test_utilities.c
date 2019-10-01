/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor
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

#include "test_utilities.h"
#include <stdio.h>
#include <stdint.h>

/**
 * \brief Assert function
 */
void unit_test_assert(const bool condition,
                      const char *const expr,
                      const char *const file,
                      const int line) {
  if (!condition) {
    printf("\r\nAssertion '%s' failed at %s:%d", expr, file, line);
    fflush(stdout);
#ifdef UNIT_TEST_BREAK_ON_ERROR
    __asm("BKPT #0");
#endif
  }
}

/**
 * @brief pass the test if f0 and f1 differ by less than eps
 */
void unit_test_float_eps(const float f0,
                         const float f1,
                         const float eps,
                         const char *const f0_expr,
                         const char *const f1_expr,
                         const char *const eps_expr,
                         const char *const file,
                         const int line) {
  float diff = f0-f1;
  if (diff < 0) diff = -diff;
  if (diff >= eps) {
    printf("\r\n%e and %e differ by more than %e in UTEST_FLOAT_EPS(%s, %s, %s) at %s:%d",
           f0,
           f1,
           eps,
           f0_expr,
           f1_expr,
           eps_expr,
           file,
           line);
    fflush(stdout);
#ifdef UNIT_TEST_BREAK_ON_ERROR
  __asm("BKPT #0");
#endif
  }
}

void unit_test_assert_eq_bool(const bool observed,
                              const bool expected,
                              const char *observed_expr,
                              const char *expected_expr,
                              const char *const file,
                              const int line) {
  if (observed != expected) {
    printf("\r\n%d != %d in %s == %s at %s:%d", observed, expected, observed_expr, expected_expr, file, line);
    fflush(stdout);
#ifdef UNIT_TEST_BREAK_ON_ERROR
    __asm("BKPT #0");
#endif
  }
}

void unit_test_assert_eq_int(const int observed,
                             const int expected,
                             const char *observed_expr,
                             const char *expected_expr,
                             const char *const file,
                             const int line) {
  if (observed != expected) {
    printf("\r\n%d != %d in %s == %s at %s:%d", observed, expected, observed_expr, expected_expr, file, line);
    fflush(stdout);
#ifdef UNIT_TEST_BREAK_ON_ERROR
    __asm("BKPT #0");
#endif
  }
}

void unit_test_assert_eq_ptr(const void *observed,
                             const void *expected,
                             const char *observed_expr,
                             const char *expected_expr,
                             const char *const file,
                             const int line) {
  if (observed != expected) {
    printf("\r\n%p != %p in %s == %s at %s:%d", observed, expected, observed_expr, expected_expr, file, line);
    fflush(stdout);
#ifdef UNIT_TEST_BREAK_ON_ERROR
    __asm("BKPT #0");
#endif
  }
}
