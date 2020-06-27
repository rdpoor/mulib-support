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
 * @file Manipulate arrays of byte-sized data.
 */

#ifndef _MU_BUF_H_
#define _MU_BUF_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "mu_types.h"

// =============================================================================
// types and definitions

typedef enum {
  MU_BUF_ERR_NONE = 0,
  MU_BUF_ERR_ILLEGAL_ARG,
  MU_BUF_ERR_INDEX_BOUNDS,
  MU_BUF_ERR_READ_ONLY,
} mu_buf_err_t;

typedef struct {
  void *elements;                  // backing store
  unsigned int is_readonly : 1;    // true if elements is read-only
  unsigned int element_size : 7;   // sizeof(element) in bytes
  unsigned int capacity : 24; // number of elements
} mu_buf_t;

// upper bounds (exclusive)
#define MU_BUF_MAX_ELEMENT_SIZE (1 << 7)
#define MU_BUF_MAX_CAPACITY (1 << 24)

// =============================================================================
// declarations

mu_buf_err_t mu_buf_init(mu_buf_t *b,
                         void *elements,
                         bool is_readonly,
                         size_t element_size,
                         size_t capacity);

void *mu_buf_elements(mu_buf_t *b);

bool mu_buf_is_read_only(mu_buf_t *b);

size_t mu_buf_element_size(mu_buf_t *b);

size_t mu_buf_capacity(mu_buf_t *b);

mu_buf_err_t mu_buf_ref(mu_buf_t *b, size_t index, void **p);

mu_buf_err_t mu_buf_get(mu_buf_t *b, size_t index, void *dst);

mu_buf_err_t mu_buf_put(mu_buf_t *b, size_t index, void *src);

mu_buf_err_t mu_buf_sort(mu_buf_t *b, mu_compare_fn cmp);

mu_buf_err_t mu_buf_from_cstr(mu_buf_t *b, const char *cstr);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_BUF_H_ */
