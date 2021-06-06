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

#include "mu_test_utils.h"
#include "core/mu_bvec.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// =============================================================================
// private types and definitions

#define BIT_COUNT 10   // One byte and two bits

// =============================================================================
// private declarations

// =============================================================================
// local storage

static uint8_t s_bits[MU_BVEC_COUNT_TO_BYTE_COUNT(BIT_COUNT)];

// =============================================================================
// public code

void mu_bvec_test() {
  ASSERT(sizeof(s_bits) == 2);
  ASSERT(mu_bvec_byte_index(BIT_COUNT) == 1);
  ASSERT(mu_bvec_byte_mask(BIT_COUNT) == 0x4);

  for(size_t p = 0; p < BIT_COUNT; p++) {
    mu_bvec_clear_all(BIT_COUNT, s_bits);
    mu_bvec_set(p, s_bits);
    for (size_t q = 0; q < BIT_COUNT; q++) {
      if (p != q) {
        ASSERT(mu_bvec_read(q, s_bits) == false);
      } else {
        ASSERT(mu_bvec_read(q, s_bits) == true);
      }
    }
  }
}

// =============================================================================
// private code
