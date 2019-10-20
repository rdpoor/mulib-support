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

#include "mu_task.h"
#include "test_utilities.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int called;
} task_state_t;

task_state_t s_t1_self;

void t1_fn(void *self, void *arg) {
  // on POSIX, can't use sleep or usleep since the cycle timer stops as well.
  for (int i = 0; i<100000; i++) {
    asm("nop");
  }
  task_state_t *s = (task_state_t *)self;
  s->called += 1;
  UTEST_ASSERTEQ_PTR(arg, self);
}

void mu_task_test() {
  mu_task_t ti1;
  mu_task_t *t1 = &ti1;
  mu_task_t *t2;
  char buf[20];

  t2 = mu_task_init(t1, t1_fn, &s_t1_self, "Task1");

#if (MU_TASK_PROFILING)
  UTEST_ASSERTEQ_STR(mu_task_name(t1), "Task1");
  UTEST_ASSERTEQ_INT(mu_task_call_count(t1), 0);
  UTEST_ASSERT(mu_task_runtime(t1) == 0.0);
  mu_task_print(t1, buf, 20);
  UTEST_ASSERT(strlen(buf) > 0);
#endif

  UTEST_ASSERTEQ_PTR(t1, t2);
  for (int i=0; i<10; i++) {
    mu_task_call(t1, &s_t1_self);  // for testing assure arg == self
  }

#if (MU_TASK_PROFILING)
  UTEST_ASSERTEQ_INT(mu_task_call_count(t1), 10);
  // not working under POSIX
  // UTEST_ASSERT(mu_task_runtime(t1) > 0.0);
#endif
}
