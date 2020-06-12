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

int mu_buf_test();
int mu_bufref_test();
int mu_bitvec_test();
int mu_spscq_test();
int mu_list_test();
int mu_pstore_test();
int mu_queue_test();
int mu_task_test();
int mu_time_test();

// =============================================================================
// public code

int main() {

  mu_test_init();
  printf("\r\nstarting mu_test...");

  mu_buf_test();
  mu_bufref_test();
  mu_bitvec_test();
  mu_spscq_test();
  mu_list_test();
  mu_pstore_test();
  mu_queue_test();
  mu_task_test();
  mu_time_test();

  printf("\r\nending mu_test: %d error%s out of %d test%s\r\n",
         mu_test_error_count(),
         mu_test_error_count() == 1 ? "" : "s",
         mu_test_count(),
         mu_test_count() == 1 ? "" : "s");

  return mu_test_error_count();
}

// =============================================================================
// private code
