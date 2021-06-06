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

#include "core/mu_bvec.h"
#include "mu_test_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// =============================================================================
// private types and definitions

// requires two bytes to hold the bits
#define BIT_COUNT 10

// =============================================================================
// private declarations

// =============================================================================
// local storage

static uint8_t s_bvec[MU_BVEC_COUNT_TO_BYTE_COUNT(BIT_COUNT)];

// =============================================================================
// public code

void mu_bvec_test() {
  ASSERT(mu_bvec_byte_index(0) == 0);
  ASSERT(mu_bvec_byte_mask(0) == 0x01);

  ASSERT(mu_bvec_byte_index(7) == 0);
  ASSERT(mu_bvec_byte_mask(7) == 0x80);

  ASSERT(mu_bvec_byte_index(8) == 1);
  ASSERT(mu_bvec_byte_mask(8) == 0x01);

  ASSERT(mu_bvec_byte_index(10) == 1);
  ASSERT(mu_bvec_byte_mask(10) == 0x04);


  for (size_t p=0; p<BIT_COUNT; p++) {
    mu_bvec_clear_all(BIT_COUNT, s_bvec);
    mu_bvec_set(p, s_bvec);
    for (size_t q=0; q<BIT_COUNT; q++) {
      if (p == q) {
        ASSERT(mu_bvec_read(q, s_bvec) == true);
      } else {
        ASSERT(mu_bvec_read(q, s_bvec) == false);
      }
    }
  }

  mu_bvec_set_all(BIT_COUNT, s_bvec);
  ASSERT(mu_bvec_is_all_zeros(BIT_COUNT, s_bvec) == false);
  ASSERT(mu_bvec_is_all_ones(BIT_COUNT, s_bvec) == true);

  mu_bvec_clear_all(BIT_COUNT, s_bvec);
  ASSERT(mu_bvec_is_all_zeros(BIT_COUNT, s_bvec) == true);
  ASSERT(mu_bvec_is_all_ones(BIT_COUNT, s_bvec) == false);

  mu_bvec_invert_all(BIT_COUNT, s_bvec);
  ASSERT(mu_bvec_is_all_zeros(BIT_COUNT, s_bvec) == false);
  ASSERT(mu_bvec_is_all_ones(BIT_COUNT, s_bvec) == true);

  mu_bvec_write_all(BIT_COUNT, s_bvec, true);
  ASSERT(mu_bvec_is_all_zeros(BIT_COUNT, s_bvec) == false);
  ASSERT(mu_bvec_is_all_ones(BIT_COUNT, s_bvec) == true);

  mu_bvec_write_all(BIT_COUNT, s_bvec, false);
  ASSERT(mu_bvec_is_all_zeros(BIT_COUNT, s_bvec) == true);
  ASSERT(mu_bvec_is_all_ones(BIT_COUNT, s_bvec) == false);

  mu_bvec_clear_all(BIT_COUNT, s_bvec);
  ASSERT(mu_bvec_count_ones(BIT_COUNT, s_bvec) == 0);
  ASSERT(mu_bvec_count_zeros(BIT_COUNT, s_bvec) == BIT_COUNT - 0);

  for (size_t p=0; p<BIT_COUNT; p++) {
    mu_bvec_clear_all(BIT_COUNT, s_bvec);
    mu_bvec_set(p, s_bvec);
    ASSERT(mu_bvec_find_first_one(BIT_COUNT, s_bvec) == p);
  }

  for (size_t p=0; p<BIT_COUNT; p++) {
    mu_bvec_set_all(BIT_COUNT, s_bvec);
    mu_bvec_clear(p, s_bvec);
    ASSERT(mu_bvec_find_first_zero(BIT_COUNT, s_bvec) == p);
  }

  mu_bvec_clear_all(BIT_COUNT, s_bvec);
  for (size_t p=0; p<BIT_COUNT; p++) {
    mu_bvec_set(p, s_bvec);
    ASSERT(mu_bvec_count_ones(BIT_COUNT, s_bvec) == p+1);
    ASSERT(mu_bvec_count_zeros(BIT_COUNT, s_bvec) == BIT_COUNT-(p+1));
  }


}

// =============================================================================
// private code
