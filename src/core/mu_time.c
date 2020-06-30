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
#include "mu_port.h"

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_time_init(void) {
  // mu_port_init() must be called separately
}

mu_time_t mu_time_offset(mu_time_t t1, mu_time_dt dt) {
  return mu_port_time_offset(t1, dt);
}

mu_time_dt mu_time_difference(mu_time_t t1, mu_time_t t2) {
  return mu_port_time_difference(t1, t2);
}

bool mu_time_precedes(mu_time_t t1, mu_time_t t2) {
  return mu_port_time_precedes(t1, t2);
}

bool mu_time_is_equal(mu_time_t t1, mu_time_t t2) {
  return mu_port_time_is_equal(t1, t2);
}

bool mu_time_follows(mu_time_t t1, mu_time_t t2) {
  return mu_port_time_precedes(t2, t1);
}

mu_time_ms_dt mu_time_duration_to_ms(mu_time_dt dt) {
  return mu_port_time_duration_to_ms(dt);
}

mu_time_dt mu_time_ms_to_duration(mu_time_ms_dt ms) {
  return mu_port_time_ms_to_duration(ms);
}

mu_time_seconds_dt mu_time_duration_to_seconds(mu_time_dt dt) {
  return mu_port_time_duration_to_seconds(dt);
}

mu_time_dt mu_time_seconds_to_duration(mu_time_seconds_dt s) {
  return mu_port_time_seconds_to_duration(s);
}

mu_time_t mu_time_now() {
  return mu_port_time_now();
}
