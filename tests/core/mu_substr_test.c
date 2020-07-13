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
#include "mu_substr.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>
// =============================================================================
// private types and definitions

#define ELEMENT_COUNT 15

// =============================================================================
// private declarations

static void reset(void);
static bool substr_equals(mu_substr_t *substr, const char *str);

// =============================================================================
// local storage

static char s_cstr1[] = "discover! ";
static char s_cstr2[ELEMENT_COUNT];

static mu_str_t s_str1, s_str2;

static mu_substr_t s_sstr1, s_sstr2, s_sstr3;

// =============================================================================
// public code

void mu_substr_test() {
  mu_substr_t *ss1 = &s_sstr1;
  mu_substr_t *ss2 = &s_sstr2;
  mu_substr_t *ss3 = &s_sstr3;
  char *pchr;
  char chr;

  reset();
  ASSERT(mu_substr_str(ss1) == &s_str1);
  ASSERT(mu_substr_start(ss1) == 0);
  ASSERT(mu_substr_end(ss1) == strlen(s_cstr1));
  ASSERT(mu_substr_length(ss1) == strlen(s_cstr1));
  ASSERT(mu_substr_remaining(ss1) == 0);

  ASSERT(mu_substr_clear(ss1) == ss1);
  ASSERT(mu_substr_start(ss1) == 0);
  ASSERT(mu_substr_end(ss1) == 0);
  ASSERT(mu_substr_length(ss1) == 0);
  ASSERT(mu_substr_remaining(ss1) == strlen(s_cstr1));

  // ==========
  // mu_substr_t *mu_substr_duplicate(mu_substr_t *dst, mu_substr_t *src);
  reset();
  ASSERT(mu_substr_duplicate(ss3, ss1) == ss3);
  ASSERT(mu_substr_str(ss3) == mu_substr_str(ss1));
  ASSERT(mu_substr_start(ss3) == mu_substr_start(ss1));
  ASSERT(mu_substr_end(ss3) == mu_substr_end(ss1));

  // ==========
  // mu_substr_err_t mu_substr_ref(mu_substr_t *s, size_t index, mu_str_data_t **p);
  // mu_substr_err_t mu_substr_get(mu_substr_t *s, size_t index, mu_str_data_t *d);
  // mu_substr_err_t mu_substr_put(mu_substr_t *s, size_t index, mu_str_data_t d);
  reset();
  ASSERT(mu_substr_slice_str(ss1, 3, 7) == MU_SUBSTR_ERR_NONE);
  ASSERT(substr_equals(ss1, "cove") == true);

  // mu_substr_ref
  ASSERT(mu_substr_ref(ss1, 0, &pchr) == MU_SUBSTR_ERR_NONE);
  ASSERT(*pchr == 'c');
  ASSERT(mu_substr_ref(ss1, 3, &pchr) == MU_SUBSTR_ERR_NONE);
  ASSERT(*pchr == 'e');
  ASSERT(mu_substr_ref(ss1, 4, &pchr) == MU_SUBSTR_ERR_INDEX);

  // mu_substr_get
  ASSERT(mu_substr_get(ss1, 0, &chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(chr == 'c');
  ASSERT(mu_substr_get(ss1, 3, &chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(chr == 'e');
  ASSERT(mu_substr_get(ss1, 4, &chr) == MU_SUBSTR_ERR_INDEX);

  // mu_substr_put
  chr = 'd';
  ASSERT(mu_substr_put(ss1, 0, chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_put(ss1, 4, chr) == MU_SUBSTR_ERR_INDEX);
  ASSERT(substr_equals(ss1, "dove") == true);

  // restore underlying string
  chr = 'c';
  ASSERT(mu_substr_put(ss1, 0, chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(substr_equals(ss1, "cove") == true);

  // ==========
  // mu_substr_err_t *mu_substr_slice_str(mu_substr_t *s, int start, int end)
  reset();
  ASSERT(mu_substr_duplicate(ss3, ss2) == ss3);
  ASSERT(mu_substr_start(ss3) == 0);
  ASSERT(mu_substr_end(ss3) == ELEMENT_COUNT);
  // positive start, positive end
  ASSERT(mu_substr_slice_str(ss3, 1, 2) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_start(ss3) == 1);
  ASSERT(mu_substr_end(ss3) == 2);
  // positive start, negative end
  ASSERT(mu_substr_duplicate(ss3, ss2) == ss3);
  ASSERT(mu_substr_slice_str(ss3, 1, -2) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_start(ss3) == 1);
  ASSERT(mu_substr_end(ss3) == ELEMENT_COUNT-2);
  // negative start, positive end
  ASSERT(mu_substr_duplicate(ss3, ss2) == ss3);
  ASSERT(mu_substr_slice_str(ss3, -4, ELEMENT_COUNT) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_start(ss3) == ELEMENT_COUNT-4);
  ASSERT(mu_substr_end(ss3) == ELEMENT_COUNT);
  // negative start, negative end
  ASSERT(mu_substr_duplicate(ss3, ss2) == ss3);
  ASSERT(mu_substr_slice_str(ss3, -4, -2) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_start(ss3) == ELEMENT_COUNT-4);
  ASSERT(mu_substr_end(ss3) == ELEMENT_COUNT-2);
  // minimum start and end
  ASSERT(mu_substr_duplicate(ss3, ss2) == ss3);
  ASSERT(mu_substr_slice_str(ss3, 0, 0) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_start(ss3) == 0);
  ASSERT(mu_substr_end(ss3) == 0);
  // maximum start and end
  ASSERT(mu_substr_duplicate(ss3, ss2) == ss3);
  ASSERT(mu_substr_slice_str(ss3, ELEMENT_COUNT, ELEMENT_COUNT) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_start(ss3) == ELEMENT_COUNT);
  ASSERT(mu_substr_end(ss3) == ELEMENT_COUNT);
  // start > end: returns illegal index, start and end are unchanged
  ASSERT(mu_substr_duplicate(ss3, ss2) == ss3);
  ASSERT(mu_substr_slice_str(ss3, 1, 0) == MU_SUBSTR_ERR_INDEX);
  ASSERT(mu_substr_start(ss3) == 0);
  ASSERT(mu_substr_end(ss3) == ELEMENT_COUNT);

  // ==========
  // mu_substr_t *mu_substr_copy(mu_substr_t *dst, mu_substr_t *src);
  reset();
  ASSERT(mu_substr_copy(ss2, ss1) == ss2);
  ASSERT(mu_substr_start(ss2) == mu_substr_start(ss1));
  ASSERT(mu_substr_end(ss2) == mu_substr_end(ss1));
  ASSERT(mu_substr_equals(ss2, ss1) == true);   // "discover! ", "discover! "

  // ==========
  // int mu_substr_cmp(mu_substr_t *s1, mu_substr_t *s2)
  reset();
  ASSERT(mu_substr_duplicate(ss3, ss1) == ss3);
  ASSERT(substr_equals(ss3, "discover! ") == true);
  ASSERT(mu_substr_cmp(ss3, ss1) == 0);   // "discover! ", "discover! "

  // mu_substr_cmp on unequal strings
  mu_substr_slice_str(ss3, 1, mu_substr_end(ss3));
  ASSERT(mu_substr_cmp(ss3, ss1) > 0);    // "iscover! ", "discover! "
  ASSERT(mu_substr_cmp(ss1, ss3) < 0);    // "discover! ", "iscover! "

  // mu_substr_cmp on an empty string.
  mu_substr_slice_str(ss3, 0, 0);
  ASSERT(mu_substr_cmp(ss3, ss1) < 0);    // "", "discover! "
  ASSERT(mu_substr_cmp(ss1, ss3) > 0);    // "discover! ", ""

  // ==========
  // int mu_substr_equals(mu_substr_t *s1, mu_substr_t *s2)
  reset();
  ASSERT(mu_substr_duplicate(ss3, ss1) == ss3);
  ASSERT(mu_substr_equals(ss3, ss1) == true);   // "discover! ", "discover! "

  // mu_substr_cmp on unequal strings
  mu_substr_slice_str(ss3, 1, mu_substr_end(ss3));
  ASSERT(mu_substr_equals(ss3, ss1) == false);    // "iscover! ", "discover! "
  ASSERT(mu_substr_equals(ss1, ss3) == false);    // "discover! ", "iscover! "

  // mu_substr_cmp on an empty string.
  mu_substr_slice_str(ss3, 0, 0);
  ASSERT(mu_substr_equals(ss3, ss1) == false);    // "", "discover! "
  ASSERT(mu_substr_equals(ss1, ss3) == false);    // "discover! ", ""

  // ==========
  // mu_substr_t *mu_substr_append(mu_substr_t *dst, mu_substr_t *src);
  reset();
  mu_substr_duplicate(ss3, ss1);
  mu_substr_clear(ss2);
  ASSERT(mu_substr_slice_str(ss3, 0, 5) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_append(ss2, ss3) == ss2);
  ASSERT(mu_substr_slice_str(ss3, -1, mu_substr_end(ss1)) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_append(ss2, ss3) == ss2);
  ASSERT(mu_substr_slice_str(ss3, 1, 3) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_append(ss2, ss3) == ss2);
  ASSERT(mu_substr_slice_str(ss3, -1, mu_substr_end(ss1)) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_append(ss2, ss3) == ss2);
  ASSERT(mu_substr_slice_str(ss3, 4, 9) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_append(ss2, ss3) == ss2);
  ASSERT(substr_equals(ss2, "disco is over!") == true);

  // ==========
  // mu_substr_t *mu_substr_strstr(mu_substr_t *dst, mu_substr_t *needle, mu_substr_t *haystack);
  reset();
  // give ss2 a copy of ss1's data
  mu_substr_copy(ss2, ss1);

  // succesful search
  mu_substr_slice_str(ss2, 1, 3);  // "is"
  ASSERT(mu_substr_strstr(ss3, ss2, ss1) == ss3);  // "is", "discover! "
  ASSERT(mu_substr_equals(ss3, ss2) == true);

  // needle is larger than haystack
  ASSERT(mu_substr_strstr(ss3, ss1, ss2) == ss3);  // "discover! ", "is"
  ASSERT(mu_substr_start(ss3) == 0);
  ASSERT(mu_substr_end(ss3) == 0);

  // haystack starts > 0
  mu_substr_slice_str(ss1, 3, 8);  // "cover"
  mu_substr_slice_str(ss2, 3, 7);  // "cove"
  ASSERT(mu_substr_strstr(ss3, ss2, ss1) == ss3);  // "cove" in "cover"
  ASSERT(mu_substr_equals(ss3, ss2) == true);

  // =========
  // mu_substr_t *mu_substr_to_cstr(mu_substr_t *src, char *cstr, size_t cstr_length)
  reset();
  ASSERT(mu_substr_to_cstr(ss1, s_cstr2, ELEMENT_COUNT) == ss1);
  ASSERT(strcmp("discover! ", s_cstr2) == 0);
  reset();
  ASSERT(mu_substr_to_cstr(ss1, s_cstr2, 5) == ss1);
  ASSERT(strcmp("disc", s_cstr2) == 0);
}

// =============================================================================
// private code

static void reset(void) {
  mu_str_init_from_cstr(&s_str1, s_cstr1);
  mu_substr_init(&s_sstr1, &s_str1);

  memset(s_cstr2, '\0', ELEMENT_COUNT);
  mu_str_init(&s_str2, s_cstr2, ELEMENT_COUNT);
  mu_substr_init(&s_sstr2, &s_str2);
}

static bool substr_equals(mu_substr_t *substr, const char *cstr) {
  char *s;
  int to_compare = strlen(cstr);
  if (to_compare > mu_substr_length(substr)) {
    to_compare = mu_substr_length(substr);
  }
  // Get a pointer to substr's string and compare againt cstr
  if (mu_substr_ref(substr, 0, &s) == MU_SUBSTR_ERR_NONE) {
    return strncmp(s, cstr, to_compare) == 0;
  } else{
    return false;
  }
}
