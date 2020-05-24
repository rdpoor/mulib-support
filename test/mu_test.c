/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
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

 // =============================================================================
 // includes

#include <stdio.h>
#include "mu_test_utils.h"

// =============================================================================
// types and definitions

// =============================================================================
// declarations

extern int error_count;  // in mu_test_utils

int mu_bufx_test();
int mu_buf_test();

// =============================================================================
// public code

int main() {

  printf("\r\nstarting mu_test...");

  mu_test_init();
  mu_bufx_test();
  mu_buf_test();

  printf("\r\nending mu_test: %d error%s out of %d test%s\r\n",
         mu_test_error_count(),
         mu_test_error_count() == 1 ? "" : "s",
         mu_test_count(),
         mu_test_count() == 1 ? "" : "s");

  return mu_test_error_count();
}

// =============================================================================
// private code