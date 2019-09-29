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

#include "../src/mu_heapsort.h"
#include "test_utilities.h"
#include <string.h>

// A structure with key/value pairs used for testing mu_heapsort.
typedef struct {
  char *key;
  int val;
} test_t;

// User-provided swap() function
static void swap(void *items, int i1, int i2) {
  test_t *kvs = (test_t *)items;
  test_t tmp;

  memcpy(&tmp, &kvs[i1], sizeof(test_t));
  memcpy(&kvs[i1], &kvs[i2], sizeof(test_t));
  memcpy(&kvs[i2], &tmp, sizeof(test_t));
}

// User provided functino to sort test_t structures by value
int sort_by_val_ascending(void *items, int i1, int i2) {
  test_t *kvs = (test_t *)items;

  return kvs[i1].val - kvs[i2].val;
}

// User provided functin to sort test_t structures by value, descending
int sort_by_val_descending(void *items, int i1, int i2) {
  return sort_by_val_ascending(items, i2, i1);
}

// User provided functino to sort test_t structures by key
int sort_by_key(void *items, int i1, int i2) {
  test_t *kvs = (test_t *)items;

  return strcmp(kvs[i1].key, kvs[i2].key);
}

#define N_ITEMS 5

void mu_heapsort_test() {
  test_t items[N_ITEMS] = {
    {.key="one", .val = 1},
    {.key="four", .val = 4},
    {.key="three", .val = 3},
    {.key="two", .val = 2},
    {.key="five", .val = 5},
  };

  // sort (numerically) by ascending value
  mu_heapsort(sort_by_val_ascending, swap, items, N_ITEMS);
  UTEST_ASSERT(items[0].val == 1);
  UTEST_ASSERT(items[1].val == 2);
  UTEST_ASSERT(items[2].val == 3);
  UTEST_ASSERT(items[3].val == 4);
  UTEST_ASSERT(items[4].val == 5);

  // sort (numerically) by descending value
  mu_heapsort(sort_by_val_descending, swap, items, N_ITEMS);
  UTEST_ASSERT(items[0].val == 5);
  UTEST_ASSERT(items[1].val == 4);
  UTEST_ASSERT(items[2].val == 3);
  UTEST_ASSERT(items[3].val == 2);
  UTEST_ASSERT(items[4].val == 1);

  // sort (alphbetically) by key
  mu_heapsort(sort_by_key, swap, items, N_ITEMS);
  UTEST_ASSERT(strcmp(items[0].key, "five") == 0);
  UTEST_ASSERT(strcmp(items[1].key, "four") == 0);
  UTEST_ASSERT(strcmp(items[2].key, "one") == 0);
  UTEST_ASSERT(strcmp(items[3].key, "three") == 0);
  UTEST_ASSERT(strcmp(items[4].key, "two") == 0);
}
