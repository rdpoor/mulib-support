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

#include "mu_bvec.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// =============================================================================
// local types and definitions

#ifndef MIN
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

// =============================================================================
// local (forward) declarations

static int get_ref_and_mask(mu_bvec_t *bv, int i, uint8_t **b);
static int count_ones_nibble(uint8_t v);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_bvec_t *mu_bvec_init(mu_bvec_t *bv, uint8_t *bits, int byte_count) {
  bv->bits = bits;
  bv->byte_count = byte_count;
  bv->capacity = byte_count * 8;
  return mu_bvec_clear_all(bv);
}

size_t mu_bvec_capacity(mu_bvec_t *bv) {
  return bv->capacity;
}

uint8_t *mu_bvec_bits(mu_bvec_t *bv) {
  return bv->bits;
}

mu_bvec_t *mu_bvec_set_all(mu_bvec_t *bv) {
  memset(bv->bits, 0xff, bv->byte_count);
  return bv;
}

mu_bvec_t *mu_bvec_clear_all(mu_bvec_t *bv) {
  memset(bv->bits, 0x00, bv->byte_count);
  return bv;
}

mu_bvec_t *mu_bvec_toggle_all(mu_bvec_t *bv) {
  for (int i=bv->byte_count-1; i>=0; i--) {
    bv->bits[i] ^= 0xff;
  }
  return bv;
}

mu_bvec_t *mu_bvec_set_bit(mu_bvec_t *bv, int i) {
  uint8_t *b, m;
  m = get_ref_and_mask(bv, i, &b);
  if (b) {
    *b |= m;
    return bv;
  } else {
    return NULL;
  }
}

mu_bvec_t *mu_bvec_clear_bit(mu_bvec_t *bv, int i) {
  uint8_t *b, m;
  m = get_ref_and_mask(bv, i, &b);
  if (b) {
    *b &= ~m;
    return bv;
  } else {
    return NULL;
  }
  return bv;
}

mu_bvec_t *mu_bvec_toggle_bit(mu_bvec_t *bv, int i) {
  uint8_t *b, m;
  m = get_ref_and_mask(bv, i, &b);
  if (b) {
    *b ^= m;
    return bv;
  } else {
    return NULL;
  }
  return bv;
}

bool mu_bvec_get_bit(mu_bvec_t *bv, int i) {
  uint8_t *b, m;
  m = get_ref_and_mask(bv, i, &b);
  if (b) {
    return *b & m;
  } else {
    // silent error.
    return false;
  }
}

mu_bvec_t *mu_bvec_copy(mu_bvec_t *dst, mu_bvec_t *src) {
  int n = MIN(dst->byte_count, src->byte_count);
  memcpy(dst->bits, src->bits, n);
  return dst;
}

mu_bvec_t *mu_bvec_and(mu_bvec_t *dst, mu_bvec_t *src) {
  int n = MIN(dst->byte_count, src->byte_count);
  for (int i=n-1; i>=0; i--) {
    dst->bits[i] &= src->bits[i];
  }
  return dst;
}

mu_bvec_t *mu_bvec_or(mu_bvec_t *dst, mu_bvec_t *src) {
  int n = MIN(dst->byte_count, src->byte_count);
  for (int i=n-1; i>=0; i--) {
    dst->bits[i] |= src->bits[i];
  }
  return dst;
}

mu_bvec_t *mu_bvec_xor(mu_bvec_t *dst, mu_bvec_t *src) {
  int n = MIN(dst->byte_count, src->byte_count);
  for (int i=n-1; i>=0; i--) {
    dst->bits[i] ^= src->bits[i];
  }
  return dst;
}

bool mu_bvec_is_all_ones(mu_bvec_t *bv) {
  for (int i=bv->byte_count-1; i>=0; i--) {
    if (bv->bits[i] != 0xff) return false;
  }
  return true;
}

bool mu_bvec_is_all_zeros(mu_bvec_t *bv) {
  for (int i=bv->byte_count-1; i>=0; i--) {
    if (bv->bits[i] != 0x0) return false;
  }
  return true;
}

int mu_bvec_count_ones(mu_bvec_t *bv) {
  int ones = 0;
  for (int i=bv->byte_count-1; i>=0; i--) {
    uint8_t v = bv->bits[i];
    ones += count_ones_nibble(v);
    ones += count_ones_nibble(v>>4);
  }
  return ones;
}

int mu_bvec_count_zeros(mu_bvec_t *bv) {
  return mu_bvec_capacity(bv) - mu_bvec_count_ones(bv);
}

// =============================================================================
// local (static) code

static int get_ref_and_mask(mu_bvec_t *bv, int i, uint8_t **b) {
  if (i >= bv->capacity) {
    *b = NULL;
    return 0;
  } else {
    *b = &(bv->bits[i/8]);
    return 1<<(i & 7);
  }
}

// Trade off for speed and size...
static int count_ones_nibble(uint8_t v) {
  static uint8_t count[] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
  };
  return count[v & 0xf];
}
