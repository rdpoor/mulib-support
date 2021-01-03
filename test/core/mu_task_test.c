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

// =============================================================================
// includes

#include "mu_time.h"
#include "mu_task.h"
#include "mu_test_utils.h"
#include <string.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

void *task_fn1(void *self, void *arg) {
  return self;
}

void *task_fn2(void *self, void *arg) {
  return arg;
}

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_task_test() {
  mu_task_t t1;
  mu_task_t t2;

  ASSERT(mu_task_init(&t1, task_fn1, &t1, "Task1") == &t1);
#if (MU_TASK_PROFILING)
  ASSERT(strcmp(mu_task_name(&t1), "Task1") == 0);
#endif
  ASSERT(mu_task_init(&t2, task_fn2, NULL, "Task2") == &t2);

  ASSERT(mu_task_call(&t1, &t2) == &t1);  // task_fn1 returns self (&t1)
  ASSERT(mu_task_call(&t2, &t1) == &t1);  // task_fn2 return arg (&t1)

#if (MU_TASK_PROFILING)
  ASSERT((&t1) == 1);
  ASSERT(mu_task_call_count(&t2) == 1);
#endif

}

// =============================================================================
// private code
