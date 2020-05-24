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

#include "mu_bufx.h"
#include <string.h>
#include <stdlib.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

/**
 * @brief return the smaller of the two sizes.
 */
static size_t min_size(size_t a, size_t b);

/**
 * @brief enforce the relation 0 <= buf->start <= buf->end <= buf->capacity
 */
static mu_bufx_t *validate_indeces(mu_bufx_t *buf);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_bufx_t *mu_robuf_init(mu_bufx_t *buf,
                        const mu_bufx_item_t const *items,
                        size_t capacity) {
  buf->ro_items = items;
  buf->capacity = capacity;
  return mu_robuf_reset(buf);
}

mu_bufx_t *mu_robuf_init_from_cstr(mu_bufx_t *buf, const char *cstr) {
  return mu_robuf_init(buf, cstr, strlen(cstr));
}

mu_bufx_t *mu_robuf_reset(mu_bufx_t *buf) {
  buf->start = 0;
  buf->end = buf->capacity;
  return buf;
}

mu_bufx_t *mu_rwbuf_init(mu_bufx_t *buf,
                       const mu_bufx_item_t const *items,
                       size_t capacity) {
  buf->rw_items = items;
  buf->capacity = capacity;
  return mu_rwbuf_reset(buf);
}

mu_bufx_t *mu_rwbuf_reset(mu_bufx_t *buf) {
  buf->start = 0;
  buf->end = 0;
  return buf;
}

const mu_bufx_item_t const *mu_robuf_items(mu_bufx_t *buf) {
  return buf->ro_items;
}

mu_bufx_item_t const *mu_rwbuf_items(mu_bufx_t *buf) {
  return buf->rw_items;
}

size_t mu_bufx_capacity(mu_bufx_t *buf) {
  return buf->capacity;
}

int mu_bufx_start(mu_bufx_t *buf) {
  return buf->start;
}

int mu_bufx_end(mu_bufx_t *buf) {
  return buf->end;
}

size_t mu_bufx_length(mu_bufx_t *buf) {
  return  buf->end - buf->start;
}

mu_bufx_t *mu_bufx_copy(mu_bufx_t *dst, mu_bufx_t *src) {
  if (dst != src) {
    memcpy(dst, src, sizeof(mu_bufx_t));
  }
  return dst;
}

int mu_bufx_cmp(mu_bufx_t *b1, mu_bufx_t *b2) {
  size_t n_items = min_size(mu_bufx_length(b1), mu_bufx_length(b2));

  const mu_bufx_item_t *p1 = &mu_robuf_items(b1)[b1->start];
  const mu_bufx_item_t *p2 = &mu_robuf_items(b2)[b2->start];

  while(n_items-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }

  // here, the shorter of the two strings has matched.  account for difference
  // in length.
  return mu_bufx_length(b1) - mu_bufx_length(b2);
}

mu_bufx_t *mu_bufx_slice(mu_bufx_t *dst, mu_bufx_t *src, int start, int end) {
  size_t src_len = mu_bufx_length(src);

  // A slice shares the same backing store and capacity as the parent...
  mu_bufx_copy(dst, src);

  // only the start and end indeces are different
  if (start >= 0) {
    dst->start = src->start + start;
  } else {
    dst->start = src->end + start + 1;
  }
  if (end >= 0) {
    dst->end = src->start + end;
  } else {
    dst->end = src->end + end + 1;
  }
  return validate_indeces(dst);
}

mu_bufx_t *mu_bufx_find(mu_bufx_t *dst, mu_bufx_t *haystack, mu_bufx_t *needle) {
  size_t needle_len = mu_bufx_length(needle);
  mu_bufx_copy(dst, haystack);

  // shorten dst so its length equals that of needle
  mu_bufx_slice(dst, dst, 0, needle_len);

  while(true) {
    if (dst->end > dst->capacity) {
      // ran off the end of haystack: cannot match.  Return empty mu_buf.
      return mu_rwbuf_reset(dst);
    }

    if (mu_bufx_cmp(dst, needle) == 0) {
      // dst equals needle.  return it.
      return dst;

    } else {
      // No match yet.  Slide right by one.  NOTE: adding 1 to dst->end can
      // exceed dst->capacity, but we check for that at the top of the loop.
      dst->start += 1;
      dst->end += 1;
    }
  }
}

// =============================================================================
// local (static) code

static size_t min_size(size_t a, size_t b) {
  return (a < b) ? a : b;
}

static mu_bufx_t *validate_indeces(mu_bufx_t *buf) {
  if (buf->start < 0) {
    buf->start = 0;
  } else if (buf->start > buf->capacity) {
    buf->start = buf->capacity;
  }
  if (buf->end < buf->start) {
    buf->end = buf->start;
  } else if (buf->end > buf->capacity) {
    buf->end = buf->end;
  }
  return buf;
}
