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
#include "core/mu_str.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// =============================================================================
// private types and definitions

#define ELEMENT_COUNT 15

// =============================================================================
// private declarations

static void reset(void);
static bool test_str_equals(mu_str_t *r, const char *s);

// =============================================================================
// local storage

static char s_cstr_ro[] = "Discover! ";
static uint8_t s_cstr_wr[ELEMENT_COUNT];

static mu_strbuf_t s_strbuf_ro;
static mu_strbuf_t s_strbuf_wr;
static mu_str_t s_str_ro;
static mu_str_t s_str_wr;

// =============================================================================
// public code

void mu_str_test() {
  mu_str_t *s_ro = &s_str_ro;
  mu_str_t *s_wr = &s_str_wr;
  mu_str_t s_str_ro2;
  mu_str_t *s_ro2 = &s_str_ro2;
  uint8_t ch;

  reset();

  for (int i=0; i<strlen(s_cstr_ro); i++) {
    ASSERT(mu_str_read_available(s_ro) == strlen(s_cstr_ro)-i);
    ASSERT(mu_str_read_ref(s_ro) == (uint8_t *)&s_cstr_ro[i]);
    ASSERT(mu_str_read_byte(s_ro, &ch) == true);
    ASSERT(ch == s_cstr_ro[i]);
  }
  // reading off the end returns false...
  ASSERT(mu_str_read_available(s_ro) == 0);
  ASSERT(mu_str_read_byte(s_ro, &ch) == false);

  for (int i=0; i<ELEMENT_COUNT; i++) {
    ASSERT(mu_str_write_available(s_wr) == ELEMENT_COUNT-i);
    ASSERT(mu_str_write_ref(s_wr) == (uint8_t *)&s_cstr_wr[i]);
    ASSERT(mu_str_write_byte(s_wr, ch) == true);
  }
  // reading off the end returns false...
  ASSERT(mu_str_write_available(s_wr) == 0);
  ASSERT(mu_str_write_byte(s_wr, ch) == false);

  reset();
  ASSERT(mu_str_copy(s_ro2, s_ro) == s_ro2);
  ASSERT(mu_str_read_available(s_ro2) == mu_str_read_available(s_ro));
  ASSERT(mu_str_read_ref(s_ro2) == mu_str_read_ref(s_ro));

  reset();
  ASSERT(mu_str_slice(s_ro2, s_ro, 3, -2) == s_ro2);
  ASSERT(test_str_equals(s_ro2, "cover"));

  reset();
  // an amusing test for mu_str_slice...
  ASSERT(test_str_equals(s_wr, ""));
  ASSERT(mu_str_slice(s_ro2, s_ro, 0, 5) == s_ro2);
  ASSERT(mu_str_append(s_wr, s_ro2) == 5);
  ASSERT(test_str_equals(s_wr, "Disco"));
  ASSERT(mu_str_slice(s_ro2, s_ro, -1, 10) == s_ro2);
  ASSERT(mu_str_append(s_wr, s_ro2) == 1);
  ASSERT(test_str_equals(s_wr, "Disco "));
  ASSERT(mu_str_slice(s_ro2, s_ro, 1, 3) == s_ro2);
  ASSERT(mu_str_append(s_wr, s_ro2) == 2);
  ASSERT(test_str_equals(s_wr, "Disco is"));
  ASSERT(mu_str_slice(s_ro2, s_ro, -1, 10) == s_ro2);
  ASSERT(mu_str_append(s_wr, s_ro2) == 1);
  ASSERT(test_str_equals(s_wr, "Disco is "));
  ASSERT(mu_str_slice(s_ro2, s_ro, 4, -1) == s_ro2);
  ASSERT(mu_str_append(s_wr, s_ro2) == 5);
  ASSERT(test_str_equals(s_wr, "Disco is over!"));

  reset();
  ASSERT(mu_str_append_cstr(s_wr, "Punk lives!") == 11);
  ASSERT(test_str_equals(s_wr, "Punk lives!"));

  reset();
  ASSERT(mu_str_printf(s_wr, "Dec %d=Oct %o", 25, 25) == 13);
  ASSERT(test_str_equals(s_wr, "Dec 25=Oct 31"));
}

// =============================================================================
// private code

static void reset(void) {
  mu_strbuf_init_from_cstr(&s_strbuf_ro, s_cstr_ro);
  mu_str_init_for_read(&s_str_ro, &s_strbuf_ro);

  memset(s_cstr_wr, '\0', ELEMENT_COUNT);
  mu_strbuf_init_wr(&s_strbuf_wr, s_cstr_wr, ELEMENT_COUNT);
  mu_str_init_for_write(&s_str_wr, &s_strbuf_wr);
}

static bool test_str_equals(mu_str_t *r, const char *s1) {
  size_t n = mu_str_read_available(r);
  if (strlen(s1) < n) {
    n = strlen(s1);
  }
  const char *s2 = (const char *)mu_str_read_ref(r);
  return strncmp(s1, s2, n) == 0;
}
