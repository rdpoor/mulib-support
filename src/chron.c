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

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

// =============================================================================
// local storage

static float s_clock_rate = CHRON_DEFAULT_CLOCK_RATE;

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
  return chron_time_difference(t1, t2) > MAX_RTC_DURATION;
}

bool chron_time_is_equal(chron_time_t t1, chron_time_t t2) {
  return t1 == t2;
}

bool chron_time_is_after(chron_time_t t1, chron_time_t t2) {
  return chron_time_difference(t2, t1) > MAX_RTC_DURATION;
}

void chron_set_clock_rate(float tics_per_second) {
  s_clock_rate = tics_per_second;
}

float chron_get_clock_rate() {
  return s_clock_rate;
}

float chron_duration_to_seconds(chron_duration_t dt) {
    return dt / s_clock_rate;
}

chron_duration_t chron_seconds_to_duration(float s) {
    return s * s_clock_rate;
}


// =============================================================================
// private code
