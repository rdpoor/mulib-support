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

#ifndef MU_BVEC_H_
#define MU_BVEC_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// types and definitions

typedef enum {
  BVEC_ERR_NONE,
} template_state_t;

typedef struct {
  size_t byte_count;
  size_t capacity;
  uint8_t *bits;
} mu_bvec_t;

// =============================================================================
// declarations

mu_bvec_t *mu_bvec_init(mu_bvec_t *bv, uint8_t *bits, int byte_count);

size_t mu_bvec_capacity(mu_bvec_t *bv);
uint8_t *mu_bvec_bits(mu_bvec_t *bv);

mu_bvec_t *mu_bvec_set_all(mu_bvec_t *bv);
mu_bvec_t *mu_bvec_clear_all(mu_bvec_t *bv);
mu_bvec_t *mu_bvec_toggle_all(mu_bvec_t *bv);

mu_bvec_t *mu_bvec_set_bit(mu_bvec_t *bv, int i);
mu_bvec_t *mu_bvec_clear_bit(mu_bvec_t *bv, int i);
mu_bvec_t *mu_bvec_toggle_bit(mu_bvec_t *bv, int i);

bool mu_bvec_get_bit(mu_bvec_t *bv, int i);

/**
 * Copy the bits contents of src to dst.  In the event that the bit vectors are
 * different lengths, only the bits in the shorter vector are copied.
 */
mu_bvec_t *mu_bvec_copy(mu_bvec_t *dst, mu_bvec_t *src);
mu_bvec_t *mu_bvec_and(mu_bvec_t *dst, mu_bvec_t *src);
mu_bvec_t *mu_bvec_or(mu_bvec_t *dst, mu_bvec_t *src);
mu_bvec_t *mu_bvec_xor(mu_bvec_t *dst, mu_bvec_t *src);

bool mu_bvec_is_all_ones(mu_bvec_t *bv);
bool mu_bvec_is_all_zeros(mu_bvec_t *bv);

int mu_bvec_count_ones(mu_bvec_t *bv);
int mu_bvec_count_zeros(mu_bvec_t *bv);

// Consider
// int mu_bvec_find_{rightmost leftmost}_{one zero}(mu_bvec_t *bv)

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MU_BVEC_H_ */
