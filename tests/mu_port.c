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

#include "mu_port.h"
#include <stdio.h>
#include <time.h>

// =============================================================================
// private types and definitions

const bool TIME_IS_SIGNED = (mu_port_time_t)-1 < 0;

// =============================================================================
// private declarations

static bool is_msb_set(mu_port_time_t t);

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_port_init() {
}

mu_port_time_t mu_port_time_offset(mu_port_time_t t, mu_port_time_dt dt) {
  return t + dt;
}

mu_port_time_dt mu_port_time_difference(mu_port_time_t t1, mu_port_time_t t2) {
  return t1 - t2;
}

bool mu_port_time_precedes(mu_port_time_t t1, mu_port_time_t t2) {
  return is_msb_set(t1 - t2);
}

bool mu_port_time_is_equal(mu_port_time_t t1, mu_port_time_t t2) {
  return t1 == t2;
}

bool mu_port_time_follows(mu_port_time_t t1, mu_port_time_t t2) {
  return is_msb_set(t2 - t1);
}

mu_port_time_dt mu_port_time_ms_to_duration(mu_port_time_ms_dt ms) {
  return mu_port_time_seconds_to_duration(ms * 1000);
}

mu_port_time_ms_dt mu_port_time_duration_to_ms(mu_port_time_dt dt) {
  return mu_port_time_duration_to_seconds(dt) / 1000;
}

mu_port_time_dt mu_port_time_seconds_to_duration(mu_port_time_seconds_dt seconds) {
  return seconds * CLOCKS_PER_SEC;
}

mu_port_time_seconds_dt mu_port_time_duration_to_seconds(mu_port_time_dt dt) {
  mu_port_time_seconds_dt secs = (mu_port_time_seconds_dt)dt / (mu_port_time_seconds_dt)CLOCKS_PER_SEC;
  return secs;
}

mu_port_time_t mu_port_rtc_now() {
  return clock();
}

// =============================================================================
// private code

static bool is_msb_set(mu_port_time_t t) {
  return TIME_IS_SIGNED ? (t < 0) : ((t << 1) >> 1) ^ t;
}
