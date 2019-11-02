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
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>


// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

// =============================================================================
// public code

mu_substring_t *mu_substring_init(mu_substring_t *substr,
                                  const char *str,
                                  int start,
                                  size_t length) {
  substr->src = str;
  substr->start = start;
  substr->length = length;
  return substr;
}

// get a pointer to the first char of the substring
const char *mu_substring_data(mu_substring_t *substr) {
  return &(substr->src[substr->start]);
}

// get the number of characters in the substring
size_t mu_substring_length(mu_substring_t *substr) {
  return substr->length;
}

// compare substring with a string
int mu_substring_cmp(mu_substring_t *substr, const char *s) {
  return strncmp(mu_substring_data(substr), s, mu_substring_length(substr));
}

// return true if the substring equals the given string
bool mu_substring_eq(mu_substring_t *substr, const char *s) {
  return mu_substring_cmp(substr, s) == 0;
}

// copy up to dst_length chars from substring to dst.
char *mu_substring_extract(mu_substring_t *substr, char *dst, int dst_length) {
  int to_copy = mu_substring_length(substr);

  if (to_copy >= dst_length) {
    to_copy = dst_length;
  }
  strncpy(dst, mu_substring_data(substr), to_copy);

  // null terminate if there is room
  if (to_copy < dst_length) {
    dst[to_copy] = '\0';
  }

  return dst;
}
