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
#include "mu_bufx.h"
#include <string.h>
#include <stdlib.h>

// =============================================================================
// private types and definitions

#define BUF_SIZE 10

// =============================================================================
// private declarations

static int test_equal(mu_bufx_t *dst, const char *expected);

// =============================================================================
// local storage

static const mu_bufx_item_t s_ro_items[BUF_SIZE];
static mu_bufx_item_t s_rw_items[BUF_SIZE];
static const char const *s_ro_cstr = "the quick brown fox jumps over the lazy dog";

// =============================================================================
// public code

void mu_bufx_test() {
  mu_bufx_t robuf1_i, robuf2_i, rwbuf1_i, rwbuf2_i;
  mu_bufx_t *robuf1 = &robuf1_i;
  mu_bufx_t *robuf2 = &robuf2_i;
  mu_bufx_t *rwbuf1 = &rwbuf1_i;
  mu_bufx_t *rwbuf2 = &rwbuf2_i;
  size_t cstr_len = strlen(s_ro_cstr);

  // mu_robuf_init()
  ASSERT(mu_robuf_init(robuf1, s_ro_items, BUF_SIZE) == robuf1);
  ASSERT(mu_bufx_capacity(robuf1) == BUF_SIZE);
  ASSERT(mu_bufx_start(robuf1) == 0);
  ASSERT(mu_bufx_end(robuf1) == BUF_SIZE);
  ASSERT(mu_robuf_items(robuf1) == s_ro_items);
  ASSERT(mu_bufx_length(robuf1) == BUF_SIZE);

  // mu_robuf_init_from_cstr()
  ASSERT(mu_robuf_init_from_cstr(robuf2, s_ro_cstr) == robuf2);
  ASSERT(mu_bufx_capacity(robuf2) == cstr_len);
  ASSERT(mu_bufx_start(robuf2) == 0);
  ASSERT(mu_bufx_end(robuf2) == cstr_len);
  ASSERT(mu_robuf_items(robuf2) == (mu_bufx_item_t *)s_ro_cstr);
  ASSERT(mu_bufx_length(robuf2) == cstr_len);

  // mu_rwbuf_init()
  ASSERT(mu_rwbuf_init(rwbuf1, s_rw_items, BUF_SIZE) == rwbuf1);
  ASSERT(mu_bufx_capacity(rwbuf1) == BUF_SIZE);
  ASSERT(mu_bufx_start(rwbuf1) == 0);
  ASSERT(mu_bufx_end(rwbuf1) == 0);
  ASSERT(mu_rwbuf_items(rwbuf1) == s_rw_items);
  ASSERT(mu_bufx_length(rwbuf1) == 0);

  //mu_bufx_copy()
  ASSERT(mu_bufx_copy(rwbuf2, rwbuf1) == rwbuf2);
  ASSERT(mu_bufx_capacity(rwbuf2) == mu_bufx_capacity(rwbuf1));
  ASSERT(mu_bufx_start(rwbuf2) == mu_bufx_start(rwbuf1));
  ASSERT(mu_bufx_end(rwbuf2) == mu_bufx_end(rwbuf1));
  ASSERT(mu_rwbuf_items(rwbuf2) == mu_rwbuf_items(rwbuf1));

  // mu_bufx_cmp()
  ASSERT(mu_bufx_cmp(mu_robuf_init_from_cstr(robuf1, "aaa"),
                    mu_robuf_init_from_cstr(robuf2, "ccc")) < 0);
  ASSERT(mu_bufx_cmp(mu_robuf_init_from_cstr(robuf1, "ccc"),
                    mu_robuf_init_from_cstr(robuf2, "aaa")) > 0);
  ASSERT(mu_bufx_cmp(mu_robuf_init_from_cstr(robuf1, "bbb"),
                    mu_robuf_init_from_cstr(robuf2, "bbb")) == 0);
  ASSERT(mu_bufx_cmp(mu_robuf_init_from_cstr(robuf1, "abc"),
                    mu_robuf_init_from_cstr(robuf2, "ab")) > 0);
  ASSERT(mu_bufx_cmp(mu_robuf_init_from_cstr(robuf1, "ab"),
                    mu_robuf_init_from_cstr(robuf2, "abc")) < 0);
  ASSERT(mu_bufx_cmp(mu_robuf_init_from_cstr(robuf1, ""),
                    mu_robuf_init_from_cstr(robuf2, "anything")) < 0);
  ASSERT(mu_bufx_cmp(mu_robuf_init_from_cstr(robuf1, "anything"),
                    mu_robuf_init_from_cstr(robuf2, "")) > 0);

  // mu_bufx_slice()
  mu_robuf_init_from_cstr(robuf1, "discovers");
  ASSERT(test_equal(mu_bufx_slice(robuf2, robuf1, 0, -1), "discovers") == 0);
  ASSERT(test_equal(mu_bufx_slice(robuf2, robuf1, 0, 5), "disco") == 0);
  ASSERT(test_equal(mu_bufx_slice(robuf2, robuf1, 3, 8), "cover") == 0);
  ASSERT(test_equal(mu_bufx_slice(robuf2, robuf1, -7, -2), "cover") == 0);
  ASSERT(test_equal(mu_bufx_slice(robuf2, robuf1, 1, -7), "is") == 0);
  ASSERT(test_equal(mu_bufx_slice(robuf2, robuf1, 0, 0), "") == 0);

  // mu_bufx_find()
  mu_robuf_init_from_cstr(robuf1, "discovers");

  mu_robuf_init_from_cstr(robuf2, "cove");
  ASSERT(mu_bufx_find(rwbuf1, robuf1, robuf2) == rwbuf1);
  ASSERT(mu_bufx_start(rwbuf1) == 3);
  ASSERT(mu_rwbuf_items(rwbuf1) == mu_robuf_items(robuf1));

  mu_robuf_init_from_cstr(robuf2, "cave");
  ASSERT(mu_bufx_find(rwbuf1, robuf1, robuf2) == rwbuf1);
  ASSERT(mu_bufx_length(rwbuf1) == 0);
}

// =============================================================================
// private code

static int test_equal(mu_bufx_t *dst, const char *expected) {
  mu_bufx_t robuf;
  mu_robuf_init_from_cstr(&robuf, expected);
  return mu_bufx_cmp(dst, mu_robuf_init_from_cstr(&robuf, expected));
}
