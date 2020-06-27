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

#include "mu_buf.h"
#include "mu_types.h"
#include <stdlib.h>
#include <string.h>

// #include <stdio.h>  // debugging

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

static void heapify(mu_buf_t *b, mu_compare_fn cmp);

static void sift_down(char *items, size_t item_size, mu_compare_fn cmp, int start, int end);

static void swap(char *items, size_t item_size, int a, int b);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_buf_err_t mu_buf_init(mu_buf_t *b,
                         void *elements,
                         bool is_readonly,
                         size_t element_size,
                         size_t capacity) {
  if (b == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (elements == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (element_size >= MU_BUF_MAX_ELEMENT_SIZE) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (capacity >= MU_BUF_MAX_CAPACITY) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  b->elements = elements;
  b->is_readonly = is_readonly;
  b->element_size = element_size;
  b->capacity = capacity;

  return MU_BUF_ERR_NONE;
}

void *mu_buf_elements(mu_buf_t *b) { return b->elements; }

bool mu_buf_is_read_only(mu_buf_t *b) { return b->is_readonly; }

size_t mu_buf_element_size(mu_buf_t *b) { return b->element_size; }

size_t mu_buf_capacity(mu_buf_t *b) { return b->capacity; }

mu_buf_err_t mu_buf_ref(mu_buf_t *b, size_t index, void **p) {
  if (index >= mu_buf_capacity(b)) {
    *p = NULL;
    return MU_BUF_ERR_INDEX_BOUNDS;
  }
  size_t item_size = mu_buf_element_size(b);
  char *items = (char *)mu_buf_elements(b);
  *p = &items[index * item_size];
  return MU_BUF_ERR_NONE;
}

mu_buf_err_t mu_buf_get(mu_buf_t *b, size_t index, void *dst) {
  void *p;
  mu_buf_err_t ret = mu_buf_ref(b, index, &p);
  if (ret == MU_BUF_ERR_NONE) {
    memcpy(dst, p, mu_buf_element_size(b));
  }
  return ret;
}

mu_buf_err_t mu_buf_put(mu_buf_t *b, size_t index, void *src) {
  void *p;
  if (mu_buf_is_read_only(b)) {
    return MU_BUF_ERR_READ_ONLY;
  }
  mu_buf_err_t ret = mu_buf_ref(b, index, &p);
  if (ret == MU_BUF_ERR_NONE) {
    memcpy(p, src, mu_buf_element_size(b));
  }
  return ret;
}

mu_buf_err_t mu_buf_sort(mu_buf_t *b, mu_compare_fn cmp) {
  if (cmp == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  if (mu_buf_is_read_only(b)) {
    return MU_BUF_ERR_READ_ONLY;
  }

  size_t item_size = mu_buf_element_size(b);
  char *items = (char *)mu_buf_elements(b);

  heapify(b, cmp);

  size_t end = mu_buf_capacity(b) - 1;
  while (end > 0) {
    swap(items, item_size, end, 0);
    end -= 1;
    sift_down(items, item_size, cmp, 0, end);
  }
  return MU_BUF_ERR_NONE;
}

mu_buf_err_t mu_buf_from_cstr(mu_buf_t *b, const char *cstr) {
  if (cstr == NULL) {
    return MU_BUF_ERR_ILLEGAL_ARG;
  }
  return mu_buf_init(b, (void *)cstr, true, sizeof(char), strlen(cstr));
}

// =============================================================================
// local (static) code

static void heapify(mu_buf_t *b, mu_compare_fn cmp) {
  char *items = (char *)mu_buf_elements(b);
  size_t item_size = mu_buf_element_size(b);
  int count = mu_buf_capacity(b);
  int start = (count - 2) / 2; // index of last parent node

  while (start >= 0) {
    sift_down(items, item_size, cmp, start, count - 1);
    start -= 1;
  }
}

static void sift_down(char *items, size_t item_size, mu_compare_fn cmp, int start, int end) {
  int root = start;
  while (root * 2 + 1 <= end) {
    // root has at least one child...
    int child = root * 2 + 1;      // left child
    if ((child + 1 <= end) &&
        cmp(&items[child * item_size], &items[(child + 1) * item_size]) < 0) {
      // child has a sibling and its value is less than the sibling's...
      child += 1; // then act on right child instead
    }
    if (cmp(&items[root * item_size], &items[child * item_size]) < 0) {
      // not in heap order...
      swap(items, item_size, root, child);
      root = child; // continue sifting down the child
    } else {
      return;
    }
  }
}

static void swap(char *items, size_t item_size, int a, int b) {
  char temp;
  char *pa = &items[a * item_size];
  char *pb = &items[b * item_size];

  for (int i=0; i<item_size; i++) {
    temp = pa[i];
    pa[i] = pb[i];
    pb[i] = temp;
  }
}
