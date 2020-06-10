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

#include "mu_cbuf.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

#define IS_POWER_OF_TWO(n) (((n) & ((n)-1)) == 0)

// =============================================================================
// local storage

// =============================================================================
// public code

mu_cbuf_err_t mu_cbuf_init(mu_cbuf_t *q,
                               mu_cbuf_item_t *store,
                               uint16_t capacity) {
  if ((capacity == 0) || !IS_POWER_OF_TWO(capacity)) {
    return MU_CQUEUE_ERR_SIZE;
  }
  q->mask = capacity - 1;
  q->store = store;
  return mu_cbuf_reset(q);
}

mu_cbuf_err_t mu_cbuf_reset(mu_cbuf_t *q) {
  q->head = 0;
  q->tail = 0;
  return MU_CQUEUE_ERR_NONE;
}

// Note: capacity is one less than the capacity of the backing store.  This is
// because we use head == tail to signify an empty queue.
uint16_t mu_cbuf_capacity(mu_cbuf_t *q) { return q->mask; }

uint16_t mu_cbuf_count(mu_cbuf_t *q) {
  return (q->tail - q->head) & q->mask;
}

bool mu_cbuf_is_empty(mu_cbuf_t *q) { return q->head == q->tail; }

bool mu_cbuf_is_full(mu_cbuf_t *q) {
  // would advancing tail by 1 make it catch up with head?
  return ((q->tail + 1) & q->mask) == q->head;
}

mu_cbuf_err_t mu_cbuf_put(mu_cbuf_t *q, mu_cbuf_item_t item) {
  if (mu_cbuf_is_full(q)) {
    return MU_CQUEUE_ERR_FULL;
  }
  q->store[q->tail] = item;
  q->tail = (q->tail + 1) & q->mask;
  return MU_CQUEUE_ERR_NONE;
}

mu_cbuf_err_t mu_cbuf_get(mu_cbuf_t *q, mu_cbuf_item_t *item) {
  if (mu_cbuf_is_empty(q)) {
    *item = NULL;
    return MU_CQUEUE_ERR_EMPTY;
  }
  *item = q->store[q->head];
  q->head = (q->head + 1) & q->mask;
  return MU_CQUEUE_ERR_NONE;
}

// =============================================================================
// private code
