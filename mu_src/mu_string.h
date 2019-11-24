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

/**
 * @file String management.
 *
 * The mu_string module provides methods for manipulating arrays of byte-sized
 * values.  Two flavors of string are provvided:
 *   mu_string supports mutable strings
 *   mu_cstring supports constant (read-only) strings
 */
#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// types and definitions

// the underlying data type.  May be modified for uint8_t, char, etc.
typedef char mu_string_data_t;

typedef struct _mu_string {
  union {
    mu_string_data_t* mbuf;        // modifiable / mutable buffer
    const mu_string_data_t* cbuf;  // constant buffer
  };
  size_t capacity;
  int start;
  int end;
} mu_string_t;

// =============================================================================
// declarations

// initialize a mu_string (writeable).  Sets start and end to 0.
mu_string_t* mu_string_init(mu_string_t* s,
                            mu_string_data_t* buf,
                            size_t buf_length);

// initialize a mu_cstring (read-only).  Sets start to 0 and end to buf_length.
mu_string_t* mu_cstring_init(mu_string_t* s,
                              const mu_string_data_t* buf,
                              size_t buf_length);

// return the underlying buffer.  Use with caution.
mu_string_data_t* mu_string_buf(mu_string_t* s);
const mu_string_data_t* mu_cstring_buf(mu_string_t* s);

// return the length of the underlying buffer.  Use for either constant or
// mutable strings.
size_t mu_string_capacity(mu_string_t* s);

// reset start = end = 0
mu_string_t* mu_string_reset(mu_string_t* s);

// reset start = 0, end = capacity
mu_string_t* mu_cstring_reset(mu_string_t* s);

// return the start index
int mu_string_start(mu_string_t* s);

// return the start index
int mu_string_end(mu_string_t* s);

// return &buf[start]
mu_string_data_t* mu_string_data(mu_string_t* s);
const mu_string_data_t* mu_cstring_data(mu_string_t* s);

// return end-start
int mu_string_length(mu_string_t* s);

// return capacity-end.
size_t mu_string_available(mu_string_t* s);

// compare substring with a c string
int mu_string_cmp(mu_string_t* s, const mu_string_data_t* cstring);

// return true if the string equals the given string
bool mu_string_eq(mu_string_t* s, const mu_string_data_t* cstring);

// make a copy of src into dst (shallow copy - doesn't copy data buffer)
mu_string_t* mu_string_duplicate(mu_string_t* dst, mu_string_t* src);

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
                             mu_string_t* dst);

// Search for cstring within s.  Returns null if not found, else return sliced
// results in dst (if given) or s (if not).
mu_string_t* mu_string_find(mu_string_t* s,
                            const mu_string_data_t* cstring,
                            mu_string_t* dst);

// ======
// The following modify the string object as well as the underlying buffer. Note
// that these only apply to mu_string objects, not mu_cstring_objects (since
// mu_cstring objects cannot be modified).

// append string referred to by src onto dst
mu_string_t* mu_string_append(mu_string_t* dst, mu_string_t* src);

// sprintf() into &string[start]
mu_string_t* mu_string_sprintf(mu_string_t* s, const char* fmt, ...);

// ======
// the following functions copy bytes:

// Copy data from string[start] to string[end] (plus null termination if
// there's room) into c_str. Copies at most c_str_length bytes.  Returns c_str.
mu_string_data_t* mu_string_extract(mu_string_t* s,
                                    mu_string_data_t* c_str,
                                    size_t c_str_length);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef MU_STRING_H_
