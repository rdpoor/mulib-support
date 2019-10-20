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

#include "../src/mu_strbuf.h"
#include "test_utilities.h"
#include <string.h>

// =============================================================================
// private types and definitions

#define BUF_SIZE 10

// =============================================================================
// private declarations

// =============================================================================
// local storage

static char s_buf[BUF_SIZE];

// =============================================================================
// public code

void mu_strbuf_test() {
  mu_strbuf_t str;

  memset(s_buf, '5', BUF_SIZE);  // make sure null chars get written

  UTEST_ASSERTEQ_PTR(mu_strbuf_init(&str, s_buf, BUF_SIZE), &str);
  UTEST_ASSERTEQ_INT(mu_strbuf_capacity(&str), BUF_SIZE);
  UTEST_ASSERTEQ_INT(mu_strbuf_length(&str), 0);
  UTEST_ASSERTEQ_STR(mu_strbuf_data(&str), "");

  UTEST_ASSERT(s_buf[0] == '\0');

  UTEST_ASSERTEQ_INT(mu_strbuf_append(&str, "%s %s", "one", "two"), 7);
  UTEST_ASSERTEQ_INT(mu_strbuf_length(&str), 7);
  UTEST_ASSERTEQ_STR(mu_strbuf_data(&str), "one two");

  UTEST_ASSERT(s_buf[7] == '\0');

  // mu_strbuf_printf() will not write beyond buf[BUF_SIZE-1] -- the last char is
  // always a null.
  UTEST_ASSERTEQ_INT(mu_strbuf_append(&str, " %s", "three"), 9);
  UTEST_ASSERTEQ_INT(mu_strbuf_length(&str), 9);
  UTEST_ASSERTEQ_STR(mu_strbuf_data(&str), "one two t");

  // Assure that terminating '\0' got written
  UTEST_ASSERT(s_buf[BUF_SIZE-1] == '\0');

  UTEST_ASSERTEQ_PTR(mu_strbuf_reset(&str), &str);
  UTEST_ASSERTEQ_INT(mu_strbuf_length(&str), 0);
  UTEST_ASSERTEQ_STR(mu_strbuf_data(&str), "");

  UTEST_ASSERT(s_buf[0] == '\0');

  // Appending a null string should not change length
  UTEST_ASSERTEQ_INT(mu_strbuf_append(&str, "%s", ""), 0);
  UTEST_ASSERTEQ_INT(mu_strbuf_length(&str), 0);
  UTEST_ASSERTEQ_STR(mu_strbuf_data(&str), "");
}

// =============================================================================
// private code
