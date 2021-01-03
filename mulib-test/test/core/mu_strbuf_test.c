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
#include "mulib.h"
#include <string.h>
#include <stdint.h>

// =============================================================================
// private types and definitions

#define ELEMENT_COUNT 10

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_strbuf_test() {
  // allocate read_only and read/write string buffers, with pointers...
  mu_strbuf_t strbuf_ro;
  mu_strbuf_t strbuf_wr;
  mu_strbuf_t *s_ro = &strbuf_ro;
  mu_strbuf_t *s_wr = &strbuf_wr;
  // allocate some C-strings for testing.
  const char cstr_ro[] = "the quick brown fox jumps over the lazy dog.";
  uint8_t cstr_wr[ELEMENT_COUNT];

  ASSERT(mu_strbuf_init_ro(s_ro, (const uint8_t *)cstr_ro, strlen(cstr_ro)) == s_ro);
  ASSERT(mu_strbuf_capacity(s_ro) == strlen(cstr_ro));
  ASSERT(mu_strbuf_rdata(s_ro) == (const uint8_t *)cstr_ro);

  ASSERT(mu_strbuf_init_wr(s_wr, cstr_wr, ELEMENT_COUNT) == s_wr);
  ASSERT(mu_strbuf_capacity(s_wr) == ELEMENT_COUNT);
  ASSERT(mu_strbuf_wdata(s_wr) == cstr_wr);

  ASSERT(mu_strbuf_init_from_cstr(s_ro, cstr_ro) == s_ro);
  ASSERT(mu_strbuf_capacity(s_ro) == strlen(cstr_ro));
  ASSERT(mu_strbuf_rdata(s_ro) == (const uint8_t *)cstr_ro);
}

// =============================================================================
// private code
