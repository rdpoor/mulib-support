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

 // =============================================================================
 // includes

#include "mu_test_utils.h"
#include "mu_strbuf.h"
#include <string.h>
#include <stdlib.h>


// =============================================================================
// private types and definitions

#define ELEMENT_COUNT 10

// =============================================================================
// private declarations

// =============================================================================
// local storage

static mu_strbuf_t s_str;

// =============================================================================
// public code

void mu_strbuf_test() {
  mu_strbuf_t *s = &s_str;
  const char s1[] = "the quick brown fox jumps over the lazy dog.";
  char s2[ELEMENT_COUNT];
  char s3;
  char *ps;

  // mu_strbuf_t *mu_strbuf_init(mu_strbuf_t *s, mu_strbuf_data_t *data, size_t capacity);
  // sizeof() includes null termination
  ASSERT(mu_strbuf_init(s, (mu_strbuf_data_t *)s1, sizeof(s1)) == s);
  ASSERT(mu_strbuf_capacity(s) == sizeof(s1));
  ASSERT(mu_strbuf_data(s) == s1);

  // mu_strbuf_err_t mu_strbuf_ref(mu_strbuf_t *s, size_t index, mu_strbuf_data_t **p);
  // mu_strbuf_err_t mu_strbuf_get(mu_strbuf_t *s, size_t index, mu_strbuf_data_t *d);
  // mu_strbuf_err_t mu_strbuf_put(mu_strbuf_t *s, size_t index, mu_strbuf_data_t d);
  ASSERT(mu_strbuf_ref(s, 0, &ps) == MU_STR_ERR_NONE);
  ASSERT(*ps == 't');
  ASSERT(mu_strbuf_ref(s, sizeof(s1)-1, &ps) == MU_STR_ERR_NONE);
  ASSERT(*ps == '\0');
  ASSERT(mu_strbuf_ref(s, sizeof(s1), &ps) == MU_STR_ERR_INDEX);

  ASSERT(mu_strbuf_get(s, 0, &s3) == MU_STR_ERR_NONE);
  ASSERT(s3 == 't');
  ASSERT(mu_strbuf_get(s, sizeof(s1)-1, &s3) == MU_STR_ERR_NONE);
  ASSERT(s3 == '\0');
  ASSERT(mu_strbuf_get(s, sizeof(s1), &s3) == MU_STR_ERR_INDEX);

  // s1 is const, so we need to use a mutable string
  ASSERT(mu_strbuf_init(s, s2, ELEMENT_COUNT) == s);

  ASSERT(mu_strbuf_put(s, 0, 'a') == MU_STR_ERR_NONE);
  ASSERT(s2[0] == 'a');
  ASSERT(mu_strbuf_put(s, ELEMENT_COUNT-1, 'z') == MU_STR_ERR_NONE);
  ASSERT(s2[ELEMENT_COUNT-1] == 'z');
  ASSERT(mu_strbuf_put(s, ELEMENT_COUNT, '?') == MU_STR_ERR_INDEX);

  // mu_strbuf_t *mu_strbuf_init_from_cstr(mu_strbuf_t *s, const char *cstr);
  // strlen() omits null termination
  ASSERT(mu_strbuf_init_from_cstr(s, s1) == s);
  ASSERT(mu_strbuf_capacity(s) == strlen(s1));
  ASSERT(mu_strbuf_data(s) == s1);

//  ASSERT(mu_strbuf_to_cstr(s, s2, ELEMENT_COUNT) == s);
  ASSERT(strncmp(s1, s2, ELEMENT_COUNT-1) == 0);
  ASSERT(strcmp(s2, "the quick") == 0);
}

// =============================================================================
// private code
