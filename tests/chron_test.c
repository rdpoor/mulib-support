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

#include "../src/chron.h"
#include "test_utilities.h"
#include <stdio.h>

#define CLOCK_HZ 1234

#define CHRON_CLOCK_EPS (0.5 / (float)CHRON_CLOCK_MAX_DURATION)

void chron_test() {
  chron_time_t t1 = 0x1;
  chron_time_t t2 = (chron_time_t)(-1);
  chron_duration_t d1 = 2;
  chron_duration_t d2 = -2;

  // chron_time_t chron_rtc_offset(chron_time_t t1, chron_duration_t dt);
  UTEST_ASSERT(chron_time_offset(t1, d1) == 3);
  UTEST_ASSERT(chron_time_offset(t1, d2) == t2);

  // chron_duration_t chron_rtc_difference(chron_time_t t1, chron_time_t t2);
  UTEST_ASSERT(chron_time_difference(t1, t2) == 2);
  UTEST_ASSERT(chron_time_difference(t2, t1) == d2);

  // bool chron_rtc_is_before(chron_time_t t1, chron_time_t t2);
  UTEST_ASSERT(chron_time_is_before(t2, t1) == true);
  UTEST_ASSERT(chron_time_is_before(t1, t1) == false);
  UTEST_ASSERT(chron_time_is_before(t1, t2) == false);

  // bool chron_rtc_is_equal(chron_time_t t1, chron_time_t t2);
  UTEST_ASSERT(chron_time_is_equal(t1, t1) == true);
  UTEST_ASSERT(chron_time_is_equal(t1, t2) == false);

  // bool chron_rtc_is_after(chron_time_t t1, chron_time_t t2);
  UTEST_ASSERT(chron_time_is_after(t2, t1) == false);
  UTEST_ASSERT(chron_time_is_after(t1, t1) == false);
  UTEST_ASSERT(chron_time_is_after(t1, t2) == true);

  UTEST_ASSERT(chron_duration_to_seconds(0xffffffff, 1.0) == -1.0);
  UTEST_ASSERT(chron_duration_to_seconds(0, 1.0) == 0.0);
  UTEST_ASSERT(chron_duration_to_seconds(1, 1.0) == 1.0);
  UTEST_ASSERT(chron_duration_to_seconds(0x7fffffff, 1.0) == CHRON_CLOCK_MAX_DURATION-1);

  UTEST_FLOAT_EPS(chron_duration_to_seconds(0xffffffff, CLOCK_HZ),-1.0/CLOCK_HZ, 0.001);
  UTEST_FLOAT_EPS(chron_duration_to_seconds(0, CLOCK_HZ), 0.0, 0.001);
  UTEST_FLOAT_EPS(chron_duration_to_seconds(1, CLOCK_HZ), 1.0/CLOCK_HZ, 0.001);
  UTEST_FLOAT_EPS(chron_duration_to_seconds(0x7fffffff, CLOCK_HZ),
                  (((chron_float_t)CHRON_CLOCK_MAX_DURATION)-1.0) / CLOCK_HZ,
                  0.001);

  // chron_duration_t chron_rtc_s_to_dt(float s);
  UTEST_ASSERT(chron_seconds_to_duration(2.0, CLOCK_HZ) == (chron_duration_t)((int)(2.0 * CLOCK_HZ)));
  UTEST_ASSERT(chron_seconds_to_duration(-2.0, CLOCK_HZ) == (chron_duration_t)((int)(-2.0 * CLOCK_HZ)));

}
