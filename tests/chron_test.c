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

#define CHRON_CLOCK_HZ 1234.0

void chron_test() {
  chron_time_t t1 = 0x1;
  chron_time_t t2 = 0xffffffff;
  chron_duration_t d1 = 2;
  chron_duration_t d2 = -2;

  // chron_time_t chron_rtc_offset(chron_time_t t1, chron_duration_t dt);
  UNIT_TEST_ASSERT(chron_time_offset(t1, d1) == 3);
  UNIT_TEST_ASSERT(chron_time_offset(t1, d2) == t2);

  // chron_duration_t chron_rtc_difference(chron_time_t t1, chron_time_t t2);
  UNIT_TEST_ASSERT(chron_time_difference(t1, t2) == 2);
  UNIT_TEST_ASSERT(chron_time_difference(t2, t1) == d2);

  // bool chron_rtc_is_before(chron_time_t t1, chron_time_t t2);
  UNIT_TEST_ASSERT(chron_time_is_before(t2, t1) == true);
  UNIT_TEST_ASSERT(chron_time_is_before(t1, t1) == false);
  UNIT_TEST_ASSERT(chron_time_is_before(t1, t2) == false);

  // bool chron_rtc_is_equal(chron_time_t t1, chron_time_t t2);
  UNIT_TEST_ASSERT(chron_time_is_equal(t1, t1) == true);
  UNIT_TEST_ASSERT(chron_time_is_equal(t1, t2) == false);

  // bool chron_rtc_is_after(chron_time_t t1, chron_time_t t2);
  UNIT_TEST_ASSERT(chron_time_is_after(t2, t1) == false);
  UNIT_TEST_ASSERT(chron_time_is_after(t1, t1) == false);
  UNIT_TEST_ASSERT(chron_time_is_after(t1, t2) == true);

  chron_set_clock_rate(CHRON_CLOCK_HZ);
  UNIT_TEST_ASSERT(chron_get_clock_rate() == CHRON_CLOCK_HZ);

  // float chron_duration_t_to_s(chron_duration_t dt);
  UNIT_TEST_ASSERT(chron_duration_to_seconds(CHRON_CLOCK_HZ) == 1.0);
  UNIT_TEST_ASSERT(chron_duration_to_seconds(-CHRON_CLOCK_HZ) == -1.0);

  // chron_duration_t chron_rtc_s_to_dt(float s);
  UNIT_TEST_ASSERT(chron_seconds_to_duration(2.0) == 2.0 * CHRON_CLOCK_HZ);
  UNIT_TEST_ASSERT(chron_seconds_to_duration(-2.0) == -2.0 * CHRON_CLOCK_HZ);

}
