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

#define BUF1_SIZE 10
#define BUF2_SIZE 100

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_string_test() {
  char buf1[BUF1_SIZE];
  mu_string_t str_1;
  mu_string_t *s1 = &str_1;

  char buf2[BUF2_SIZE];
  mu_string_t str_2;
  mu_string_t *s2 = &str_2;

  // will be used for substring operations
  mu_string_t str_3;
  mu_string_t *s3 = &str_3;

  //                      00000000001111111111222222222233333333334444
  //                      01234567890123456789012345678901234567890123
  const char cstring[] = "The quick brown fox jumps over the lazy dog.";
  mu_cstring_t cstr_1;
  mu_cstring_t *c1 = &cstr_1;

  // Will be used as a substring of c1 -- no data of its own.
  mu_cstring_t cstr_2;
  mu_cstring_t *c2 = &cstr_2;

  // // initialize a mu_string (writeable).  Sets start and end to 0.
  // mu_string_t *mu_string_init(mu_string_t *s, mu_string_data_t *buf, size_t buf_length);
  UTEST_ASSERTEQ_PTR(mu_string_init(s1, buf1, BUF1_SIZE), s1);
  UTEST_ASSERTEQ_INT(mu_string_start(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_capacity(s1), BUF1_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s1), buf1);
  UTEST_ASSERTEQ_PTR(mu_string_data(s1), buf1);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_available(s1), BUF1_SIZE);

  UTEST_ASSERTEQ_PTR(mu_string_init(s2, buf2, BUF2_SIZE), s2);
  UTEST_ASSERTEQ_INT(mu_string_start(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_capacity(s2), BUF2_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s2), buf2);
  UTEST_ASSERTEQ_PTR(mu_string_data(s2), buf2);
  UTEST_ASSERTEQ_INT(mu_string_length(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_available(s2), BUF2_SIZE);

  // // initialize a mu_cstring (read-only).  Sets start to 0 and end to buf_length.
  // mu_string_t *mu_cstring_init(mu_cstring_t *s, const mu_string_data_t const *buf, size_t buf_length);
  UTEST_ASSERTEQ_PTR(mu_cstring_init(c1, cstring, strlen(cstring)), c1);
  UTEST_ASSERTEQ_INT(mu_cstring_start(c1), 0);
  UTEST_ASSERTEQ_INT(mu_cstring_end(c1), strlen(cstring));
  UTEST_ASSERTEQ_INT(mu_cstring_capacity(c1), strlen(cstring));
  UTEST_ASSERTEQ_PTR(mu_cstring_buf(c1), cstring);
  UTEST_ASSERTEQ_PTR(mu_cstring_data(c1), cstring);
  UTEST_ASSERTEQ_INT(mu_cstring_length(c1), strlen(cstring));

  // Copy cstring into string.  One example where casting is appropriate.
  // Note that source string exceeds buf1 capacity.
  UTEST_ASSERTEQ_PTR(mu_string_append(s1, (mu_string_t *)c1), s1);
  UTEST_ASSERTEQ_INT(mu_string_start(s1), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s1), BUF1_SIZE);
  UTEST_ASSERTEQ_INT(mu_string_capacity(s1), BUF1_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s1), buf1);
  UTEST_ASSERTEQ_PTR(mu_string_data(s1), buf1);
  UTEST_ASSERTEQ_INT(mu_string_length(s1), BUF1_SIZE);
  UTEST_ASSERTEQ_INT(mu_string_available(s1), 0);

  // Note that buf2 is large enough for source string
  UTEST_ASSERTEQ_PTR(mu_string_append(s2, (mu_string_t *)c1), s2);
  UTEST_ASSERTEQ_INT(mu_string_start(s2), 0);
  UTEST_ASSERTEQ_INT(mu_string_end(s2), strlen(cstring));
  UTEST_ASSERTEQ_INT(mu_string_capacity(s2), BUF2_SIZE);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s2), buf2);
  UTEST_ASSERTEQ_PTR(mu_string_data(s2), buf2);
  UTEST_ASSERTEQ_INT(mu_string_length(s2), strlen(cstring));
  UTEST_ASSERTEQ_INT(mu_string_available(s2), BUF2_SIZE - strlen(cstring));

  UTEST_ASSERTEQ_INT(mu_string_cmp(s1, "The quick "), 0);
  UTEST_ASSERTEQ_BOOL(mu_string_cmp(s1, "The quack "), true); // non-zero
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s1, "The quick "), true);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s1, "The quack "), false);
  UTEST_ASSERTEQ_PTR(mu_string_duplicate(s3, s1), s3);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s3), mu_string_buf(s1));
  UTEST_ASSERTEQ_PTR(mu_string_data(s3), mu_string_data(s1));
  UTEST_ASSERTEQ_INT(mu_string_start(s3), mu_string_start(s1));
  UTEST_ASSERTEQ_INT(mu_string_end(s3), mu_string_end(s1));
  UTEST_ASSERTEQ_INT(mu_string_length(s3), mu_string_length(s1));

  UTEST_ASSERTEQ_PTR(mu_string_slice(s1, 4, -4, s2), s2);
  UTEST_ASSERTEQ_PTR(mu_string_buf(s2), mu_string_buf(s1));
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, "qui"), true);

  // A bit of fun.
  UTEST_ASSERTEQ_PTR(mu_string_init(s2, buf2, BUF2_SIZE), s2);
  mu_string_reset(s2);
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 0, 4, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 35, 43, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 19, 35, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 4, 19, s3));
  mu_string_append(s2, mu_string_slice((mu_string_t *)c1, 43, 44, s3));
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, "The lazy dog jumps over the quick brown fox."), true);

  mu_string_reset(s2);
  mu_string_sprintf(s2, "%d bottles of beer on the wall.", 99);
  UTEST_ASSERTEQ_BOOL(mu_string_eq(s2, "99 bottles of beer on the wall."), true);

  // // reset start = end = 0
  // mu_string_t *mu_string_reset(mu_string_t *s);
  //
  // // reset start = 0, end = capacity
  // mu_cstring_t *mu_cstring_reset(mu_cstring_t *s);

  // // compare substring with a c string
  // int mu_string_cmp(mu_string_t *s, const mu_string_data_t *cstring);
  // int mu_cstring_cmp(mu_cstring_t *s, const mu_string_data_t *cstring);
  //
  // // return true if the string equals the given string
  // bool mu_string_eq(mu_string_t *s, const mu_string_data_t *cstring);
  // bool mu_cstring_eq(mu_cstring_t *s, const mu_string_data_t *cstring);
  //
  // // make a copy of s into dst (shallow copy - doesn't copy chars)
  // mu_string_t *mu_string_duplicate(mu_string_t *dst, mu_string_t *src);
  // mu_cstring_t *mu_cstring_duplicate(mu_cstring_t *dst, mu_cstring_t *src);
  //
  // // ======
  // // The following manipulate the string object s directly unless a dst object is
  // // provided.  The underlying data buffer is not affected.
  //
  // // take a string of the underlying string.  negative end counts from end.
  // mu_string_t *mu_string_slice(mu_string_t *s, int start, int end, mu_string_t *dst);
  // mu_cstring_t *mu_cstring_slice(mu_cstring_t *s, int start, int end, mu_cstring_t *dst);
  //
  // // Find cstring within s.  Returns null if not found, else return sliced results
  // // in dst (if given) or s (if not).
  // mu_string_t *mu_string_find(mu_string_t *s, const mu_string_data_t *cstring, mu_string_t *dst);
  // mu_cstring_t *mu_cstring_find(mu_cstring_t *s, const mu_string_data_t *cstring, mu_cstring_t *dst);
  //
  // // ======
  // // The following modify the string object as well as the underlying buffer.  Note that
  // // these only apply to mu_string objects, not mu_cstring_objects (since mu_cstring objects
  // // cannot be modified).
  //
  // // append string referred to by src onto dst
  // mu_string_t *mu_string_append(mu_string_t *dst, mu_string_t *src);
  //
  // // sprintf() into &string[start]
  // mu_string_t *mu_string_sprintf(mu_string_t *s, const char *fmt, ...);
  //
  // // ======
  // // the following functions copy bytes:
  //
  // // Copy data from string[start] to string[end] (plus null termination, if there's room) into c_str.
  // // Copies at most c_str_length bytes.  Returns c_str.
  // mu_string_data_t *mu_string_extract(mu_string_t *s, mu_string_data_t *c_str, size_t c_str_length);
  // mu_string_data_t *mu_cstring_extract(mu_cstring_t *s, mu_string_data_t *c_str, size_t c_str_length);

}

// =============================================================================
// private code
