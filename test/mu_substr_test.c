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
#include "mu_substr.h"
#include "mu_buf.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>
// =============================================================================
// private types and definitions

#define ELEMENT_COUNT 15

// =============================================================================
// private declarations

static void reset();
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

  reset();
  ASSERT(mu_substr_duplicate(ss3, ss1) == ss3);
  ASSERT(mu_substr_str(ss3) == mu_substr_str(ss1));
  ASSERT(mu_substr_start(ss3) == mu_substr_start(ss1));
  ASSERT(mu_substr_end(ss3) == mu_substr_end(ss1));

  reset();
  // trim 3 chars from front and 3 chars from end
  ASSERT(mu_substr_trim(ss1, 3, 3) == ss1);
  ASSERT(mu_substr_start(ss1) == 3);
  ASSERT(mu_substr_end(ss1) == mu_str_capacity(&s_str1) - 3);
  ASSERT(substr_equals(ss1, "cove") == true);

  ASSERT(mu_substr_ref(ss1, 0, &pchr) == MU_SUBSTR_ERR_NONE);
  ASSERT(*pchr == 'c');
  ASSERT(mu_substr_ref(ss1, 3, &pchr) == MU_SUBSTR_ERR_NONE);
  ASSERT(*pchr == 'e');
  ASSERT(mu_substr_ref(ss1, 4, &pchr) == MU_SUBSTR_ERR_INDEX);

  ASSERT(mu_substr_get(ss1, 0, &chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(chr == 'c');
  ASSERT(mu_substr_get(ss1, 3, &chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(chr == 'e');
  ASSERT(mu_substr_get(ss1, 4, &chr) == MU_SUBSTR_ERR_INDEX);

  chr = 'd';
  ASSERT(mu_substr_put(ss1, 0, chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(mu_substr_put(ss1, 4, chr) == MU_SUBSTR_ERR_INDEX);
  ASSERT(substr_equals(ss1, "dove") == true);

  // restore underlying string
  chr = 'c';
  ASSERT(mu_substr_put(ss1, 0, chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(substr_equals(ss1, "cove") == true);

  reset();
  ASSERT(mu_substr_copy(ss2, ss1) == ss2);
  ASSERT(substr_equals(ss2, "discover! ") == true);

  reset();
  // give ss2 its own copy of the discover string
  ASSERT(mu_substr_copy(ss2, ss1) == ss2);
  ASSERT(substr_equals(ss2, "discover! ") == true);
  ASSERT(mu_substr_trim(ss2, 4, 2) == ss2);
  ASSERT(substr_equals(ss2, "over") == true);
  ASSERT(mu_substr_cmp(ss1, ss2) < 0);  // 'd' < 'o'
  ASSERT(mu_substr_equal(ss1, ss2) == false);

  ASSERT(mu_substr_trim(ss1, 2, 0) == ss1);
  ASSERT(substr_equals(ss1, "scover! ") == true);
  ASSERT(mu_substr_cmp(ss1, ss2) > 0);  // 's' > 'o'
  ASSERT(mu_substr_equal(ss1, ss2) == false);

  ASSERT(mu_substr_trim(ss1, 2, 0) == ss1);
  ASSERT(substr_equals(ss1, "over! ") == true);
  ASSERT(mu_substr_cmp(ss1, ss2) == 0);  // 'o' == 'o'
  ASSERT(mu_substr_equal(ss1, ss2) == true);

  reset();
  // give ss2 its own copy of the discover string
  ASSERT(mu_substr_copy(ss2, ss1) == ss2);
  ASSERT(substr_equals(ss2, "discover! ") == true);
  ASSERT(mu_substr_trim(ss2, 3, 3) == ss2);
  ASSERT(substr_equals(ss2, "cove") == true);
  ASSERT(mu_substr_strstr(ss3, ss2, ss1) == ss3);
  substr_equals(ss3, "cove");
  ASSERT(mu_substr_equal(ss3, ss2) == true);
  // bash a character in 'needle'
  chr = 'd';
  ASSERT(mu_substr_put(ss2, 0, chr) == MU_SUBSTR_ERR_NONE);
  ASSERT(substr_equals(ss2, "cove") == false);
  ASSERT(substr_equals(ss2, "dove") == true);
  ASSERT(mu_substr_strstr(ss3, ss2, ss1) == ss3);
  // TODO: does strcmp("", any_string) == 0?  should mu_substr_cmp() do the same?
  // ASSERT(mu_substr_equal(ss3, ss2) == false);
  ASSERT(mu_substr_length(ss3) == 0);

  reset();
  ASSERT(mu_substr_trim(ss1, 1, 7) == ss1);
  ASSERT(substr_equals(ss1, "is") == true);
  ASSERT(mu_substr_to_cstr(ss1, s_cstr2, ELEMENT_COUNT) == ss1);
  ASSERT(strcmp(s_cstr2, "is") == 0);
}

// =============================================================================
// private code

static void reset(void) {
  mu_str_init_from_cstr(&s_str1, s_cstr1);
  mu_substr_init(&s_sstr1, &s_str1);

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
