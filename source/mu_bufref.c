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
#include "mu_bufref.h"
#include <string.h>
#include <stdlib.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

/**
 * @brief Clamp start and end as needed assure
 *        0 <= ref->start <= ref->end <= capacity
 */
static mu_bufref_t *validate_indeces(mu_bufref_t *ref, size_t capacity);

// =============================================================================
// local storage

// =============================================================================
// public code

// =============================================================================
// declarations

mu_buf_err_t mu_bufref_init(mu_bufref_t *ref, mu_buf_t *buf) {
  if (ref == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (buf == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  ref->buf = buf;
  mu_bufref_reset(ref);
  return MU_BUF_ERR_NONE;
}

mu_buf_err_t mu_bufref_slice_buf(mu_bufref_t *ref, mu_buf_t *buf, ssize_t start, ssize_t end) {
  if (ref == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (buf == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  ref->buf = buf;
  size_t capacity = mu_buf_capacity(buf);

  if (start >= 0) {
    ref->start = start;
  }  else {
    ref->start = capacity + start;
  }

  if (end >= 0) {
    ref->end = end;
  } else {
    ref->end = capacity + end;
  }

  validate_indeces(ref, capacity);
  return MU_BUF_ERR_NONE;
}

mu_buf_err_t mu_bufref_slice_bufref(mu_bufref_t *ref, mu_bufref_t *src, ssize_t start, ssize_t end) {
  if (ref == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (src == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  ref->buf = src->buf;  // ref shares underlying buffer
  size_t capacity = mu_bufref_count(src);  // slice cannot exceed src's limits

  if (start >= 0) {
    ref->start = src->start + start;
  }  else {
    ref->start = src->end + start;
  }

  if (end >= 0) {
    ref->end = src->start + end;
  } else {
    ref->end = src->end + end;
  }

  validate_indeces(ref, capacity);
  return MU_BUF_ERR_NONE;
}

mu_bufref_t *mu_bufref_reset(mu_bufref_t *ref) {
  mu_buf_t *buf = mu_bufref_buf(ref);

  ref->start = 0;
  if (mu_buf_is_read_only(buf)) {
    ref->end = mu_buf_capacity(buf);
  } else {
    ref->end = 0;
  }
  return ref;
}

mu_buf_t *mu_bufref_buf(mu_bufref_t *ref) {
  return ref->buf;
}

void *mu_bufref_elements(mu_bufref_t *ref) {
  return mu_buf_elements(ref->buf);
}

bool mu_bufref_is_read_only(mu_bufref_t *ref) {
  return mu_buf_is_read_only(ref->buf);
}

size_t mu_bufref_element_size(mu_bufref_t *ref) {
  return mu_buf_element_size(ref->buf);
}

size_t mu_bufref_capacity(mu_bufref_t *ref) {
  return mu_buf_capacity(ref->buf);
}

size_t mu_bufref_start(mu_bufref_t *ref) {
  return ref->start;
}

size_t mu_bufref_end(mu_bufref_t *ref) {
  return ref->end;
}

size_t mu_bufref_count(mu_bufref_t *ref) {
  return ref->end - ref->start;
}

// =============================================================================
// local (static) code

static mu_bufref_t *validate_indeces(mu_bufref_t *ref, size_t capacity) {
  if (ref->start < 0) {
    ref->start = 0;
  } else if (ref->start > capacity) {
    ref->start = capacity;
  }
  if (ref->end < ref->start) {
    ref->end = ref->start;
  } else if (ref->end  > ref->start + capacity) {
    ref->end = ref->start + capacity;
  }
  return ref;
}
