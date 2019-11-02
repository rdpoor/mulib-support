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

#include "mu_substring.h"
#include "test_utilities.h"
#include <string.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

#define DST_SIZE 10

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_substring_test() {
  mu_substring_t substring;
  mu_substring_t *ss = &substring;
  const char *s = "The quick brown fox jumps over the lazy dog.";
  char dst[DST_SIZE];

  // Initialize a mu_substring_t object with a reference to an underlying string
  // mu_substring_t *mu_substring_init(mu_substring_t *substr, const char *str, int start, int length);
  UTEST_ASSERTEQ_PTR(mu_substring_init(ss, s, 12, 3), ss);

  // Return a reference to the start of the mu_substring
  // const char *mu_substring_data(mu_substring_t *substr);
  UTEST_ASSERT(mu_substring_data(ss)[0] == 'o');
  UTEST_ASSERT(mu_substring_data(ss)[1] == 'w');

  // Return the length of the mu_substring
  // int mu_substring_length(mu_substring_t *substr);
  UTEST_ASSERTEQ_INT(mu_substring_length(ss), 3);

  // Compare a mu_substring to a string
  // int mu_substring_cmp(mu_substring_t *substr, const char *s);
  UTEST_ASSERT(mu_substring_cmp(ss, "owl") > 0);
  UTEST_ASSERT(mu_substring_cmp(ss, "own") == 0);
  UTEST_ASSERT(mu_substring_cmp(ss, "ows") < 0);

  // Return true if the mu_substring is equal to another string
  // bool mu_substring_eq(mu_substring_t *substr, const char *s);
  UTEST_ASSERTEQ_BOOL(mu_substring_eq(ss, "own"), true);
  UTEST_ASSERTEQ_BOOL(mu_substring_eq(ss, "pwn"), false);

  // Copy up to dst_length characters from a mu_substring into dst
  // const char *mu_substring_extract(mu_substring_t *substr, char *dst, int dst_length);
  memset(dst, 'z', DST_SIZE);  // make sure mu_substring_extract properly terminates dst
  UTEST_ASSERTEQ_PTR(mu_substring_extract(ss, dst, DST_SIZE), dst);
  UTEST_ASSERTEQ_INT(strcmp(dst, "own"), 0);
}

// =============================================================================
// private code
