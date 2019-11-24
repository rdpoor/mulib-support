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

#define SMALLBUF_SIZE 10
#define BIGBUF_SIZE 100

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_string_test() {
  char smallbuf[SMALLBUF_SIZE];
  mu_string_t str_1;
  mu_string_t *s1 = &str_1;

  char bigbuf[BIGBUF_SIZE];
  mu_string_t str_2;
  mu_string_t *s2 = &str_2;

  // will be used for substring operations
  mu_string_t str_3;
  mu_string_t *s3 = &str_3;

  //                      00000000001111111111222222222233333333334444
  //                      01234567890123456789012345678901234567890123
  const char cstring[] = "the quick brown fox jumps over the lazy dog.";
  mu_string_t cstr_1;
  mu_string_t *c1 = &cstr_1;

  // // initialize a mu_string (writeable).  Sets start and end to 0.
  // mu_string_t *mu_string_init(mu_string_t *s, mu_string_data_t *buf, size_t buf_length);
  UTEST_ASSERTEQ_PTR(mu_string_init(s1, smallbuf, SMALLBUF_SIZE), s1);
  UTEST_ASSERTEQ_INT(mu_string_start(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_capacity(s1), SMALLBUF_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s1), smallbuf);
  UTEST_ASSERTEQ_PTR(mu_string_data(s1), smallbuf);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_available(s1), SMALLBUF_SIZE);

  UTEST_ASSERTEQ_PTR(mu_string_init(s2, bigbuf, BIGBUF_SIZE), s2);
  UTEST_ASSERTEQ_INT(mu_string_start(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_capacity(s2), BIGBUF_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s2), bigbuf);
  UTEST_ASSERTEQ_PTR(mu_string_data(s2), bigbuf);
  UTEST_ASSERTEQ_INT(mu_string_length(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_available(s2), BIGBUF_SIZE);

  // // initialize a mu_cstring (read-only).  Sets start to 0 and end to buf_length.
  // mu_string_t *mu_cstring_init(mu_cstring_t *s, const mu_string_data_t *buf, size_t buf_length);
  UTEST_ASSERTEQ_PTR(mu_cstring_init(c1, cstring, strlen(cstring)), c1);
  UTEST_ASSERTEQ_INT(mu_string_start(c1), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(c1), strlen(cstring));
  UTEST_ASSERTEQ_INT(mu_string_capacity(c1), strlen(cstring));
  UTEST_ASSERTEQ_PTR(mu_string_buf(c1), cstring);
  UTEST_ASSERTEQ_PTR(mu_string_data(c1), cstring);
  UTEST_ASSERTEQ_INT(mu_string_length(c1), strlen(cstring));

  // Copy c1 into s1. Note: source string exceeds smallbuf capacity.
  UTEST_ASSERTEQ_PTR(mu_string_append(s1, c1), s1);
  UTEST_ASSERTEQ_INT(mu_string_start(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s1), SMALLBUF_SIZE);
  UTEST_ASSERTEQ_INT(mu_string_capacity(s1), SMALLBUF_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s1), smallbuf);
  UTEST_ASSERTEQ_PTR(mu_string_data(s1), smallbuf);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), SMALLBUF_SIZE);
  UTEST_ASSERTEQ_INT(mu_string_available(s1), 0);

  // Copy c1 into s2.  Note that s2 has sufficient capacity for source string
  UTEST_ASSERTEQ_PTR(mu_string_append(s2, c1), s2);
  UTEST_ASSERTEQ_INT(mu_string_start(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s2), strlen(cstring));
  UTEST_ASSERTEQ_INT(mu_string_capacity(s2), BIGBUF_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s2), bigbuf);
  UTEST_ASSERTEQ_PTR(mu_string_data(s2), bigbuf);
  UTEST_ASSERTEQ_INT(mu_string_length(s2), strlen(cstring));
  UTEST_ASSERTEQ_INT(mu_string_available(s2), BIGBUF_SIZE - strlen(cstring));

  UTEST_ASSERTEQ_INT(mu_string_cmp(s1, "the quick "), 0);
  UTEST_ASSERTEQ_BOOL(mu_string_cmp(s1, "the quack "), true); // any non-0 value
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s1, "the quick "), true);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s1, "the quack "), false);

  UTEST_ASSERTEQ_PTR(mu_string_duplicate(s3, s1), s3);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s3), mu_string_buf(s1));
  UTEST_ASSERTEQ_PTR(mu_string_data(s3), mu_string_data(s1));
  UTEST_ASSERTEQ_INT(mu_string_start(s3), mu_string_start(s1));
  UTEST_ASSERTEQ_INT(mu_string_end(s3), mu_string_end(s1));
  UTEST_ASSERTEQ_INT(mu_string_length(s3), mu_string_length(s1));
  UTEST_ASSERTEQ_INT(mu_string_capacity(s3), mu_string_capacity(s1));

  UTEST_ASSERTEQ_PTR(mu_string_slice(s1, 4, -4, s2), s2);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s2), mu_string_buf(s1));
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, "qui"), true);

  UTEST_ASSERTEQ_PTR(mu_string_slice(s1, -5, -1, s2), s2);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, "ick "), true);

  // cannot set end before start
  UTEST_ASSERTEQ_PTR(mu_string_slice(s1, 3, 2, s2), s2);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, ""), true);

  // nonexistent string
  UTEST_ASSERTEQ_PTR(mu_string_find(c1, "box", s3), NULL);

  // successful find.
  UTEST_ASSERTEQ_PTR(mu_string_find(c1, "fox", s3), s3);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s3, "fox"), true);
  UTEST_ASSERTEQ_INT(mu_string_start(s3), 16);
  UTEST_ASSERTEQ_INT(mu_string_end(s3), 19);
  UTEST_ASSERTEQ_INT(mu_string_length(s3), 3);

  // set s3 to "the lazy dog"
  UTEST_ASSERTEQ_PTR(mu_string_slice(c1, 31, -1, s3), s3);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s3, "the lazy dog."), true);
  // Assure that find finds the second "the"
  UTEST_ASSERTEQ_PTR(mu_string_find(s3, "the", s3), s3);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s3, "the"), true);
  UTEST_ASSERTEQ_INT(mu_string_start(s3), 31);
  UTEST_ASSERTEQ_INT(mu_string_end(s3), 34);
  UTEST_ASSERTEQ_INT(mu_string_length(s3), 3);

  // set s3 to "the lazy dog"
  UTEST_ASSERTEQ_PTR(mu_string_slice(c1, 31, -1, s3), s3);
  // assure that find does not match on a string outside of the slice
  UTEST_ASSERTEQ_PTR(mu_string_find(s3, "the quick", s3), NULL);

  // A bit of fun.
  UTEST_ASSERTEQ_PTR(mu_string_init(s2, bigbuf, BIGBUF_SIZE), s2);
  mu_string_reset(s2);
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 0, 4, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 35, 43, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 19, 35, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 4, 19, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 43, 44, s3));
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, "the lazy dog jumps over the quick brown fox."), true);

  mu_string_reset(s2);
  mu_string_sprintf(s2, "%d bottles of beer on the wall.", 99);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, "99 bottles of beer on the wall."), true);

  UTEST_ASSERTEQ_PTR(mu_string_extract(c1, smallbuf, SMALLBUF_SIZE), smallbuf);
  UTEST_ASSERTEQ_INT(strncmp(smallbuf, "the quick ", SMALLBUF_SIZE), 0);
}

// =============================================================================
// private code
