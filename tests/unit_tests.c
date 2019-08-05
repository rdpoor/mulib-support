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

#include "unit_tests.h"
#include <stdio.h>

void bcast_test();
void chron_test();
void heapsort_test();
void jemi_test();
void mulog_test();
void pool_test();
void sched_test();

void unit_tests() {

  printf("\r\n==== Starting Unit Tests:");

  printf("\r\n     starting bcast_test()...");
  bcast_test();
  printf("completed");

  printf("\r\n     starting chron_test()...");
  chron_test();
  printf("completed");

  printf("\r\n     starting heapsort_test()...");
  heapsort_test();
  printf("completed");

  printf("\r\n     starting jemi_test()...");
  jemi_test();
  printf("completed");

  printf("\r\n     starting mulog_test()...");
  mulog_test();
  printf("completed");

  printf("\r\n     starting pool_test()...");
  pool_test();
  printf("completed");

  printf("\r\n     starting sched_test()...");
  sched_test();
  printf("completed");

  printf("\r\n==== Completed Unit Tests\r\n");

}
