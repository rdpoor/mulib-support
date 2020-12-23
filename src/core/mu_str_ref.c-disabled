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

#include "mu_str_ref.h"
#include "mu_str_rbuf.h"
#include "mu_str_wbuf.h"
#include "mulib.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

/**
 * @brief return a pointer to the next byte for reading.
 */
static const uint8_t *const read_ptr(const mu_str_ref_t *const src);

/**
 * @brief return a pointer to the next byte for writing.
 */
static uint8_t *write_ptr(const mu_str_ref_t *const dst);

/**
 * @brief Copy up to count bytes from src to the end of dst.  Returns number of
 * bytes actually copied.
 */
static uint8_t append(mu_str_ref_t *dst, const uint8_t *const src, int count);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_str_ref_t *mu_str_ref_init_for_read(mu_str_ref_t *ref, mu_str_rbuf_t *rbuf) {
  ref->rbuf = rbuf;
  return mu_str_ref_read_reset(ref);
}

mu_str_ref_t *mu_str_ref_init_for_write(mu_str_ref_t *ref,
                                        mu_str_wbuf_t *wbuf) {
  ref->wbuf = wbuf;
  return mu_str_ref_write_reset(ref);
}

mu_str_ref_t *mu_str_ref_read_reset(mu_str_ref_t *ref) {
  ref->get_i = 0;
  ref->put_i = mu_str_rbuf_capacity(ref->rbuf);
  return ref;
}

mu_str_ref_t *mu_str_ref_write_reset(mu_str_ref_t *ref) {
  ref->get_i = 0;
  ref->put_i = 0;
  return ref;
}

mu_str_ref_t *mu_str_ref_copy(mu_str_ref_t *dst,
                              const mu_str_ref_t *const src) {
  return (mu_str_ref_t *)memcpy(dst, src, sizeof(mu_str_ref_t));
}

mu_str_ref_t *mu_str_ref_slice(mu_str_ref_t *dst,
                               const mu_str_ref_t *const src,
                               int start,
                               int end) {
  size_t len = mu_str_ref_readable_count(src);
  size_t s1, e1;

  mu_str_ref_copy(dst, src);

  if (start >= 0) {
    s1 = start;
  } else {
    s1 = len + start;
  }

  if (end >= 0) {
    e1 = end;
  } else {
    e1 = len + end;
  }

  // clamp.
  if (s1 > len) {
    s1 = len;
  }
  if (e1 < 0) {
    e1 = 0;
  }
  if (s1 > e1) {
    s1 = e1;
  }

  // s1, e1 refer the start and end of the slice, relative to get_i:
  dst->put_i = dst->get_i + e1;
  dst->get_i += s1;

  return dst;
}

size_t mu_str_ref_readable_count(const mu_str_ref_t *const ref) {
  return ref->put_i - ref->get_i;
}

size_t mu_str_ref_writeable_count(const mu_str_ref_t *const ref) {
  return mu_str_wbuf_capacity(ref->wbuf) - ref->put_i;
}

bool mu_str_ref_read_byte(mu_str_ref_t *ref, uint8_t *byte) {
  if (mu_str_ref_readable_count(ref) == 0) {
    return false;
  }
  *byte = *read_ptr(ref);
  ref->get_i += 1;
  return true;
}

bool mu_str_ref_write_byte(mu_str_ref_t *ref, uint8_t byte) {
  if (mu_str_ref_writeable_count(ref) == 0) {
    return false;
  }
  *write_ptr(ref) = byte;
  ref->put_i += 1;
  return true;
}

size_t mu_str_ref_append(mu_str_ref_t *dst, const mu_str_ref_t *const src) {
  return append(dst, read_ptr(src), mu_str_ref_readable_count(src));
}

size_t mu_str_ref_append_cstr(mu_str_ref_t *dst, const char * const cstr) {
  return append(dst, (const uint8_t *const)cstr, strlen(cstr));
}

size_t mu_str_ref_printf(mu_str_ref_t *dst, const char *fmt, ...) {
  size_t writeable = mu_str_ref_writeable_count(dst);
  size_t written = 0;

  if (writeable > 0) {
    va_list ap;
    va_start(ap, fmt);
    written = vsnprintf((char *)write_ptr(dst), writeable, fmt, ap);
    va_end(ap);
    // Note: written is the number of bytes that *would* have been written if
    // there was enough room, NOT the number of bytes actually written.
    if (written > writeable) {
      written = writeable;
    }
    dst->put_i += written;
  }
  return written;
}

size_t
mu_str_ref_to_cstr(const mu_str_ref_t *const src, uint8_t *buf, size_t buflen) {
  size_t copied = mu_str_ref_readable_count(src);
  if (copied > buflen - 1) {
    copied = buflen - 1; // always leave room for null termination
  }
  memcpy(buf, read_ptr(src), copied);
  buf[copied] = '\0'; // null terminate

  return copied - 1; // return # of bytes copied, not including null
}

// =============================================================================
// local (static) code

static const uint8_t *const read_ptr(const mu_str_ref_t *const src) {
  return &src->rbuf->store[src->get_i];
}

static uint8_t *write_ptr(const mu_str_ref_t *const dst) {
  return &dst->wbuf->store[dst->put_i];
}

static uint8_t append(mu_str_ref_t *dst, const uint8_t *const src, int count) {
  size_t writeable = mu_str_ref_writeable_count(dst);

  if (count > writeable) {
    count = writeable;
  }
  memcpy(write_ptr(dst), src, count);
  dst->put_i += count;
  return count;
}
