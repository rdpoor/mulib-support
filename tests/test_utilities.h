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

#ifndef TEST_UTILITIES_H_
#define TEST_UTILITIES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define UNUSED(x) (void)(x)

#define UTEST_ASSERT(condition) unit_test_assert((condition), #condition, __FILE__, __LINE__)
void unit_test_assert(const bool condition,
                      const char *const expr,
                      const char *const file,
                      const int line);

#define UTEST_FLOAT_EPS(f0, f1, eps) unit_test_float_eps((f0), (f1), (eps), #f0, #f1, #eps, __FILE__, __LINE__)
void unit_test_float_eps(const float f0,
                         const float f1,
                         const float eps,
                         const char *const f0_expr,
                         const char *const f1_expr,
                         const char *const eps_expr,
                         const char *const file,
                         const int line);

#define UTEST_ASSERTEQ_BOOL(observed, expected) \
  unit_test_assert_eq_bool((observed), (expected), #observed, #expected, __FILE__, __LINE__)
void unit_test_assert_eq_bool(const bool observed,
                              const bool expected,
                              const char *observed_expr,
                              const char *expected_expr,
                              const char *const file,
                              const int line);

#define UTEST_ASSERTEQ_INT(observed, expected) \
  unit_test_assert_eq_int((observed), (expected), #observed, #expected, __FILE__, __LINE__)
void unit_test_assert_eq_int(const int observed,
                             const int expected,
                             const char *observed_expr,
                             const char *expected_expr,
                             const char *const file,
                             const int line);

#define UTEST_ASSERTEQ_PTR(observed, expected) \
  unit_test_assert_eq_ptr((observed), (expected), #observed, #expected, __FILE__, __LINE__)
void unit_test_assert_eq_ptr(const void *observed,
                             const void *expected,
                             const char *observed_expr,
                             const char *expected_expr,
                             const char *const file,
                             const int line);


#ifdef __cplusplus
}
#endif
#endif // TEST_UTILITIES_H_
