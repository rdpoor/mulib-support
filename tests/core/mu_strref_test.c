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
#include "mu_strref.h"
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
static bool strref_equals(mu_strref_t *r, const char *s);

// =============================================================================
// local storage

static char s_cstr1[] = "discover! ";
static char s_cstr2[ELEMENT_COUNT];

static mu_strbuf_t s_strbuf1, s_strbuf2;

static mu_strref_t s_strref1, s_strref2;

// =============================================================================
// public code

void mu_strref_test() {
  mu_strref_t *sr1 = &s_strref1;
  mu_strref_t *sr2 = &s_strref2;
  char chr;

  reset();
  ASSERT(mu_strref_strbuf(sr1) == &s_strbuf1);
  ASSERT(mu_strref_is_readonly(sr1) == true);
  ASSERT(mu_strref_capacity(sr1) == mu_strbuf_capacity(&s_strbuf1));
  ASSERT(mu_strref_can_get(sr1) == true);
  ASSERT(mu_strref_can_put(sr1) == false);
  ASSERT(strref_equals(sr1, "discover! "));

  ASSERT(mu_strref_strbuf(sr2) == &s_strbuf2);
  ASSERT(mu_strref_is_readonly(sr2) == false);
  ASSERT(mu_strref_capacity(sr2) == mu_strbuf_capacity(&s_strbuf2) - 1);
  ASSERT(mu_strref_can_get(sr2) == false);
  ASSERT(mu_strref_can_put(sr2) == true);
  ASSERT(strref_equals(sr2, ""));

  // mu_strref_get()
  reset();
  for (int i=0; i<strlen(s_cstr1); i++) {
    ASSERT(mu_strref_get(sr1, &chr) == MU_STRREF_ERR_NONE);
    ASSERT(chr == s_cstr1[i]);
  }
  ASSERT(mu_strref_get(sr1, &chr) == MU_STRREF_ERR_EMPTY);
  ASSERT(chr == '\0');

  ASSERT(mu_strref_get(sr2, &chr) == MU_STRREF_ERR_EMPTY);
  ASSERT(chr == '\0');

  // mu_strref_put()
  reset();
  ASSERT(mu_strref_put(sr1, 'a') == MU_STRREF_ERR_FULL);
  ASSERT(strref_equals(sr1, "discover! "));
  ASSERT(mu_strref_put(sr2, 'a') == MU_STRREF_ERR_NONE);
  ASSERT(strref_equals(sr2, "a"));

  // mu_strref_puts()
  reset();
  ASSERT(mu_strref_puts(sr1, "we ") == 0);
  ASSERT(mu_strref_puts(sr2, "we ") == 3);
  ASSERT(strref_equals(sr2, "we "));

  // mu_strref_append()
  ASSERT(mu_strref_append(sr1, sr2) == 0);   // cannot append to readonly str
  ASSERT(strref_equals(sr1, "discover! "));

  reset();
  ASSERT(mu_strref_puts(sr2, "we ") == 3);
  ASSERT(mu_strref_append(sr2, sr1) == strlen(s_cstr1));
  ASSERT(strref_equals(sr2, "we discover! "));

  // mu_strref_append() with wrapping around...
  reset();
  // first advance the put index...
  for (int i=0; i<ELEMENT_COUNT-1; i++) {
    ASSERT(mu_strref_put(sr2, 'a') == MU_STRREF_ERR_NONE);
  }
  ASSERT(mu_strref_can_put(sr2) == false);  // should be full
  // now advanec the get index
  for (int i=0; i<ELEMENT_COUNT-1; i++) {
    ASSERT(mu_strref_get(sr2, &chr) == MU_STRREF_ERR_NONE);
  }
  ASSERT(mu_strref_can_get(sr2) == false);  // should be empty
  // Verify that sr2->get_i == sr2->put_i == ELEMENT_COUNT-1
  ASSERT(sr2->get_i == ELEMENT_COUNT-1);
  ASSERT(sr2->put_i == ELEMENT_COUNT-1);
  // Now test puts() with wrapping around
  ASSERT(mu_strref_puts(sr2, s_cstr1) == strlen(s_cstr1));
  ASSERT(strref_equals(sr2, s_cstr1));
}

// =============================================================================
// private code

static void reset(void) {
  mu_strbuf_init_from_cstr(&s_strbuf1, s_cstr1);
  mu_strref_init(&s_strref1, &s_strbuf1, true);

  memset(s_cstr2, '\0', ELEMENT_COUNT);
  mu_strbuf_init(&s_strbuf2, s_cstr2, ELEMENT_COUNT);
  mu_strref_init(&s_strref2, &s_strbuf2, false);
}

static bool strref_equals(mu_strref_t *r, const char *s) {
  char d;
  while (*s != '\0') {
    if (mu_strref_get(r, &d) != MU_STRREF_ERR_NONE) {
      return false;
    }
    if (d != *s++) {
      return false;
    }
  }
  return true;
}
