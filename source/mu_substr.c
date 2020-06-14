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

#include "mu_str.h"
#include "mu_substr.h"
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

// #include <stdio.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static size_t min_size(size_t a, size_t b);
static bool index_is_valid(mu_substr_t *s, size_t index);
static mu_str_data_t *ref_offset(mu_substr_t *s, size_t offset);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_substr_t *mu_substr_init(mu_substr_t *s, mu_str_t *str) {
  s->str = str;
  return mu_substr_reset(s);
}

mu_substr_t *mu_substr_duplicate(mu_substr_t *dst, mu_substr_t *src) {
  if (src != dst) {
    memcpy(dst, src, sizeof(mu_substr_t));
  }
  return dst;
}

mu_substr_t *mu_substr_reset(mu_substr_t *s) {
  s->start = 0;
  s->end = mu_str_capacity(s->str);
  return s;
}

mu_substr_t *mu_substr_clear(mu_substr_t *s) {
  s->start = 0;
  s->end = 0;
  return s;
}

mu_str_t *mu_substr_str(mu_substr_t *s) {
  return s->str;
}

size_t mu_substr_start(mu_substr_t *s) {
  return s->start;
}

size_t mu_substr_end(mu_substr_t *s) {
  return s->end;
}

size_t mu_substr_length(mu_substr_t *s) {
  return s->end - s->start;
}

size_t mu_substr_remaining(mu_substr_t *s) {
  return mu_str_capacity(s->str) - s->end;
}

mu_substr_err_t mu_substr_ref(mu_substr_t *s, size_t index, mu_str_data_t **p) {
  if (index_is_valid(s, index)) {
    *p = ref_offset(s, index);
    return MU_SUBSTR_ERR_NONE;
  } else {
    *p = NULL;
    return MU_SUBSTR_ERR_INDEX;
  }
}

mu_substr_err_t mu_substr_get(mu_substr_t *s, size_t index, mu_str_data_t *d) {
  if (index_is_valid(s, index)) {
    *d = *ref_offset(s, index);
    return MU_SUBSTR_ERR_NONE;
  } else {
    *d = '\0';
    return MU_SUBSTR_ERR_INDEX;
  }
}

mu_substr_err_t mu_substr_put(mu_substr_t *s, size_t index, mu_str_data_t d) {
  if (index_is_valid(s, index)) {
    *ref_offset(s, index) = d;
    return MU_SUBSTR_ERR_NONE;
  } else {
    return MU_SUBSTR_ERR_INDEX;
  }
}

mu_substr_t *mu_substr_copy(mu_substr_t *dst, mu_substr_t *src) {
  size_t dst_len = mu_substr_length(dst);
  size_t src_len = mu_substr_length(src);
  size_t to_copy = min_size(dst_len, src_len);
  if (to_copy > 0) {
    memmove(ref_offset(dst, 0), ref_offset(src, 0), to_copy);
  }
  dst->end = dst->start + to_copy;
  return dst;
}

int mu_substr_cmp(mu_substr_t *s1, mu_substr_t *s2) {
  size_t to_compare = min_size(mu_substr_length(s1), mu_substr_length(s2));
  mu_str_data_t *p1 = ref_offset(s1, 0);
  mu_str_data_t *p2 = ref_offset(s2, 0);
  // printf("\r\nmu_substr_cmp: '%s' vs '%s' for %ld", p1, p2, to_compare);
  while (to_compare-- > 0 && *p1 == *p2) {
    p1++;
    p2++;
  }
  if (*p1 > *p2) {
    return 1;
  } else if (*p1 < *p2) {
    return -1;
  } else {
    return 0;
  }
}

bool mu_substr_equal(mu_substr_t *s1, mu_substr_t *s2) {
  return mu_substr_cmp(s1, s2) == 0;
}

mu_substr_t *mu_substr_append(mu_substr_t *dst, mu_substr_t *src) {
  size_t to_copy = min_size(mu_substr_length(src), mu_substr_remaining(dst));
  mu_str_data_t *sp = ref_offset(src, 0);
  mu_str_data_t *dp = ref_offset(dst, 0);
  memcpy(dp, sp, to_copy);
  dst->end += to_copy;
  return dst;
}

mu_substr_t *mu_substr_trim(mu_substr_t *s, size_t from_start, size_t from_end) {
  s->start += from_start;

  // size_t is unsigned: prevent wrap-around
  if (from_end < s->end) {
    s->end -= from_end;
  } else {
    s->end = 0;
  }

  // assure that s->start <= s->end
  if (s->end < s->start) {
    s->end = s->start;
  }
  return s;
}

mu_substr_t *mu_substr_strstr(mu_substr_t *dst, mu_substr_t *needle, mu_substr_t *haystack) {
  size_t needle_len = mu_substr_length(needle);
  mu_substr_duplicate(dst, haystack);

  while(true) {
    size_t dst_len = mu_substr_length(dst);
    if (dst_len < needle_len) {
      // ran off the end of haystack: cannot match.  Return empty substr.
      return mu_substr_clear(dst);

    } else if (mu_substr_cmp(dst, needle) == 0) {
      // needle is a substring of dst.  Fix length and return match.
      // Note that to_trim is guaranteed non-negative because needle_len is
      // always less than or equal to dst_len
      size_t to_trim = dst_len - needle_len;
      return mu_substr_trim(dst, 0, to_trim);

    } else {
      // No match yet.  Compare needle to next char in haystack.
      mu_substr_trim(dst, 1, 0);
    }
  }
  return dst;
}

mu_substr_t *mu_substr_to_cstr(mu_substr_t *s, char *cstr, size_t cstr_length) {
  // always leave one byte for null terminator
  size_t to_copy = min_size(mu_substr_length(s), cstr_length-1);
  memmove(cstr, ref_offset(s, 0), to_copy);
  cstr[to_copy] = '\0';  // null terminate
  return s;
}

// =============================================================================
// local (static) code

static size_t min_size(size_t a, size_t b) {
  return (a < b) ? a : b;
}

static bool index_is_valid(mu_substr_t *s, size_t index) {
  size_t abs_i = index + s->start;
  return (s->start <= abs_i) && (abs_i < s->end);
}

static mu_str_data_t *ref_offset(mu_substr_t *s, size_t offset) {
  // Violate mu_str's data abstraction in the name of efficiency...
  return &s->str->data[s->start + offset];
}
