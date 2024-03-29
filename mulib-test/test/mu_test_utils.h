/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

#ifndef _MU_TEST_UTILS_H_
#define _MU_TEST_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

 // =============================================================================
 // includes

#include <stdbool.h>
#include "mu_config.h"


// =============================================================================
// types and definitions

#ifndef ASSERT
#define ASSERT(expr) mu_test_assert((expr), #expr, __FILE__, __LINE__)
#endif

// =============================================================================
// declarations

void mu_test_init(void);
int mu_test_count(void);
int mu_test_error_count(void);

void mu_test_assert(const bool condition,
                    const char *const expr,
                    const char *const file,
                    const int line);

// =============================================================================
// local storage

// =============================================================================
// public code

#ifdef __cplusplus
}
#endif

#endif // _MU_TEST_UTILS_H_
