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

#include "mulib.h"
#include "mu_test_utils.h"
#include <unistd.h>

#include <stdio.h>
// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_time_test() {
  mu_time_t t1;
  mu_time_t t2;

  mu_time_dt dt1;
  mu_time_ms_dt dm1;

  t1 = mu_time_now();   // an arbitrary time
  dm1 = mu_time_ms_to_duration(1000);
  t2 = mu_time_offset(t1, dm1);

  ASSERT(mu_time_precedes(t1, t2) == true);
  ASSERT(mu_time_precedes(t1, t1) == false);
  ASSERT(mu_time_precedes(t2, t1) == false);

  ASSERT(mu_time_equals(t1, t1) == true);
  ASSERT(mu_time_equals(t1, t2) == false);

  ASSERT(mu_time_follows(t1, t2) == false);
  ASSERT(mu_time_follows(t1, t1) == false);
  ASSERT(mu_time_follows(t2, t1) == true);

  dt1 = mu_time_difference(t2, t1);
  ASSERT(mu_time_duration_to_ms(dt1) == 1000);

#if (MU_VM_HAS_FLOAT)
  mu_time_s_dt ds1;

  t1 = mu_time_now();   // an arbitrary time
  ds1 = mu_time_s_to_duration(1.0);
  t2 = mu_time_offset(t1, dm1);

  ASSERT(mu_time_precedes(t1, t2) == true);
  ASSERT(mu_time_precedes(t1, t1) == false);
  ASSERT(mu_time_precedes(t2, t1) == false);

  ASSERT(mu_time_equals(t1, t1) == true);
  ASSERT(mu_time_equals(t1, t2) == false);

  ASSERT(mu_time_follows(t1, t2) == false);
  ASSERT(mu_time_follows(t1, t1) == false);
  ASSERT(mu_time_follows(t2, t1) == true);

  dt1 = mu_time_difference(t2, t1);
  ASSERT(mu_time_duration_to_s(dt1) == 1.0);
#endif
}

// =============================================================================
// private code
