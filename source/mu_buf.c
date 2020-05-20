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

#include "mu_buf.h"
#include <string.h>
#include <stdlib.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

// =============================================================================
// public code

mu_buf_t *mu_robuf_init(mu_buf_t *buf,
                        const mu_buf_item_t const *items,
                        size_t capacity) {
  buf->ro_items = items;
  buf->capacity = capacity;
  return mu_robuf_reset(buf);
}

mu_buf_t *mu_robuf_init_from_cstr(mu_buf_t *buf, const char *cstr) {
  return mu_robuf_init(buf, cstr, strlen(cstr));
}

mu_buf_t *mu_robuf_reset(mu_buf_t *buf) {
  buf->start = 0;
  buf->end = buf->capacity;
  return buf;
}

mu_buf_t *mu_rwbuf_init(mu_buf_t *buf,
                       const mu_buf_item_t const *items,
                       size_t capacity) {
  buf->rw_items = items;
  buf->capacity = capacity;
  return mu_rwbuf_reset(buf);
}

mu_buf_t *mu_rwbuf_reset(mu_buf_t *buf) {
  buf->start = 0;
  buf->end = 0;
  return buf;
}

const mu_buf_item_t const *mu_robuf_items(mu_buf_t *buf) {
  return buf->ro_items;
}

mu_buf_item_t const *mu_rwbuf_items(mu_buf_t *buf) {
  return buf->rw_items;
}

size_t mu_buf_capacity(mu_buf_t *buf) {
  return buf->capacity;
}

int mu_buf_start(mu_buf_t *buf) {
  return buf->start;
}

int mu_buf_end(mu_buf_t *buf) {
  return buf->end;
}

size_t mu_buf_length(mu_buf_t *buf) {
  return  buf->end - buf->start;
}

// =============================================================================
// local (static) code
