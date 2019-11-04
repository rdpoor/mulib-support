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

#include "mu_string.h"
#include "test_utilities.h"
#include <string.h>
#include <stdio.h>

// =============================================================================
// private types and definitions

#define BUF_SIZE 10

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_string_test() {
  char src_cstring[] = "The quick brown fox jumps over the lazy dog.";
  char dst_buf[BUF_SIZE];
  mu_string_t str_1, str_2;

  mu_string_t *s1 = &str_1;
  mu_string_t *s2 = &str_2;

  // mu_string_t *mu_string_init(mu_string_t *s, mu_cstring_t *buf, size_t buf_length);
  // mu_cstring_t *mu_string_buf(mu_string_t *s);
  // size_t mu_string_buf_length(mu_string_t *s);
  UTEST_ASSERTEQ_PTR(mu_string_init(s1, src_cstring, strlen(src_cstring)), s1);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s1), src_cstring);
  UTEST_ASSERTEQ_INT(mu_string_buf_length(s1), strlen(src_cstring));

  // start and end pointers are initially zero
  UTEST_ASSERTEQ_INT(mu_string_start(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_available(s1), strlen(src_cstring));

  // reset start = end = 0
  // mu_string_t *mu_string_reset(mu_string_t *s);

  // mu_string_t *mu_string_slice(mu_string_t *s, int start, int end, mu_string_t *dst);
  UTEST_ASSERTEQ_PTR(mu_string_slice(s1, 16, 19, NULL), s1);
  UTEST_ASSERTEQ_INT(mu_string_start(s1), 16);
  UTEST_ASSERTEQ_INT(mu_string_end(s1), 19);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), 3);
  UTEST_ASSERT(mu_string_cmp(s1, "fox") == 0);
  UTEST_ASSERT(mu_string_cmp(s1, "box") > 0);
  UTEST_ASSERT(mu_string_cmp(s1, "vox") < 0);

  UTEST_ASSERTEQ_BOOL(mu_string_eq(s1, "fox"), true);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s1, "box"), false);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s1, "vox"), false);

  // test negative indeces for slice
  UTEST_ASSERTEQ_PTR(mu_string_slice(s1, -5, -2, NULL), s1);
  UTEST_ASSERTEQ_INT(mu_string_start(s1), 40);
  UTEST_ASSERTEQ_INT(mu_string_end(s1), 43);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), 3);

  UTEST_ASSERT(mu_string_cmp(s1, "dog") == 0);
  UTEST_ASSERT(mu_string_cmp(s1, "bog") > 0);
  UTEST_ASSERT(mu_string_cmp(s1, "log") < 0);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), 3);

  UTEST_ASSERTEQ_PTR(mu_string_duplicate(s2, s1), s2);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s2), mu_string_buf(s1));
  UTEST_ASSERTEQ_INT(mu_string_length(s2), mu_string_length(s1));
  UTEST_ASSERTEQ_INT(mu_string_start(s2), mu_string_start(s1));
  UTEST_ASSERTEQ_INT(mu_string_end(s2), mu_string_end(s1));
  UTEST_ASSERTEQ_INT(mu_string_length(s2), mu_string_length(s1));

  // Find cstring within s.  Returns null if not found, else return sliced results
  // in dst (if given) or s (if not).
  // mu_string_t *mu_string_find(mu_string_t *s, const mu_cstring_t *cstring, mu_string_t *dst);
  UTEST_ASSERTEQ_PTR(mu_string_find(s1, "quack", NULL), NULL);
  UTEST_ASSERTEQ_PTR(mu_string_find(s1, "quick", s2), s2);
  UTEST_ASSERTEQ_PTR(mu_string_find(s1, "quick", NULL), s1);

  // the following functions copy bytes:

  // Copy data from string[i0] to string[i1] (plus null termination) into c_str.
  // mu_cstring_t *mu_string_extract(mu_string_t *s, mu_cstring_t *c_str, size_t c_str_length);
  UTEST_ASSERTEQ_PTR(mu_string_extract(s1, dst_buf, BUF_SIZE), dst_buf);
  UTEST_ASSERTEQ_INT(strcmp(dst_buf, "quick"), 0);

  // append string referred to by src onto dst
  // mu_string_t *mu_string_append(mu_string_t *dst, mu_string_t *src);
  UTEST_ASSERTEQ_PTR(mu_string_init(s2, dst_buf, BUF_SIZE), s2);
  UTEST_ASSERTEQ_PTR(mu_string_append(s2, s1), s2);
  UTEST_ASSERTEQ_PTR(mu_string_sprintf(s2, " %d", 42), s2);
  UTEST_ASSERTEQ_INT(strcmp(dst_buf, "quick 42"), 0);


  // sprintf() into &string[i1]
  // mu_string_t *mu_string_sprintf(mu_string_t *s, const char *fmt, ...);


}

// =============================================================================
// private code
