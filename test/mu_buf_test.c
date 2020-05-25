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
 #include "mu_buf.h"
 #include <string.h>
 #include <stdlib.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

#define ELEMENT_COUNT 10

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_buf_test() {
  mu_buf_t bi;
  mu_buf_t *b = &bi;
  const char *s1 = "the quick brown fox jumps over the lazy dog.";
  char s2[ELEMENT_COUNT];

  ASSERT(mu_buf_init(b, NULL, true, sizeof(char), sizeof(s1) / sizeof(char)) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_init(b, (void *)s1, true, MU_BUF_MAX_ELEMENT_SIZE, sizeof(s1) / sizeof(char)) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_init(b, (void *)s1, true, sizeof(char), MU_BUF_MAX_CAPACITY) == MU_BUF_ERR_ILLEGAL_ARG);

  ASSERT(mu_buf_init(b, (void *)s1, true, sizeof(char), sizeof(s1) / sizeof(char)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_elements(b) == s1);
  ASSERT(mu_buf_is_read_only(b) == true);
  ASSERT(mu_buf_element_size(b) == sizeof(char));
  ASSERT(mu_buf_capacity(b) == sizeof(s1) / sizeof(char));

  ASSERT(mu_buf_init(b, (void *)s2, false, sizeof(char), sizeof(s2) / sizeof(char)) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_elements(b) == s2);
  ASSERT(mu_buf_is_read_only(b) == false);
  ASSERT(mu_buf_element_size(b) == sizeof(char));
  ASSERT(mu_buf_capacity(b) == sizeof(s2) / sizeof(char));

  ASSERT(mu_buf_from_cstr(b, NULL) == MU_BUF_ERR_ILLEGAL_ARG);
  ASSERT(mu_buf_from_cstr(b, s1) == MU_BUF_ERR_NONE);
  ASSERT(mu_buf_elements(b) == s1);
  ASSERT(mu_buf_is_read_only(b) == true);
  ASSERT(mu_buf_element_size(b) == sizeof(char));
  ASSERT(mu_buf_capacity(b) == strlen(s1));
}

// =============================================================================
// private code
