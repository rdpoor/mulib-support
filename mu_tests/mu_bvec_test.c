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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFbvecEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 // =============================================================================
 // includes

#include "mu_bvec.h"
#include "test_utilities.h"

// =============================================================================
// private types and definitions

// # of bytes in the bit vector
#define BVEC_BYTE_COUNT1 4
#define BVEC_BYTE_COUNT2 3

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_bvec_test() {
  uint8_t pool1[BVEC_BYTE_COUNT1];
  uint8_t pool2[BVEC_BYTE_COUNT2];
  mu_bvec_t bvec1, bvec2;
  mu_bvec_t *bv1 = &bvec1;
  mu_bvec_t *bv2 = &bvec2;

  UTEST_ASSERTEQ_PTR(mu_bvec_init(bv1, pool1, BVEC_BYTE_COUNT1), bv1);
  UTEST_ASSERTEQ_PTR(mu_bvec_init(bv2, pool2, BVEC_BYTE_COUNT2), bv2);

  UTEST_ASSERTEQ_INT(mu_bvec_capacity(bv1), BVEC_BYTE_COUNT1 * 8);

  UTEST_ASSERTEQ_PTR(mu_bvec_bits(bv1), pool1);

  // assure vector is zero'd on initialization
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), false);
  }

  // mu_bvec_set_all()
  UTEST_ASSERTEQ_PTR(mu_bvec_set_all(bv1), bv1);
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), true);
  }

  // mu_bvec_clear_all()
  UTEST_ASSERTEQ_PTR(mu_bvec_clear_all(bv1), bv1);
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), false);
  }

  // mu_bvec_toggle_all()
  UTEST_ASSERTEQ_PTR(mu_bvec_toggle_all(bv1), bv1);
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), true);
  }

  // mu_bvec_set_bit()
  mu_bvec_clear_all(bv1);
  UTEST_ASSERTEQ_PTR(mu_bvec_set_bit(bv1, 7), bv1);
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    if (i == 7) {
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), true);
    } else {
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), false);
    }
  }

  // mu_bvec_clear_bit()
  mu_bvec_set_all(bv1);
  UTEST_ASSERTEQ_PTR(mu_bvec_clear_bit(bv1, 7), bv1);
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    if (i == 7) {
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), false);
    } else {
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), true);
    }
  }

  // mu_bvec_toggle_bit()
  mu_bvec_clear_all(bv1);
  UTEST_ASSERTEQ_PTR(mu_bvec_toggle_bit(bv1, 7), bv1);
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    if (i == 7) {
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), true);
    } else {
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), false);
    }
  }
  UTEST_ASSERTEQ_PTR(mu_bvec_toggle_bit(bv1, 7), bv1);
  for (int i=mu_bvec_capacity(bv1)-1; i>=0; i--) {
    UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv1, i), false);
  }

  // put some interesting bits into bv1
  mu_bvec_clear_all(bv1);
  mu_bvec_set_bit(bv1, 24);
  mu_bvec_set_bit(bv1, 17);
  mu_bvec_set_bit(bv1, 10);
  mu_bvec_set_bit(bv1, 3);

  // mu_bvec_t *mu_bvec_copy(mu_bvec_t *dst, mu_bvec_t *src);
  mu_bvec_set_all(bv2);
  UTEST_ASSERTEQ_PTR(mu_bvec_copy(bv2, bv1), bv2);
  for (int i=mu_bvec_capacity(bv2)-1; i>=0; i--) {
    switch(i) {
      case 3:
      case 10:
      case 17:
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv2, i), true);
      break;
      default:
      UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv2, i), false);
    }
  }

  // mu_bvec_t *mu_bvec_xor(mu_bvec_t *dst, mu_bvec_t *src);
  UTEST_ASSERTEQ_PTR(mu_bvec_xor(bv2, bv1), bv2);
  for (int i=mu_bvec_capacity(bv2)-1; i>=0; i--) {
    UTEST_ASSERTEQ_BOOL(mu_bvec_get_bit(bv2, i), false);
  }

  // bool mu_bvec_is_all_ones(mu_bvec_t *bv1);
  mu_bvec_clear_all(bv1);
  UTEST_ASSERTEQ_BOOL(mu_bvec_is_all_ones(bv1), false);
  mu_bvec_set_all(bv1);
  UTEST_ASSERTEQ_BOOL(mu_bvec_is_all_ones(bv1), true);

  // bool mu_bvec_is_all_zeros(mu_bvec_t *bv1);
  mu_bvec_clear_all(bv1);
  UTEST_ASSERTEQ_BOOL(mu_bvec_is_all_zeros(bv1), true);
  mu_bvec_set_all(bv1);
  UTEST_ASSERTEQ_BOOL(mu_bvec_is_all_zeros(bv1), false);

  // bool mu_bvec_count_ones(mu_bvec_t *bv1);
  // bool mu_bvec_count_zeros(mu_bvec_t *bv1);
  mu_bvec_clear_all(bv1);
  UTEST_ASSERTEQ_INT(mu_bvec_count_ones(bv1), 0);
  UTEST_ASSERTEQ_INT(mu_bvec_count_zeros(bv1), mu_bvec_capacity(bv1));

  mu_bvec_clear_all(bv1);
  mu_bvec_set_bit(bv1, 24);
  mu_bvec_set_bit(bv1, 17);
  mu_bvec_set_bit(bv1, 10);
  mu_bvec_set_bit(bv1, 3);

  UTEST_ASSERTEQ_INT(mu_bvec_count_ones(bv1), 4);
  UTEST_ASSERTEQ_INT(mu_bvec_count_zeros(bv1), mu_bvec_capacity(bv1) - 4);
}

// =============================================================================
// private code
