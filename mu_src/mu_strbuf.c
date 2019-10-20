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

mu_strbuf_t *mu_strbuf_init(mu_strbuf_t *str, char *buf, size_t capacity) {
  str->data = buf;
  str->capacity = capacity;
  return mu_strbuf_reset(str);
}

mu_strbuf_t *mu_strbuf_reset(mu_strbuf_t *str) {
  str->length = 0;
  str->data[0] = '\0';
  return str;
}

size_t mu_strbuf_capacity(mu_strbuf_t *str) { return str->capacity; }

// Return the length of the accumulated string.  Never exceeds capacity-1
// since mu_strbuf_append() always terminates the string with a '\0'.
size_t mu_strbuf_length(mu_strbuf_t *str) {
  return MIN(str->length, str->capacity - 1);
}

size_t mu_strbuf_append(mu_strbuf_t *str, const char *fmt, ...) {
  va_list ap;
  size_t available = str->capacity - str->length;

  va_start(ap, fmt);
  // append no more than `available` chars to the end of the buffer
  int written = vsnprintf(&(str->data[str->length]), available, fmt, ap);
  va_end(ap);

  str->length += (written < available) ? written : available;
  return mu_strbuf_length(str);
}

char *mu_strbuf_data(mu_strbuf_t *str) { return str->data; }

// =============================================================================
// private code
