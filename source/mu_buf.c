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

#include "mu_buf.h"
#include <string.h>
#include <stdlib.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

// =============================================================================
// public code

// =============================================================================
// declarations

mu_buf_err_t mu_buf_init(mu_buf_t *b,
                         void *elements,
                         bool is_readonly,
                         size_t element_size,
                         size_t element_count) {
  if (b == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (elements == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (element_size >= MU_BUF_MAX_ELEMENT_SIZE) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (element_count >= MU_BUF_MAX_ELEMENT_COUNT) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  b->elements = elements;
  b->is_readonly = is_readonly;
  b->element_size = element_size;
  b->element_count = element_count;

  return MU_BUF_ERR_NONE;
}

void *mu_buf_elements(mu_buf_t *b) {
  return b->elements;
}

bool mu_buf_is_read_only(mu_buf_t *b) {
  return b->is_readonly;
}

size_t mu_buf_element_size(mu_buf_t *b) {
  return b->element_size;
}

size_t mu_buf_element_count(mu_buf_t *b) {
  return b->element_count;
}

mu_buf_err_t mu_buf_from_cstr(mu_buf_t *b, const char *cstr) {
  if (cstr == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  return mu_buf_init(b, (void *)cstr, true, sizeof(char), strlen(cstr));
}

// =============================================================================
// local (static) code