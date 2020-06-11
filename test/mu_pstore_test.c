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

#include "mu_pstore.h"
#include "mu_test_utils.h"
#include <stddef.h>

// =============================================================================
// private types and definitions

#define PSTORE_CAPACITY 4

typedef struct {
  char id;
} item_t;

// =============================================================================
// private declarations

static int sort_up(void *a, void *b);
static int sort_dn(void *a, void *b);

// =============================================================================
// local storage

static mu_pstore_item_t s_items[PSTORE_CAPACITY];
static item_t s_item1 = {.id = 'a'};
static item_t s_item2 = {.id = 'b'};
static item_t s_item3 = {.id = 'c'};
static item_t s_item4 = {.id = 'd'};
static item_t s_item5 = {.id = 'e'};

// =============================================================================
// public code

void mu_pstore_test() {
  mu_pstore_t pi;
  mu_pstore_t *p = &pi;
  mu_pstore_item_t item;

  // mu_pstore_t *mu_pstore_init(mu_pstore_t *pstore, mu_pstore_item_t *items,
  // size_t capacity);
  ASSERT(mu_pstore_init(p, s_items, PSTORE_CAPACITY) == p);
  ASSERT(mu_pstore_items(p) == s_items);
  ASSERT(mu_pstore_capacity(p) == PSTORE_CAPACITY);

  // mu_pstore_err_t mu_pstore_push(mu_pstore_t *pstore, mu_pstore_item_t item);
  ASSERT(mu_pstore_count(p) == 0);
  ASSERT(mu_pstore_peek(p, &item) == MU_PSTORE_ERR_EMPTY);
  ASSERT(item == NULL);

  ASSERT(mu_pstore_push(p, &s_item1) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 1);
  ASSERT(mu_pstore_peek(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item1);

  ASSERT(mu_pstore_push(p, &s_item2) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 2);
  ASSERT(mu_pstore_peek(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item2);

  ASSERT(mu_pstore_push(p, &s_item3) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 3);
  ASSERT(mu_pstore_peek(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item3);

  ASSERT(mu_pstore_push(p, &s_item4) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 4);
  ASSERT(mu_pstore_peek(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item4);

  ASSERT(mu_pstore_push(p, &s_item5) == MU_PSTORE_ERR_FULL);
  ASSERT(mu_pstore_count(p) == 4);
  ASSERT(mu_pstore_peek(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item4);

  // mu_pstore_err_t mu_pstore_pop(mu_pstore_t *pstore, mu_pstore_item_t *item);
  ASSERT(mu_pstore_pop(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item4);
  ASSERT(mu_pstore_count(p) == 3);

  ASSERT(mu_pstore_pop(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item3);
  ASSERT(mu_pstore_count(p) == 2);

  ASSERT(mu_pstore_pop(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item2);
  ASSERT(mu_pstore_count(p) == 1);

  ASSERT(mu_pstore_pop(p, &item) == MU_PSTORE_ERR_NONE);
  ASSERT(item == &s_item1);
  ASSERT(mu_pstore_count(p) == 0);

  ASSERT(mu_pstore_pop(p, &item) == MU_PSTORE_ERR_EMPTY);
  ASSERT(item == NULL);
  ASSERT(mu_pstore_count(p) == 0);

  // mu_ptsore_t *mu_pstore_reset(mu_pstore_t *pstore);
  mu_pstore_push(p, &s_item1);
  ASSERT(mu_pstore_reset(p) == p);
  ASSERT(mu_pstore_count(p) == 0);

  // mu_pstore_err_t mu_pstore_insert(mu_pstore_t *pstore, mu_pstore_item_t
  // item, size_t index); insert into empty store
  ASSERT(mu_pstore_insert(p, &s_item1, 0) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 1);
  // insert at top
  ASSERT(mu_pstore_insert(p, &s_item2, 0) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 2);
  ASSERT(mu_pstore_items(p)[0] == &s_item2);
  ASSERT(mu_pstore_items(p)[1] == &s_item1);
  // insert not at top
  ASSERT(mu_pstore_insert(p, &s_item3, 1) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 3);
  ASSERT(mu_pstore_items(p)[0] == &s_item2);
  ASSERT(mu_pstore_items(p)[1] == &s_item3);
  ASSERT(mu_pstore_items(p)[2] == &s_item1);
  // insert at bottom
  ASSERT(mu_pstore_insert(p, &s_item4, 3) == MU_PSTORE_ERR_NONE);
  ASSERT(mu_pstore_count(p) == 4);
  ASSERT(mu_pstore_items(p)[0] == &s_item2);
  ASSERT(mu_pstore_items(p)[1] == &s_item3);
  ASSERT(mu_pstore_items(p)[2] == &s_item1);
  ASSERT(mu_pstore_items(p)[3] == &s_item4);
  // insert when full
  ASSERT(mu_pstore_insert(p, &s_item5, 0) == MU_PSTORE_ERR_FULL);
  ASSERT(mu_pstore_count(p) == 4);

  mu_pstore_reset(p);
  // insert with illegal index
  ASSERT(mu_pstore_insert(p, &s_item1, 1) == MU_PSTORE_ERR_INDEX);
  ASSERT(mu_pstore_count(p) == 0);

  // mu_pstore_err_t mu_pstore_delete(mu_pstore_t *pstore, mu_pstore_item_t
  // *item, size_t index);
  mu_pstore_reset(p); // []
  // remove empty
  ASSERT(mu_pstore_delete(p, &item, 3) == MU_PSTORE_ERR_EMPTY); // []
  ASSERT(item == NULL);
  // load up some items
  mu_pstore_push(p, &s_item1); // [item1]
  mu_pstore_push(p, &s_item2); // [item1 item2]
  mu_pstore_push(p, &s_item3); // [item1 item2 item3]
  mu_pstore_push(p, &s_item4); // [item1 item2 item3 item4]
  // remove at end
  ASSERT(mu_pstore_delete(p, &item, 3) ==
         MU_PSTORE_ERR_NONE); // [item1 item2 item3]
  ASSERT(item == &s_item4);
  // remove at middle
  ASSERT(mu_pstore_delete(p, &item, 1) ==
         MU_PSTORE_ERR_NONE); // [item1 item3]
  ASSERT(item == &s_item2);
  // remove at head
  ASSERT(mu_pstore_delete(p, &item, 0) == MU_PSTORE_ERR_NONE); // [item1]
  ASSERT(item == &s_item1);
  // remove at with illegal index
  ASSERT(mu_pstore_delete(p, &item, 1) == MU_PSTORE_ERR_INDEX); // [item1]
  ASSERT(item == NULL);

  // mu_pstore_err_t mu_pstore_insert_sorted(mu_pstore_t *pstore,
  // mu_pstore_item_t item, mu_compare_fn cmp);
  mu_pstore_reset(p);
  // insert first
  ASSERT(mu_pstore_insert_sorted(p, &s_item2, sort_up) ==
         MU_PSTORE_ERR_NONE); // [b]
  ASSERT(mu_pstore_items(p)[0] == &s_item2);
  ASSERT(mu_pstore_insert_sorted(p, &s_item4, sort_up) ==
         MU_PSTORE_ERR_NONE); // [b d]
  ASSERT(mu_pstore_items(p)[0] == &s_item2);
  ASSERT(mu_pstore_items(p)[1] == &s_item4);
  ASSERT(mu_pstore_insert_sorted(p, &s_item3, sort_up) ==
         MU_PSTORE_ERR_NONE); // [b c d]
  ASSERT(mu_pstore_items(p)[0] == &s_item2);
  ASSERT(mu_pstore_items(p)[1] == &s_item3);
  ASSERT(mu_pstore_items(p)[2] == &s_item4);
  ASSERT(mu_pstore_insert_sorted(p, &s_item1, sort_up) ==
         MU_PSTORE_ERR_NONE); // [a b c d]
  ASSERT(mu_pstore_items(p)[0] == &s_item1);
  ASSERT(mu_pstore_items(p)[1] == &s_item2);
  ASSERT(mu_pstore_items(p)[2] == &s_item3);
  ASSERT(mu_pstore_items(p)[3] == &s_item4);
  ASSERT(mu_pstore_insert_sorted(p, &s_item5, sort_up) ==
         MU_PSTORE_ERR_FULL); // [a b c d]

  // mu_pstore_err_t mu_pstore_sort(mu_pstore_t *pstore, mu_compare_fn cmp);
  ASSERT(mu_pstore_sort(p, sort_dn) == MU_PSTORE_ERR_NONE); // [d c b a]
  ASSERT(mu_pstore_items(p)[0] == &s_item4);
  ASSERT(mu_pstore_items(p)[1] == &s_item3);
  ASSERT(mu_pstore_items(p)[2] == &s_item2);
  ASSERT(mu_pstore_items(p)[3] == &s_item1);
}

// =============================================================================
// private code

static int sort_up(void *a, void *b) {
  item_t *ai = (item_t *)a;
  item_t *bi = (item_t *)b;
  return ai->id - bi->id;
}

static int sort_dn(void *a, void *b) {
  item_t *ai = (item_t *)a;
  item_t *bi = (item_t *)b;
  return bi->id - ai->id;
}
