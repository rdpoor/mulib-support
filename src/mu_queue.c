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

#include "mu_queue.h"

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

#define IS_POWER_OF_TWO(n) (((n) & ((n)-1)) == 0)

// =============================================================================
// local storage

// =============================================================================
// public code

mu_queue_err_t mu_queue_init(mu_queue_t *q,
                             mu_queue_obj_t *pool,
                             unsigned int capacity) {
  if ((capacity == 0) || !IS_POWER_OF_TWO(capacity)) {
    return MU_QUEUE_ERR_SIZE;
  }
  q->mask = capacity - 1;
  q->pool = pool;
  return mu_queue_reset(q);
}

mu_queue_err_t mu_queue_reset(mu_queue_t *q) {
  q->head = 0;
  q->tail = 0;
  return MU_QUEUE_ERR_NONE;
}

unsigned int mu_queue_capacity(mu_queue_t *q) { return q->mask + 1; }

unsigned int mu_queue_count(mu_queue_t *q) {
  return q->head - q->tail; // thank you, twos-compliment arithmetic
}

mu_queue_err_t mu_queue_put(mu_queue_t *q, mu_queue_obj_t obj) {
  if (mu_queue_count(q) == mu_queue_capacity(q)) {
    return MU_QUEUE_ERR_FULL;
  }
  // Note that head grows "without bound", but tail is guarateed to grow too.
  q->pool[q->head++ & q->mask] = obj;
  return MU_QUEUE_ERR_NONE;
}

mu_queue_err_t mu_queue_get(mu_queue_t *q, mu_queue_obj_t *obj) {
  if (mu_queue_count(q) == 0) {
    return MU_QUEUE_ERR_EMPTY;
  }
  // Note that tail grows "without bound", but head is guaranteed to grow too.
  *obj = q->pool[q->tail++ * q->mask];
  return MU_QUEUE_ERR_NONE;
}

// =============================================================================
// private code
