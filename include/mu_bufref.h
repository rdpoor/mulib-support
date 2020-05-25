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

/**
 * @file Manipulate arrays of byte-sized data.
 */

#ifndef _MU_BUFREF_H_
#define _MU_BUFREF_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "mu_buf.h"

// =============================================================================
// types and definitions

typedef struct {
  mu_buf_t *buf;  // pointer to mu_buf
  size_t start;
  size_t end;
} mu_bufref_t;

// =============================================================================
// declarations

mu_buf_err_t mu_bufref_init(mu_bufref_t *ref, mu_buf_t *buf);

mu_buf_err_t mu_bufref_slice_buf(mu_bufref_t *ref, mu_buf_t *src, ssize_t start, ssize_t end);

mu_buf_err_t mu_bufref_slice_bufref(mu_bufref_t *ref, mu_bufref_t *src, ssize_t start, ssize_t end);

/**
 * @brief reset this buffer reference.
 *
 * Note: Sets end to zero if this is a read-write buffer and to capacity if this
 * is a read-only buffer.  (I might regret this design decision.)
 */
mu_bufref_t *mu_bufref_reset(mu_bufref_t *ref);

mu_buf_t *mu_bufref_buf(mu_bufref_t *ref);

void *mu_bufref_elements(mu_bufref_t *ref);

bool mu_bufref_is_read_only(mu_bufref_t *ref);

size_t mu_bufref_element_size(mu_bufref_t *ref);

size_t mu_bufref_capacity(mu_bufref_t *ref);

size_t mu_bufref_start(mu_bufref_t *ref);

size_t mu_bufref_end(mu_bufref_t *ref);

size_t mu_bufref_count(mu_bufref_t *ref);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_BUFREF_H_ */
