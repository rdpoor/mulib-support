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

/**
IMPLEMENTATION NOTE:

Nearly every function in this module could be made more efficient.  This first
draft was written with an eye towards simplicity and testability.

Feel free to improve!  The unit test suite is your safety net...
*/

 // =============================================================================
 // includes

 #include "mu_test_utils.h"
 #include "mu_bitvec.h"
 #include <stdlib.h>
 #include <stdint.h>
#include <unistd.h>
#include <stdio.h>
// =============================================================================
// private types and definitions

// requires two bytes to hold the bits
#define VECTOR_LENGTH_1 10
#define VECTOR_LENGTH_2 17

// =============================================================================
// private declarations

// =============================================================================
// local storage

static uint8_t s_bits1[BVEC_LENGTH_TO_BYTE_COUNT(VECTOR_LENGTH_1)];
static uint8_t s_bits2[BVEC_LENGTH_TO_BYTE_COUNT(VECTOR_LENGTH_2)];
static mu_bitvec_t s_bv1;
static mu_bitvec_t s_bv2;

// =============================================================================
// public code

void mu_bitvec_test() {
  mu_bitvec_t *bv1 = &s_bv1;
  mu_bitvec_t *bv2 = &s_bv2;

  // ==========
  // mu_bitvec_t *mu_bitvec_init(mu_bitvec_t *bv, size_t length, uint8_t *bits, size_t byte_count);
  ASSERT(mu_bitvec_init(bv1, VECTOR_LENGTH_1, s_bits1, sizeof(s_bits1)) == bv1);
  ASSERT(mu_bitvec_init(bv2, VECTOR_LENGTH_2, s_bits2, sizeof(s_bits2)) == bv2);
  ASSERT(mu_bitvec_is_all_zeros(bv1) == true);
  ASSERT(mu_bitvec_is_all_zeros(bv2) == true);

  // ==========
  // uint8_t *mu_bitvec_bits(mu_bitvec_t *bv);
  ASSERT(mu_bitvec_bits(bv1) == s_bits1);
  ASSERT(mu_bitvec_bits(bv2) == s_bits2);

  // ==========
  // size_t mu_bitvec_length(mu_bitvec_t *bv)
  ASSERT(mu_bitvec_length(bv1) == VECTOR_LENGTH_1);
  ASSERT(mu_bitvec_length(bv2) == VECTOR_LENGTH_2);

  // ==========
  // size_t mu_bitvec_byte_count(mu_bitvec_t *bv);
  ASSERT(mu_bitvec_byte_count(bv1) == sizeof(s_bits1));
  ASSERT(mu_bitvec_byte_count(bv2) == sizeof(s_bits2));

  // ==========
  // mu_bitvec_t *mu_bitvec_set_all(mu_bitvec_t *bv);
  ASSERT(mu_bitvec_set_all(bv1) == bv1);
  ASSERT(mu_bitvec_is_all_ones(bv1) == true);
  ASSERT(mu_bitvec_set_all(bv2) == bv2);
  ASSERT(mu_bitvec_is_all_ones(bv2) == true);

  // ==========
  // mu_bitvec_t *mu_bitvec_clear_all(mu_bitvec_t *bv);
  ASSERT(mu_bitvec_clear_all(bv1) == bv1);
  ASSERT(mu_bitvec_is_all_zeros(bv1) == true);
  ASSERT(mu_bitvec_clear_all(bv2) == bv2);
  ASSERT(mu_bitvec_is_all_zeros(bv2) == true);

  // ==========
  // mu_bitvec_t *mu_bitvec_toggle_all(mu_bitvec_t *bv);
  ASSERT(mu_bitvec_toggle_all(bv1) == bv1);
  ASSERT(mu_bitvec_is_all_ones(bv1) == true);
  ASSERT(mu_bitvec_toggle_all(bv2) == bv2);
  ASSERT(mu_bitvec_is_all_ones(bv2) == true);

  ASSERT(mu_bitvec_toggle_all(bv1) == bv1);
  ASSERT(mu_bitvec_is_all_zeros(bv1) == true);
  ASSERT(mu_bitvec_toggle_all(bv2) == bv2);
  ASSERT(mu_bitvec_is_all_zeros(bv2) == true);

  // ==========
  // void mu_bitvec_write_bit(mu_bitvec_t *bv, int i, bool value);
  // bool mu_bitvec_read_bit(mu_bitvec_t *bv, int i);
  for (int i=0; i<mu_bitvec_length(bv1); i++) {
    mu_bitvec_clear_all(bv1);
    mu_bitvec_write_bit(bv1, i, true);
    for (int j=0; j<mu_bitvec_length(bv1); j++) {
      // could write the following as ASSERT(mu_bitvec_read_bit(bv1, j) == (i==j)) but
      // that might confuse the future me...
      if (j != i) {
        ASSERT(mu_bitvec_read_bit(bv1, j) == false);
      } else {
        ASSERT(mu_bitvec_read_bit(bv1, j) == true);
      }
    }
  }

  for (int i=0; i<mu_bitvec_length(bv1); i++) {
    mu_bitvec_set_all(bv1);
    mu_bitvec_write_bit(bv1, i, false);
    for (int j=0; j<mu_bitvec_length(bv1); j++) {
      if (j != i) {
        ASSERT(mu_bitvec_read_bit(bv1, j) == true);
      } else {
        ASSERT(mu_bitvec_read_bit(bv1, j) == false);
      }
    }
  }

  // read and write out of bounds
  ASSERT(mu_bitvec_write_bit(bv1, mu_bitvec_length(bv1)-1, true) == bv1);
  ASSERT(mu_bitvec_write_bit(bv1, mu_bitvec_length(bv1), true) == NULL);
  ASSERT(mu_bitvec_read_bit(bv1, mu_bitvec_length(bv1)) == false);

  // ==========
  // mu_bitvec_t *mu_bitvec_set_bit(mu_bitvec_t *bv, int i);
  // mu_bitvec_t *mu_bitvec_clear_bit(mu_bitvec_t *bv, int i);
  // mu_bitvec_t *mu_bitvec_toggle_bit(mu_bitvec_t *bv, int i);

  for (int i=0; i<mu_bitvec_length(bv1); i++) {
    mu_bitvec_clear_all(bv1);
    mu_bitvec_set_bit(bv1, i);
    for (int j=0; j<mu_bitvec_length(bv1); j++) {
      if (j != i) {
        ASSERT(mu_bitvec_read_bit(bv1, j) == false);
      } else {
        ASSERT(mu_bitvec_read_bit(bv1, j) == true);
      }
    }
  }
  // index in and out of bounds
  ASSERT(mu_bitvec_set_bit(bv1, mu_bitvec_length(bv1)-1) == bv1);
  ASSERT(mu_bitvec_set_bit(bv1, mu_bitvec_length(bv1)) == NULL);

  for (int i=0; i<mu_bitvec_length(bv1); i++) {
    mu_bitvec_set_all(bv1);
    mu_bitvec_clear_bit(bv1, i);
    for (int j=0; j<mu_bitvec_length(bv1); j++) {
      if (j != i) {
        ASSERT(mu_bitvec_read_bit(bv1, j) == true);
      } else {
        ASSERT(mu_bitvec_read_bit(bv1, j) == false);
      }
    }
  }
  // index in and out of bounds
  ASSERT(mu_bitvec_clear_bit(bv1, mu_bitvec_length(bv1)-1) == bv1);
  ASSERT(mu_bitvec_clear_bit(bv1, mu_bitvec_length(bv1)) == NULL);

  for (int i=0; i<mu_bitvec_length(bv1); i++) {
    mu_bitvec_clear_all(bv1);
    mu_bitvec_toggle_bit(bv1, i);
    for (int j=0; j<mu_bitvec_length(bv1); j++) {
      if (j != i) {
        ASSERT(mu_bitvec_read_bit(bv1, j) == false);
      } else {
        ASSERT(mu_bitvec_read_bit(bv1, j) == true);
      }
    }
  }
  // index in and out of bounds
  ASSERT(mu_bitvec_toggle_bit(bv1, mu_bitvec_length(bv1)-1) == bv1);
  ASSERT(mu_bitvec_toggle_bit(bv1, mu_bitvec_length(bv1)) == NULL);

  for (int i=0; i<mu_bitvec_length(bv1); i++) {
    mu_bitvec_set_all(bv1);
    mu_bitvec_toggle_bit(bv1, i);
    for (int j=0; j<mu_bitvec_length(bv1); j++) {
      if (j != i) {
        ASSERT(mu_bitvec_read_bit(bv1, j) == true);
      } else {
        ASSERT(mu_bitvec_read_bit(bv1, j) == false);
      }
    }
  }

  // ==========
  // mu_bitvec_t *mu_bitvec_copy(mu_bitvec_t *dst, mu_bitvec_t *src);
  mu_bitvec_set_all(bv1);
  mu_bitvec_clear_all(bv2);
  ASSERT(mu_bitvec_copy(bv2, bv1) == bv2);
  for (int i=0; i<mu_bitvec_length(bv2); i++) {
    // only bits from the shorter of the two are copied.
    if (i < mu_bitvec_length(bv1)) {
      ASSERT(mu_bitvec_read_bit(bv2, i) == true);
    } else {
      ASSERT(mu_bitvec_read_bit(bv2, i) == false);
    }
  }

  mu_bitvec_set_all(bv1);
  mu_bitvec_clear_all(bv2);
  ASSERT(mu_bitvec_copy(bv1, bv2) == bv1);
  for (int i=0; i<mu_bitvec_length(bv1); i++) {
    // all bits from bv2 are copied since it is the longer of the two
    ASSERT(mu_bitvec_read_bit(bv2, i) == false);
  }

  // ==========
  // mu_bitvec_t *mu_bitvec_and(mu_bitvec_t *dst, mu_bitvec_t *src);
  mu_bitvec_write_bit(bv1, 0, false);
  mu_bitvec_write_bit(bv2, 0, false);
  mu_bitvec_and(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == false);

  mu_bitvec_write_bit(bv1, 0, true);
  mu_bitvec_write_bit(bv2, 0, false);
  mu_bitvec_and(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == false);

  mu_bitvec_write_bit(bv1, 0, false);
  mu_bitvec_write_bit(bv2, 0, true);
  mu_bitvec_and(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == false);

  mu_bitvec_write_bit(bv1, 0, true);
  mu_bitvec_write_bit(bv2, 0, true);
  mu_bitvec_and(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == true);

  // ==========
  // mu_bitvec_t *mu_bitvec_or(mu_bitvec_t *dst, mu_bitvec_t *src);
  mu_bitvec_write_bit(bv1, 0, false);
  mu_bitvec_write_bit(bv2, 0, false);
  mu_bitvec_or(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == false);

  mu_bitvec_write_bit(bv1, 0, true);
  mu_bitvec_write_bit(bv2, 0, false);
  mu_bitvec_or(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == true);

  mu_bitvec_write_bit(bv1, 0, false);
  mu_bitvec_write_bit(bv2, 0, true);
  mu_bitvec_or(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == true);

  mu_bitvec_write_bit(bv1, 0, true);
  mu_bitvec_write_bit(bv2, 0, true);
  mu_bitvec_or(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == true);

  // ==========
  // mu_bitvec_t *mu_bitvec_xor(mu_bitvec_t *dst, mu_bitvec_t *src);
  mu_bitvec_write_bit(bv1, 0, false);
  mu_bitvec_write_bit(bv2, 0, false);
  mu_bitvec_xor(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == false);

  mu_bitvec_write_bit(bv1, 0, true);
  mu_bitvec_write_bit(bv2, 0, false);
  mu_bitvec_xor(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == true);

  mu_bitvec_write_bit(bv1, 0, false);
  mu_bitvec_write_bit(bv2, 0, true);
  mu_bitvec_xor(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == true);

  mu_bitvec_write_bit(bv1, 0, true);
  mu_bitvec_write_bit(bv2, 0, true);
  mu_bitvec_xor(bv1, bv2);
  ASSERT(mu_bitvec_read_bit(bv1, 0) == false);

  // ==========
  // bool mu_bitvec_equals(mu_bitvec_t *bv1, mu_bitvec_t *bv2);
  mu_bitvec_clear_all(bv1);
  mu_bitvec_clear_all(bv2);
  ASSERT(mu_bitvec_equals(bv1, bv2) == true);
  // a bit set in bv2 above bv1's length isn't counted
  mu_bitvec_write_bit(bv2, mu_bitvec_length(bv1), true);
  ASSERT(mu_bitvec_equals(bv1, bv2) == true);
  // ... but within bv1's length is counted
  mu_bitvec_write_bit(bv2, mu_bitvec_length(bv1)-1, true);
  ASSERT(mu_bitvec_equals(bv1, bv2) == false);

  // ==========
  // bool mu_bitvec_is_all_ones(mu_bitvec_t *bv);
  mu_bitvec_clear_all(bv1);
  ASSERT(mu_bitvec_is_all_ones(bv1) == false);
  mu_bitvec_set_all(bv1);
  ASSERT(mu_bitvec_is_all_ones(bv1) == true);

  // ==========
  // bool mu_bitvec_is_all_zeros(mu_bitvec_t *bv);
  mu_bitvec_clear_all(bv1);
  ASSERT(mu_bitvec_is_all_zeros(bv1) == true);
  mu_bitvec_set_all(bv1);
  ASSERT(mu_bitvec_is_all_zeros(bv1) == false);

  // ==========
  // int mu_bitvec_count_ones(mu_bitvec_t *bv);
  mu_bitvec_clear_all(bv1);
  ASSERT(mu_bitvec_count_ones(bv1) == 0);
  mu_bitvec_write_bit(bv1, 0, true);
  ASSERT(mu_bitvec_count_ones(bv1) == 1);
  mu_bitvec_write_bit(bv1, mu_bitvec_length(bv1)-1, true);
  ASSERT(mu_bitvec_count_ones(bv1) == 2);

  // ==========
  // int mu_bitvec_count_zeros(mu_bitvec_t *bv);
  mu_bitvec_clear_all(bv1);
  ASSERT(mu_bitvec_count_zeros(bv1) == mu_bitvec_length(bv1));
  mu_bitvec_write_bit(bv1, 0, true);
  ASSERT(mu_bitvec_count_zeros(bv1) == mu_bitvec_length(bv1)-1);
  mu_bitvec_write_bit(bv1, mu_bitvec_length(bv1)-1, true);
  ASSERT(mu_bitvec_count_zeros(bv1) == mu_bitvec_length(bv1)-2);
}

// =============================================================================
// private code
