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

#include "../src/mu_collection.h"
#include "test_utilities.h"
#include <string.h>

#define N_ITEMS 5
#define ARRAY_SIZE 7

static void mu_vect_test();

void mu_collection_test() {
  mu_vect_test();
}

static void mu_vect_test() {
  mu_vect_item_t items[N_ITEMS];
  mu_vect_item_t item;
  mu_vect_item_t arr[ARRAY_SIZE];

  mu_vect_t c;
  // mu_vect_ref_t ref1, ref2;

  mu_vect_init(&c, items, N_ITEMS);

  // ===========================================================================
  // capacity, count, is_full, is_empty

  // c = [];

  UTEST_ASSERTEQ_SIZE(mu_vect_capacity(&c), N_ITEMS);
  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 0);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_empty(&c), true);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_full(&c), false);

  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)1), MU_COLLECTION_ERR_NONE);
  // c = [1]

  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 1);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_empty(&c), false);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_full(&c), false);

  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)1), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)1), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)1), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)1), MU_COLLECTION_ERR_NONE);
  // c = [1, 1, 1, 1, 1]

  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 5);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_empty(&c), false);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_full(&c), true);

  // ===========================================================================
  // reset

  mu_vect_reset(&c);
  UTEST_ASSERTEQ_SIZE(mu_vect_capacity(&c), N_ITEMS);
  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 0);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_empty(&c), true);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_full(&c), false);

  // ===========================================================================
  // append, push

  // c = [];

  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)1), MU_COLLECTION_ERR_NONE);
  // c = [1]
  UTEST_ASSERTEQ_INT(mu_vect_push(&c, (void *)2), MU_COLLECTION_ERR_NONE);
  // c = [2, 1]
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)3), MU_COLLECTION_ERR_NONE);
  // c = [2, 1, 3]
  UTEST_ASSERTEQ_INT(mu_vect_push(&c, (void *)4), MU_COLLECTION_ERR_NONE);
  // c = [4, 2, 1, 3]
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (void *)5), MU_COLLECTION_ERR_NONE);
  // c = [4, 2, 1, 3, 5]

  // adding to a collection that's full
  UTEST_ASSERTEQ_INT(mu_vect_push(&c, (void *)6), MU_COLLECTION_ERR_FULL);
  // c = [4, 2, 1, 3, 5]

  // ===========================================================================
  // remove, pop

  // c = [4, 2, 1, 3, 5]
  // pop, remove
  UTEST_ASSERTEQ_INT(mu_vect_pop(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (void *)5);
  UTEST_ASSERTEQ_INT(mu_vect_remove(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (void *)4);
  UTEST_ASSERTEQ_INT(mu_vect_pop(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (void *)3);
  UTEST_ASSERTEQ_INT(mu_vect_remove(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (void *)2);
  UTEST_ASSERTEQ_INT(mu_vect_pop(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (void *)1);

  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 0);

  // pop from an empty c
  UTEST_ASSERTEQ_INT(mu_vect_remove(&c, &item), MU_COLLECTION_ERR_EMPTY);
  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 0);

  // ===========================================================================
  // from_array, to_array

  arr[0] = (void *)10;
  arr[1] = (void *)11;
  arr[2] = (void *)12;
  arr[3] = (void *)13;
  arr[4] = (void *)14;
  arr[5] = (void *)15;
  arr[6] = (void *)16;

  UTEST_ASSERTEQ_SIZE(mu_vect_from_array(&c, arr, ARRAY_SIZE), N_ITEMS);
  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), N_ITEMS);

  memset(arr, 0, sizeof(arr));

  UTEST_ASSERTEQ_SIZE(mu_vect_to_array(&c, arr, ARRAY_SIZE), N_ITEMS);
  UTEST_ASSERTEQ_PTR(arr[0], (void *)10);
  UTEST_ASSERTEQ_PTR(arr[1], (void *)11);
  UTEST_ASSERTEQ_PTR(arr[2], (void *)12);
  UTEST_ASSERTEQ_PTR(arr[3], (void *)13);
  UTEST_ASSERTEQ_PTR(arr[4], (void *)14);

  // ===========================================================================
  // first, last

  UTEST_ASSERTEQ_INT(mu_vect_first(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (void *)10);
  UTEST_ASSERTEQ_INT(mu_vect_last(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (void *)14);

  mu_vect_reset(&c);
  UTEST_ASSERTEQ_INT(mu_vect_first(&c, &item), MU_COLLECTION_ERR_EMPTY);
  UTEST_ASSERTEQ_INT(mu_vect_last(&c, &item), MU_COLLECTION_ERR_EMPTY);

// =============================================================================
// References

}
