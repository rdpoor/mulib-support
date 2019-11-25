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
#include <stdio.h>
#include <string.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

int clamp(int lo, int val, int hi);

mu_string_t* init_or_slice(mu_string_t* s, int start, int end, mu_string_t* d);

// =============================================================================
// local storage

// =============================================================================
// public code

// initialize a mu_string (writeable).  Sets start and end to 0.
mu_string_t* mu_string_init(mu_string_t* s,
                            mu_string_data_t* buf,
                            size_t buf_length) {
  s->mbuf = buf;
  s->capacity = buf_length;
  return mu_string_reset(s);
}

// initialize a mu_cstring (read-only).  Sets start to 0 and end to buf_length.
mu_string_t* mu_cstring_init(mu_string_t* s,
                             const mu_string_data_t* buf,
                             size_t buf_length) {
  s->cbuf = buf;
  s->capacity = buf_length;
  return mu_cstring_reset(s);
}

// return the underlying buffer.  Use with caution.
mu_string_data_t* mu_string_buf(mu_string_t* s) {
  return s->mbuf;
}

const mu_string_data_t* mu_cstring_buf(mu_string_t* s) {
  return s->cbuf;
}

// return the length of the underlying buffer
size_t mu_string_capacity(mu_string_t* s) {
  return s->capacity;
}

// reset start = end = 0
mu_string_t* mu_string_reset(mu_string_t* s) {
  s->start = 0;
  s->end = 0;
  return s;
}

// reset start = 0, end = capacity
mu_string_t* mu_cstring_reset(mu_string_t* s) {
  s->start = 0;
  s->end = s->capacity;
  return s;
}

// return the start index
int mu_string_start(mu_string_t* s) {
  return s->start;
}

// return the end index
int mu_string_end(mu_string_t* s) {
  return s->end;
}

// return &buf[start]
mu_string_data_t* mu_string_data(mu_string_t* s) {
  return &s->mbuf[s->start];
}

const mu_string_data_t* mu_cstring_data(mu_string_t* s) {
  return &s->cbuf[s->start];
}

// return end-start
int mu_string_length(mu_string_t* s) {
  return s->end - s->start;
}

// return capacity-end.
size_t mu_string_available(mu_string_t* s) {
  return s->capacity - s->end;
}

// compare substring with a c string
int mu_string_cmp(mu_string_t* s, const mu_string_data_t* cstring) {
  int dlen = mu_string_length(s) - strlen(cstring);
  if (dlen != 0) {
    return dlen;   // string lengths differ
  } else {
    // A  char buffer may be assigned to a const char * pointer, so we use the
    // const accessor here, since it will work with constant or mutable data.
    return strncmp(mu_cstring_data(s), cstring, mu_string_length(s));
  }
}

// return true if the string equals the given string
bool mu_string_eq(mu_string_t* s, const mu_string_data_t* cstring) {
  return mu_string_cmp(s, cstring) == 0;
}

// make a copy of s into dst (shallow copy - doesn't copy chars)
mu_string_t* mu_string_duplicate(mu_string_t* dst, mu_string_t* src) {
  memcpy((void*)dst, (void*)src, sizeof(mu_string_t));
  return dst;
}

// ======
// The following functons modify s directly unless a dst object is
// provided.  The underlying data buffer is not modified.

// Create a substring from the underlying string.  Negative indeces end counts
// from end, so:
//      slice("discover", 0, 5) => "disco"
//      slice("discover", 3, -1) => "cover"
//      slice("discover", -4, -1) => "over"
// Returns sliced results in dst if given, s it not.
mu_string_t* mu_string_slice(mu_string_t* s,
                             int start,
                             int end,
                             mu_string_t* dst) {
  int s1, e1;

  // Note to self: resist urge to use ternary expression: discrete `if` clauses
  // make code coverage tests more useful.
  if (start >= 0) {
    s1 = s->start + start;
  } else {
    s1 = s->end + start + 1;
  }

  if (end >= 0) {
    e1 = s->start + end;
  } else {
    e1 = s->end + end + 1;
  }

  if (e1 < s1) {
    // end cannot precede start
    e1 = s1;
  }

  return init_or_slice(s, clamp(s->start, s1, s->end),
                       clamp(s->start, e1, s->end), dst);
}

// Search for cstring within s.  Returns null if not found, else return sliced
// results in dst (if given) or s (if not).
mu_string_t* mu_string_find(mu_string_t* s,
                            const mu_string_data_t* cstring,
                            mu_string_t* dst) {
  int s1, e1;

  mu_string_data_t* found = strstr(mu_cstring_data(s), cstring);

  if (found == NULL) {
    // Didn't find string
    return NULL;
  }

  s1 = mu_string_start(s) + found - mu_cstring_data(s);
  if ((s1 < s->start) || (s1 >= s->end)) {
    // start of found string lies outside of s
    return NULL;
  }

  e1 = s1 + strlen(cstring);
  if (e1 > s->end) {
    // end of found string lies outside of s
    return NULL;
  }

  // found substring.  create slice
  return init_or_slice(s, s1, e1, dst);
}

// ======
// The following modify the string object as well as the underlying buffer. Note
// that these only apply to mu_string objects, not mu_cstring_objects (since
// mu_cstring objects cannot be modified).

// append string referred to by src onto dst
mu_string_t* mu_string_append(mu_string_t* dst, mu_string_t* src) {
  size_t n_available = mu_string_available(dst);
  int to_copy = mu_string_length(src);

  if (to_copy > n_available) {
    to_copy = n_available;
  }
  strncpy((char*)&dst->mbuf[dst->end], mu_string_data(src), to_copy);
  dst->end += to_copy;
  return dst;
}

// sprintf() into &string[start]
mu_string_t* mu_string_sprintf(mu_string_t* s, const char* fmt, ...) {
  va_list ap;
  size_t n_available = mu_string_available(s);
  va_start(ap, fmt);
  // append no more than `available` chars to the end of the buffer
  // (+1 for a null byte)
  int n_written =
      vsnprintf((char*)&(s->mbuf[s->end]), n_available + 1, fmt, ap);
  va_end(ap);
  s->end += (n_written < n_available) ? n_written : n_available;

  return s;
}

// ======
// the following functions copy bytes:

// Copy data from string[start] to string[end] (plus null termination, if
// there's room) into c_str. Copies at most c_str_length bytes.  Returns c_str.
mu_string_data_t* mu_string_extract(mu_string_t* s,
                                    mu_string_data_t* c_str,
                                    size_t c_str_length) {
  int to_copy = mu_string_length(s);

  if (to_copy > c_str_length) {
    to_copy = c_str_length;
  }
  strncpy(c_str, mu_string_data(s), to_copy);
  // null terminate if there's room
  if (to_copy < c_str_length) {
    c_str[to_copy] = '\0';
  }
  return c_str;
}

// =============================================================================
// private code

// Limit val to be lo <= val <= hi
int clamp(int lo, int val, int hi) {
  if (val < lo) {
    return lo;
  } else if (val > hi) {
    return hi;
  } else {
    return val;
  }
}

// If d is NULL, modify start and end of s and return it.  Otherwise, initialize
// d as a substring of s and return it.
mu_string_t* init_or_slice(mu_string_t* s, int start, int end, mu_string_t* d) {
  if (d) {
    d->cbuf = s->cbuf;
    d->capacity = s->capacity;
  } else {
    d = s;
  }
  d->start = start;
  d->end = end;
  return d;
}
