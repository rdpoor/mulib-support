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

#include "mu_string.h"
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

// initialize a mu_string from a C string
mu_string_t *mu_string_init(mu_string_t *s, mu_cstring_t *buf, size_t buf_length) {
  s->buf = buf;
  s->buf_length = buf_length;
  return mu_string_reset(s);
}

// return the underlying C string
mu_cstring_t *mu_string_buf(mu_string_t *s) {
  return s->buf;
}

// return the length of the underlying C string
size_t mu_string_buf_length(mu_string_t *s) {
  return s->buf_length;
}

// reset start = end = 0
mu_string_t *mu_string_reset(mu_string_t *s) {
  s->start = s->end = 0;
  return s;
}

// return the start index
int mu_string_start(mu_string_t *s) {
  return s->start;
}

// return the start index
int mu_string_end(mu_string_t *s) {
  return s->end;
}

mu_cstring_t *mu_string_cstring(mu_string_t *s) {
  return &(s->buf[s->start]);
}

// return i1-i0
int mu_string_length(mu_string_t *s) {
  return s->end - s->start;
}

// return length - i1
size_t mu_string_available(mu_string_t *s) {
  return s->buf_length - s->end;
}

// compare substring with a c string
int mu_string_cmp(mu_string_t *s, const mu_cstring_t *cstring) {
  return strncmp((const char *)mu_string_cstring(s), (const char *)cstring, mu_string_length(s));
}

// return true if the string equals the given string
bool mu_string_eq(mu_string_t *s, const mu_cstring_t *cstring) {
  return mu_string_cmp(s, cstring) == 0;
}

// make a copy of s into dst (shallow copy - doesn't copy chars)
mu_string_t *mu_string_duplicate(mu_string_t *dst, mu_string_t *src) {
  memcpy(dst, src, sizeof(mu_string_t));
  return dst;
}

// The following manipulate the string object s directly unless a dst object is
// provided.

// take a string of the underlying string.  negative end counts from end.
mu_string_t *mu_string_slice(mu_string_t *s, int start, int end, mu_string_t *dst) {
  mu_string_t *d = dst ? mu_string_duplicate(dst, s) : s;
  d->start = start;
  d->end = (end >= 0) ? end : mu_string_buf_length(d) - end;
  return d;
}

// Find cstring within s.  Returns null if not found, else return sliced results
// in dst (if given) or s (if not).
mu_string_t *mu_string_find(mu_string_t *s, const mu_cstring_t *cstring, mu_string_t *dst) {
  mu_cstring_t *found = (mu_cstring_t *)strstr((const char *)mu_string_cstring(s), (const char *)cstring);
  if (found == NULL) {
    return NULL;
  }
  // make sure that found lies within the [start, end] boundaries
  size_t len = strlen((const char *)cstring);
  int s1 = found - mu_string_cstring(s);  // starting index of found string
  int e1 = s1 + len;                      // ending index of found string
  if (e1 > mu_string_end(s)) {
    return NULL;                          // found string exceeded bounds of s
  }

  // prepare and return result
  return mu_string_slice(s, s1, e1, dst);
}

// the following functions copy bytes:

// Copy data from string[i0] to string[i1] (plus null termination) into c_str.
mu_cstring_t *mu_string_extract(mu_string_t *s, mu_cstring_t *c_str, size_t c_str_length) {
  int to_copy = mu_string_length(s);

  if (to_copy > c_str_length) {
    to_copy = c_str_length;
  }
  strncpy((char *)c_str, (const char *)mu_string_cstring(s), to_copy);

  // null terminate if there is room
  if (to_copy < c_str_length) {
    c_str[to_copy] = '\0';
  }

  return c_str;
}

// append string referred to by src onto dst
mu_string_t *mu_string_append(mu_string_t *dst, mu_string_t *src) {
  size_t n_available = mu_string_available(dst);
  int to_copy = mu_string_length(src);
  if (to_copy > n_available) {
    to_copy = n_available;
  }
  strncpy((char *)&(dst->buf[dst->end]), (const char *)mu_string_cstring(src), to_copy);
  dst->end += to_copy;

  return dst;
}

// sprintf() into &string[i1]
mu_string_t *mu_string_sprintf(mu_string_t *s, const char *fmt, ...) {
  va_list ap;
  size_t n_available = mu_string_available(s);
  va_start(ap, fmt);
  // append no more than `available` chars to the end of the buffer
  // (+1 for a null byte)
  int n_written = vsnprintf((char *)&(s->buf[s->end]), n_available+1, fmt, ap);
  va_end(ap);

  s->end += (n_written < n_available) ? n_written : n_available;
  return s;
}

// =============================================================================
// private code
