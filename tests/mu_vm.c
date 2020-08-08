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

#include "mu_vm.h"
#include <stdio.h>
#include <time.h>

// =============================================================================
// private types and definitions

const bool TIME_IS_SIGNED = (mu_vm_time_t)-1 < 0;

// =============================================================================
// private declarations

static bool is_msb_set(mu_vm_time_t t);

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_vm_init() {
}

mu_vm_time_t mu_vm_time_offset(mu_vm_time_t t, mu_vm_time_dt dt) {
  return t + dt;
}

mu_vm_time_dt mu_vm_time_difference(mu_vm_time_t t1, mu_vm_time_t t2) {
  return t1 - t2;
}

bool mu_vm_time_precedes(mu_vm_time_t t1, mu_vm_time_t t2) {
  return is_msb_set(t1 - t2);
}

bool mu_vm_time_equals(mu_vm_time_t t1, mu_vm_time_t t2) {
  return t1 == t2;
}

bool mu_vm_time_follows(mu_vm_time_t t1, mu_vm_time_t t2) {
  return is_msb_set(t2 - t1);
}

mu_vm_time_dt mu_vm_time_ms_to_duration(mu_vm_time_ms_dt ms) {
  return mu_vm_time_s_to_duration(ms * 1000);
}

mu_vm_time_ms_dt mu_vm_time_duration_to_ms(mu_vm_time_dt dt) {
  return mu_vm_time_duration_to_s(dt) / 1000;
}

mu_vm_time_dt mu_vm_time_s_to_duration(mu_vm_time_s_dt seconds) {
  return seconds * CLOCKS_PER_SEC;
}

mu_vm_time_s_dt mu_vm_time_duration_to_s(mu_vm_time_dt dt) {
  mu_vm_time_s_dt secs = (mu_vm_time_s_dt)dt / (mu_vm_time_s_dt)CLOCKS_PER_SEC;
  return secs;
}

mu_vm_time_t mu_vm_rtc_now() {
  return clock();
}

// =============================================================================
// private code

static bool is_msb_set(mu_vm_time_t t) {
  return TIME_IS_SIGNED ? (t < 0) : ((t << 1) >> 1) ^ t;
}
