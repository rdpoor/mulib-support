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

#include "chron.h"
#include <stdio.h>

// =============================================================================
// private types and definitions

#if CHRON_CLOCK_NBITS == 32
typedef int32_t signed_duration_t;  // signed version of chron_duration_t
#endif

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

// =============================================================================
// public code

// Return t1+dt
chron_time_t chron_time_offset(chron_time_t t1, chron_duration_t dt) {
  return t1 + dt;
}

// Return t1-t2 as a duration
chron_duration_t chron_time_difference(chron_time_t t1, chron_time_t t2) {
  return t1 - t2;   // works due to twos compliment arithmetic
}

// Return true iff t1 is strictly earlier, equal to or
// later than t2.
bool chron_time_is_before(chron_time_t t1, chron_time_t t2) {
  return chron_time_difference(t1, t2) > CHRON_CLOCK_MAX_DURATION;
}

bool chron_time_is_equal(chron_time_t t1, chron_time_t t2) {
  return t1 == t2;
}

bool chron_time_is_after(chron_time_t t1, chron_time_t t2) {
  return chron_time_difference(t2, t1) > CHRON_CLOCK_MAX_DURATION;
}

chron_float_t chron_duration_to_seconds(chron_duration_t dt, chron_float_t clock_rate) {
  signed_duration_t sdt = (signed_duration_t)dt;  // convert to signed
  return sdt / clock_rate;
}

chron_duration_t chron_seconds_to_duration(chron_float_t s, chron_float_t clock_rate) {
  signed_duration_t sdt = s * clock_rate;
  return (chron_duration_t)sdt;            // convert to unsigned.
}
