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

#include "mu_strbuf.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

// =============================================================================
// local storage

// =============================================================================
// public code

mu_strbuf_t *mu_strbuf_init(mu_strbuf_t *sb, char *buf, size_t capacity) {
  sb->data = buf;
  sb->capacity = capacity-1;  // last byte is always reserved for '\0'
  buf[sb->capacity] = '\0';
  return mu_strbuf_reset(sb);
}

mu_strbuf_t *mu_strbuf_reset(mu_strbuf_t *sb) {
  sb->length = 0;
  sb->data[0] = '\0';
  return sb;
}

size_t mu_strbuf_capacity(mu_strbuf_t *sb) { return sb->capacity; }

size_t mu_strbuf_length(mu_strbuf_t *sb) {
  return sb->length;
}

size_t mu_strbuf_available(mu_strbuf_t *sb) {
  return sb->capacity - sb->length;
}

size_t mu_strbuf_append(mu_strbuf_t *sb, const char *s) {
  size_t n_available = mu_strbuf_available(sb);
  size_t n_written = strlen(s);
  n_written = MIN(n_written, n_available);

  strncpy(&(sb->data[sb->length]), s, n_written);
  sb->length += n_written;

  return n_written;
}

size_t mu_strbuf_printf(mu_strbuf_t *sb, const char *fmt, ...) {
  va_list ap;
  size_t n_available = mu_strbuf_available(sb);
  va_start(ap, fmt);
  // append no more than `available` chars to the end of the buffer
  // (+1 for a null byte)
  int n_written = vsnprintf(&(sb->data[sb->length]), n_available+1, fmt, ap);
  va_end(ap);

  sb->length += (n_written < n_available) ? n_written : n_available;
  return mu_strbuf_length(sb);
}

char *mu_strbuf_data(mu_strbuf_t *sb) { return sb->data; }

// =============================================================================
// private code
