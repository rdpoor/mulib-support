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

#ifndef MU_STRING_H_
#define MU_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stddef.h>

// =============================================================================
// types and definitions

typedef unsigned char mu_cstring_t;

typedef struct {
  mu_cstring_t *buf;
  size_t buf_length;
  int start;
  int end;
} mu_string_t;

// =============================================================================
// declarations

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
#include <stdbool.h>

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
mu_string_t *mu_string_init(mu_string_t *s, mu_cstring_t *buf, size_t buf_length);

// return the underlying C string
mu_cstring_t *mu_string_buf(mu_string_t *s);

// return the length of the underlying C string
size_t mu_string_buf_length(mu_string_t *s);

// reset start = end = 0
mu_string_t *mu_string_reset(mu_string_t *s);

// return the start index
int mu_string_start(mu_string_t *s);

// return the start index
int mu_string_end(mu_string_t *s);

mu_cstring_t *mu_string_cstring(mu_string_t *s);

// return i1-i0
int mu_string_length(mu_string_t *s);

// return length - i1
size_t mu_string_available(mu_string_t *s);

// compare substring with a c string
int mu_string_cmp(mu_string_t *s, const mu_cstring_t *cstring);

// return true if the string equals the given string
bool mu_string_eq(mu_string_t *s, const mu_cstring_t *cstring);

// make a copy of s into dst (shallow copy - doesn't copy chars)
mu_string_t *mu_string_duplicate(mu_string_t *dst, mu_string_t *src);

// The following manipulate the string object s directly unless a dst object is
// provided.

// take a string of the underlying string.  negative end counts from end.
mu_string_t *mu_string_slice(mu_string_t *s, int start, int end, mu_string_t *dst);

// Find cstring within s.  Returns null if not found, else return sliced results
// in dst (if given) or s (if not).
mu_string_t *mu_string_find(mu_string_t *s, const mu_cstring_t *cstring, mu_string_t *dst);

// the following functions copy bytes:

// Copy data from string[i0] to string[i1] (plus null termination) into c_str.
mu_cstring_t *mu_string_extract(mu_string_t *s, mu_cstring_t *c_str, size_t c_str_length);

// append string referred to by src onto dst
mu_string_t *mu_string_append(mu_string_t *dst, mu_string_t *src);

// sprintf() into &string[i1]
mu_string_t *mu_string_sprintf(mu_string_t *s, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_STRING_H_
