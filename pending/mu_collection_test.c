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
#include <stdio.h>

#define N_ITEMS 5
#define ARRAY_SIZE 7

static void mu_vect_test();

void mu_collection_test() {
  mu_vect_test();
}

/**
 * Helper function to initialize an array with items.
 */
static void fill_vect(mu_vect_t *c) {
  void *arr[ARRAY_SIZE];

  arr[0] = (mu_vect_item_t)10;
  arr[1] = (mu_vect_item_t)11;
  arr[2] = (mu_vect_item_t)12;
  arr[3] = (mu_vect_item_t)13;
  arr[4] = (mu_vect_item_t)14;
  mu_vect_from_array(c, arr, ARRAY_SIZE);
}

typedef struct {
  int count;
  void *first_found;
} test_struct_t;

// specialized traversal function that stops aftr finding the first odd item
static bool first_odd_fn(mu_vref_t *ref, void *self) {
  void *item;

  ((test_struct_t *)self)->count += 1;
  mu_vect_deref(ref, &item);
  if ((uint32_t)item & 1) {
    // is odd
    ((test_struct_t *)self)->first_found = item;
    return false;
  } else {
    return true;
  }
}

static void mu_vect_test() {
  void *items[N_ITEMS];
  void *item;
  void *arr[ARRAY_SIZE];

  mu_vect_t c;
  mu_vref_t ref;

  mu_vect_init(&c, items, N_ITEMS);

  // ===========================================================================
  // capacity, count, is_full, is_empty

  // c = [];

  UTEST_ASSERTEQ_SIZE(mu_vect_capacity(&c), N_ITEMS);
  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 0);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_empty(&c), true);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_full(&c), false);

  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)1), MU_COLLECTION_ERR_NONE);
  // c = [1]

  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 1);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_empty(&c), false);
  UTEST_ASSERTEQ_BOOL(mu_vect_is_full(&c), false);

  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)1), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)1), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)1), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)1), MU_COLLECTION_ERR_NONE);
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

  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)1), MU_COLLECTION_ERR_NONE);
  // c = [1]
  UTEST_ASSERTEQ_INT(mu_vect_push(&c, (mu_vect_item_t)2), MU_COLLECTION_ERR_NONE);
  // c = [2, 1]
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)3), MU_COLLECTION_ERR_NONE);
  // c = [2, 1, 3]
  UTEST_ASSERTEQ_INT(mu_vect_push(&c, (mu_vect_item_t)4), MU_COLLECTION_ERR_NONE);
  // c = [4, 2, 1, 3]
  UTEST_ASSERTEQ_INT(mu_vect_append(&c, (mu_vect_item_t)5), MU_COLLECTION_ERR_NONE);
  // c = [4, 2, 1, 3, 5]

  // adding to a collection that's full
  UTEST_ASSERTEQ_INT(mu_vect_push(&c, (mu_vect_item_t)6), MU_COLLECTION_ERR_FULL);
  // c = [4, 2, 1, 3, 5]

  // ===========================================================================
  // remove, pop

  // c = [4, 2, 1, 3, 5]
  // pop, remove
  UTEST_ASSERTEQ_INT(mu_vect_pop(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)4);
  UTEST_ASSERTEQ_INT(mu_vect_remove(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)5);
  UTEST_ASSERTEQ_INT(mu_vect_pop(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)2);
  UTEST_ASSERTEQ_INT(mu_vect_remove(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)3);
  UTEST_ASSERTEQ_INT(mu_vect_pop(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)1);

  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 0);

  // pop from an empty c
  UTEST_ASSERTEQ_INT(mu_vect_remove(&c, &item), MU_COLLECTION_ERR_EMPTY);
  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), 0);

  // ===========================================================================
  // from_array, to_array

  arr[0] = (mu_vect_item_t)10;
  arr[1] = (mu_vect_item_t)11;
  arr[2] = (mu_vect_item_t)12;
  arr[3] = (mu_vect_item_t)13;
  arr[4] = (mu_vect_item_t)14;
  arr[5] = (mu_vect_item_t)15;
  arr[6] = (mu_vect_item_t)16;

  UTEST_ASSERTEQ_SIZE(mu_vect_from_array(&c, arr, ARRAY_SIZE), N_ITEMS);
  UTEST_ASSERTEQ_SIZE(mu_vect_count(&c), N_ITEMS);

  memset(arr, 0, sizeof(arr));

  UTEST_ASSERTEQ_SIZE(mu_vect_to_array(&c, arr, ARRAY_SIZE), N_ITEMS);
  UTEST_ASSERTEQ_PTR(arr[0], (mu_vect_item_t)10);
  UTEST_ASSERTEQ_PTR(arr[1], (mu_vect_item_t)11);
  UTEST_ASSERTEQ_PTR(arr[2], (mu_vect_item_t)12);
  UTEST_ASSERTEQ_PTR(arr[3], (mu_vect_item_t)13);
  UTEST_ASSERTEQ_PTR(arr[4], (mu_vect_item_t)14);

  // ===========================================================================
  // first, last

  UTEST_ASSERTEQ_INT(mu_vect_first(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)10);
  UTEST_ASSERTEQ_INT(mu_vect_last(&c, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)14);

  mu_vect_reset(&c);
  UTEST_ASSERTEQ_INT(mu_vect_first(&c, &item), MU_COLLECTION_ERR_EMPTY);
  UTEST_ASSERTEQ_INT(mu_vect_last(&c, &item), MU_COLLECTION_ERR_EMPTY);

  // ===========================================================================
  // References

  // ref_first, ref_last, deref
  mu_vect_reset(&c);
  // vect = []
  UTEST_ASSERTEQ_INT(mu_vref_first(&c, &ref), MU_COLLECTION_ERR_BOUNDS);
  UTEST_ASSERTEQ_INT(mu_vref_last(&c, &ref), MU_COLLECTION_ERR_BOUNDS);

  fill_vect(&c);
  // vect = [10, 11, 12, 13, 14]
  UTEST_ASSERTEQ_INT(mu_vref_first(&c, &ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)10);

  UTEST_ASSERTEQ_INT(mu_vref_last(&c, &ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)14);

  // ref_next, ref_prev
  UTEST_ASSERTEQ_INT(mu_vref_first(&c, &ref), MU_COLLECTION_ERR_NONE);

  UTEST_ASSERTEQ_INT(mu_vref_next(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)11);

  UTEST_ASSERTEQ_INT(mu_vref_next(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)12);

  UTEST_ASSERTEQ_INT(mu_vref_next(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)13);

  UTEST_ASSERTEQ_INT(mu_vref_next(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)14);

  UTEST_ASSERTEQ_INT(mu_vref_next(&ref), MU_COLLECTION_ERR_BOUNDS);

  UTEST_ASSERTEQ_INT(mu_vref_last(&c, &ref), MU_COLLECTION_ERR_NONE);

  UTEST_ASSERTEQ_INT(mu_vref_prev(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)13);

  UTEST_ASSERTEQ_INT(mu_vref_prev(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)12);

  UTEST_ASSERTEQ_INT(mu_vref_prev(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)11);

  UTEST_ASSERTEQ_INT(mu_vref_prev(&ref), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)10);

  UTEST_ASSERTEQ_INT(mu_vref_prev(&ref), MU_COLLECTION_ERR_BOUNDS);

  // ===========================================================================
  // insert_before, insert_after

  mu_vect_reset(&c);
  mu_vect_push(&c, (mu_vect_item_t)25);
  // vect = [25]
  mu_vref_first(&c, &ref);
  // vect = [25]
  //     ref ^
  UTEST_ASSERTEQ_INT(mu_vref_push(&ref, (mu_vect_item_t)23), MU_COLLECTION_ERR_NONE);
  // vect = [23, 25]
  //         ref ^
  UTEST_ASSERTEQ_INT(mu_vref_append(&ref, (mu_vect_item_t)27), MU_COLLECTION_ERR_NONE);
  // vect = [23, 25, 27]
  //         ref ^
  UTEST_ASSERTEQ_INT(mu_vref_push(&ref, (mu_vect_item_t)24), MU_COLLECTION_ERR_NONE);
  // vect = [23, 24, 25, 27]
  //             ref ^
  UTEST_ASSERTEQ_INT(mu_vref_append(&ref, (mu_vect_item_t)26), MU_COLLECTION_ERR_NONE);
  // vect = [23, 24, 25, 26, 27]
  //             ref ^
  UTEST_ASSERTEQ_INT(mu_vref_append(&ref, (mu_vect_item_t)29), MU_COLLECTION_ERR_FULL);
  // vect = [23, 24, 25, 26, 27]
  //             ref ^
  // check results
  UTEST_ASSERTEQ_SIZE(mu_vect_to_array(&c, arr, ARRAY_SIZE), N_ITEMS);
  UTEST_ASSERTEQ_PTR(arr[0], (mu_vect_item_t)23);
  UTEST_ASSERTEQ_PTR(arr[1], (mu_vect_item_t)24);
  UTEST_ASSERTEQ_PTR(arr[2], (mu_vect_item_t)25);
  UTEST_ASSERTEQ_PTR(arr[3], (mu_vect_item_t)26);
  UTEST_ASSERTEQ_PTR(arr[4], (mu_vect_item_t)27);

  // ===========================================================================
  // ref_pop, ref_remove

  // vect = [23, 24, 25, 26, 27]
  //             ref ^
  UTEST_ASSERTEQ_INT(mu_vref_pop(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)25);
  // vect = [23, 24, 26, 27]
  //             ref ^
  UTEST_ASSERTEQ_INT(mu_vref_remove(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)26);
  // vect = [23, 24, 27]
  //         ref ^
  UTEST_ASSERTEQ_INT(mu_vref_pop(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)24);
  // vect = [23, 27]
  //         ref ^
  UTEST_ASSERTEQ_INT(mu_vref_remove(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)27);
  // vect = [23]
  //     ref ^
  UTEST_ASSERTEQ_INT(mu_vref_pop(&ref, &item), MU_COLLECTION_ERR_NONE);
  UTEST_ASSERTEQ_PTR(item, (mu_vect_item_t)23);
  // vect = []
  //    ref ^
  UTEST_ASSERTEQ_INT(mu_vect_deref(&ref, &item), MU_COLLECTION_ERR_BOUNDS);

  // ===========================================================================
  // traverse

  fill_vect(&c);
  // vect = [10, 11, 12, 13, 14]
  test_struct_t target;
  target.count = 0;
  target.first_found = NULL;

  mu_vect_traverse(&c, first_odd_fn, &target);
  UTEST_ASSERTEQ_PTR(target.first_found, (mu_vect_item_t)11);
  UTEST_ASSERTEQ_INT(target.count, 2);
}
