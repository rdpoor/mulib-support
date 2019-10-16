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

#include "mu_time.h"
#include "port.h"
#include <stdio.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

mu_time_t mu_time_now() {
  return port_time_now();
}

mu_time_t mu_time_offset(mu_time_t t1, mu_time_dt dt) {
  return port_time_offset(t1, dt);
}

mu_time_dt mu_time_difference(mu_time_t t1, mu_time_t t2) {
  return port_time_difference(t1, t2);
}

// Return true iff t1 is strictly earlier, equal to or
// later than t2.
bool mu_time_is_before(mu_time_t t1, mu_time_t t2) {
  return port_time_is_before(t1, t2);
}

bool mu_time_is_equal(mu_time_t t1, mu_time_t t2) {
  return port_time_is_equal(t1, t2);
}

bool mu_time_is_after(mu_time_t t1, mu_time_t t2) {
  return port_time_is_before(t2, t1);
}

mu_time_seconds_t mu_time_duration_to_seconds(mu_time_dt dt) {
  return port_time_duration_to_seconds(dt);
}

mu_time_dt mu_time_seconds_to_duration(mu_time_seconds_t s) {
  return port_time_seconds_to_duration(s);
}
