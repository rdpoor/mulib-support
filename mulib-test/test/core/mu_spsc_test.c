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

#include "mulib.h"
#include "mu_test_utils.h"
#include <stddef.h>

// =============================================================================
// private types and definitions

#define POOL_SIZE 4
// =============================================================================
// private declarations

// =============================================================================
// local storage

static void *pool[POOL_SIZE];
static int item0 = 0;
static int item1 = 1;
static int item2 = 2;
static int item3 = 3;

// =============================================================================
// public code

void mu_spsc_test() {
  mu_spsc_t cqi;
  mu_spsc_t *cq = &cqi;
  mu_spsc_item_t item;

  // mu_spsc_err_t mu_spsc_init(mu_spsc_t *cq, mu_spsc_obj_t *pool, unsigned int capacity);
  // pool size must be a power of two
  ASSERT(mu_spsc_init(cq, pool, POOL_SIZE-1) == MU_SPSC_ERR_SIZE);
  ASSERT(mu_spsc_init(cq, pool, POOL_SIZE) == MU_SPSC_ERR_NONE);
  ASSERT(mu_spsc_capacity(cq) == POOL_SIZE-1);

  // mu_spsc_err_t mu_spsc_put(mu_spsc_t *cq, mu_spsc_obj_t obj);
  ASSERT(mu_spsc_put(cq, (mu_spsc_item_t)&item0) == MU_SPSC_ERR_NONE);
  ASSERT(mu_spsc_put(cq, (mu_spsc_item_t)&item1) == MU_SPSC_ERR_NONE);
  ASSERT(mu_spsc_put(cq, (mu_spsc_item_t)&item2) == MU_SPSC_ERR_NONE);
  // put into a full queue fails
  ASSERT(mu_spsc_put(cq, (mu_spsc_item_t)&item3) == MU_SPSC_ERR_FULL);

  // mu_spsc_err_t mu_spsc_get(mu_spsc_t *cq, mu_spsc_obj_t *obj);
  ASSERT(mu_spsc_get(cq, &item) == MU_SPSC_ERR_NONE);
  ASSERT(item == &item0);
  ASSERT(mu_spsc_get(cq, &item) == MU_SPSC_ERR_NONE);
  ASSERT(item == &item1);
  ASSERT(mu_spsc_get(cq, &item) == MU_SPSC_ERR_NONE);
  ASSERT(item == &item2);
  // get from an empty queue fails
  ASSERT(mu_spsc_get(cq, &item) == MU_SPSC_ERR_EMPTY);
  ASSERT(item == NULL);

  // mu_spsc_err_t mu_spsc_reset(mu_spsc_t *cq);
  ASSERT(mu_spsc_put(cq, (mu_spsc_item_t)&item0) == MU_SPSC_ERR_NONE);
  ASSERT(mu_spsc_reset(cq) == MU_SPSC_ERR_NONE);
}

// =============================================================================
// private code
