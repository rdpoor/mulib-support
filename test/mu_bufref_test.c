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

 #include "mu_test_utils.h"
 #include "mu_bufref.h"
 #include "mu_buf.h"
 #include <string.h>
 #include <stdlib.h>

#include <stdio.h>
// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

#define ELEMENT_COUNT 10

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_bufref_test() {
  mu_buf_t bufi;
  mu_buf_t *buf = &bufi;
  mu_bufref_t refi1;
  mu_bufref_t *ref1 = &refi1;
  mu_bufref_t refi2;
  mu_bufref_t *ref2 = &refi2;

  char *s1 = "Hello, world.";
  char s2[ELEMENT_COUNT];

  ASSERT(mu_bufref_init(NULL, buf) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_bufref_init(ref1, NULL) == MU_BUF_ERR_ILLEGAL_ARG);

  mu_buf_from_cstr(buf, s1);
  ASSERT(mu_bufref_init(ref1, buf) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_reset(ref1) == ref1);
  ASSERT(mu_bufref_buf(ref1) == buf);
  ASSERT(mu_bufref_elements(ref1) == s1);
  ASSERT(mu_bufref_is_read_only(ref1) == true);
  ASSERT(mu_bufref_element_size(ref1) == sizeof(char));
  ASSERT(mu_bufref_capacity(ref1) == strlen(s1));
  ASSERT(mu_bufref_start(ref1) == 0);
  ASSERT(mu_bufref_end(ref1) == mu_buf_capacity(buf));
  ASSERT(mu_bufref_count(ref1) == mu_buf_capacity(buf));

  mu_buf_init(buf, s2, false, sizeof(char), sizeof(s2) / sizeof(char));
  ASSERT(mu_bufref_init(ref1, buf) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_reset(ref1) == ref1);
  ASSERT(mu_bufref_buf(ref1) == buf);
  ASSERT(mu_bufref_elements(ref1) == s2);
  ASSERT(mu_bufref_is_read_only(ref1) == false);
  ASSERT(mu_bufref_element_size(ref1) == sizeof(char));
  ASSERT(mu_bufref_capacity(ref1) == sizeof(s2));
  ASSERT(mu_bufref_start(ref1) == 0);
  ASSERT(mu_bufref_end(ref1) == 0);
  ASSERT(mu_bufref_count(ref1) == 0);

  // =====
  // mu_bufref_slice_buf(mu_bufref_t *ref, mu_buf_t *src, size_t start, size_t end);
  ASSERT(mu_bufref_slice_buf(NULL, buf, 0, 0) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_bufref_slice_buf(ref1, NULL, 0, 0) == MU_BUF_ERR_ILLEGAL_ARG);

  ASSERT(mu_bufref_slice_buf(ref1, buf, 0, 0) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref1) == 0);
  ASSERT(mu_bufref_end(ref1) == 0);
  ASSERT(mu_bufref_count(ref1) == 0);

  ASSERT(mu_bufref_slice_buf(ref1, buf, 0, ELEMENT_COUNT) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref1) == 0);
  ASSERT(mu_bufref_end(ref1) == ELEMENT_COUNT);
  ASSERT(mu_bufref_count(ref1) == ELEMENT_COUNT);

  // slice limits end <= capacity
  ASSERT(mu_bufref_slice_buf(ref1, buf, 0, ELEMENT_COUNT+1) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref1) == 0);
  ASSERT(mu_bufref_end(ref1) == ELEMENT_COUNT);
  ASSERT(mu_bufref_count(ref1) == ELEMENT_COUNT);

  // slice limits start <= end
  ASSERT(mu_bufref_slice_buf(ref1, buf, 1, 0) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref1) == 1);
  ASSERT(mu_bufref_end(ref1) == 1);
  ASSERT(mu_bufref_count(ref1) == 0);

  // negative index counts from end (-1 => next to last element)
  ASSERT(mu_bufref_slice_buf(ref1, buf, 0, -1) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref1) == 0);
  ASSERT(mu_bufref_end(ref1) == ELEMENT_COUNT-1);
  ASSERT(mu_bufref_count(ref1) == ELEMENT_COUNT-1);

  // negative index counts from end (-1 => next to last element)
  ASSERT(mu_bufref_slice_buf(ref1, buf, -2, -1) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref1) == ELEMENT_COUNT-2);
  ASSERT(mu_bufref_end(ref1) == ELEMENT_COUNT-1);
  ASSERT(mu_bufref_count(ref1) == 1);

  // =====
  // mu_bufref_slice_bufref(mu_bufref_t *ref, mu_buf_t *src, size_t start, size_t end);
  mu_buf_init(buf, s2, false, sizeof(char), sizeof(s2) / sizeof(char));
  // Slicing will be relative to ref1, which is 4 elements shorter than buf
  mu_bufref_slice_buf(ref1, buf, 2, ELEMENT_COUNT-2);
  ASSERT(mu_bufref_count(ref1) == ELEMENT_COUNT-4);
  ASSERT(mu_bufref_slice_bufref(NULL, ref1, 0, 0) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_bufref_slice_bufref(ref2, NULL, 0, 0) == MU_BUF_ERR_ILLEGAL_ARG);

  ASSERT(mu_bufref_slice_bufref(ref2, ref1, 0, 0) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref2) == 2);
  ASSERT(mu_bufref_end(ref2) == 2);
  ASSERT(mu_bufref_count(ref2) == 0);

  ASSERT(mu_bufref_slice_bufref(ref2, ref1, 0, ELEMENT_COUNT) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref2) == 2);
  ASSERT(mu_bufref_end(ref2) == ELEMENT_COUNT-2);
  ASSERT(mu_bufref_count(ref2) == ELEMENT_COUNT-4);

  // negative index counts from end (-1 => next to last element)
  ASSERT(mu_bufref_slice_bufref(ref2, ref1, 0, -1) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref2) == 2);
  ASSERT(mu_bufref_end(ref2) == ELEMENT_COUNT-3);
  ASSERT(mu_bufref_count(ref2) == ELEMENT_COUNT-5);

  // negative index counts from end (-1 => next to last element)
  ASSERT(mu_bufref_slice_bufref(ref2, ref1, -2, -1) == MU_BUF_ERR_NONE);
  ASSERT(mu_bufref_start(ref2) == ELEMENT_COUNT-4);
  ASSERT(mu_bufref_end(ref2) == ELEMENT_COUNT-3);
  ASSERT(mu_bufref_count(ref2) == 1);
}

// =============================================================================
// private code
