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

 #include "mu_test_utils.h"
 #include "mu_cirq.h"
 #include <string.h>
 #include <stdint.h>
 // #include <stdio.h>

// =============================================================================
// private types and definitions

// Define a data type to be linked via mu_cirq.  We've intentionally chosen a
// structure in which the link field not the first field in order to demonstrate
// the MU_LIST_REF and MU_LIST_CONTAINER macros.
//
typedef struct {
  uint8_t a;
  uint16_t b;
  uint32_t c;
} item_t;

// =============================================================================
// private declarations

/**
 * @brief zero out the static storage
 */
static void reset_statics(void);

/**
 * @brief Return true if two item_t are equal.
 */
static bool items_are_equal(const item_t *a, const item_t *b);

/**
 * @brief print an item_t
 */
// static void print_item(const item_t *item);

// =============================================================================
// local storage

uint8_t s_a2[4];
uint16_t s_b2[4];
uint32_t s_c2[4];
item_t s_items2[4];

// =============================================================================
// public code

void mu_cirq_test() {
  uint8_t store[100];

  mu_cirq_t queue;
  uint8_t a1[] = {1, 2, 3, 4};
  // uint16_t b1[] = {1, 2, 3, 4};
  // uint32_t c1[] = {1, 2, 3, 4};
  item_t items1[4] = {{1, 11, 111}, {2, 22, 222}, {3, 33, 333}, {4, 44, 444}};

  mu_cirq_t *q = &queue;

  // pathology
  reset_statics();
  ASSERT(mu_cirq_init(q, store, 1) == q);
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == true);
  ASSERT(mu_cirq_capacity(q) == 0);

  // capacity = 1
  reset_statics();
  ASSERT(mu_cirq_init(q, store, 2) == q);
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(mu_cirq_capacity(q) == 1);

  ASSERT(mu_cirq_reset(q) == q);
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 0);   // queue was empty
  ASSERT(mu_cirq_write_8(q, a1, 4) == 1);  // queue can only hold one
  ASSERT(mu_cirq_is_empty(q) == false);
  ASSERT(mu_cirq_is_full(q) == true);
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 1);   // queue held one element
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(s_a2[0] == 1);

  // capacity = 3
  reset_statics();
  ASSERT(mu_cirq_init(q, store, 4) == q);
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(mu_cirq_capacity(q) == 3);

  ASSERT(mu_cirq_reset(q) == q);
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 0);   // queue was empty
  ASSERT(mu_cirq_write_8(q, a1, 4) == 3);  // queue can only hold three
  ASSERT(mu_cirq_is_empty(q) == false);
  ASSERT(mu_cirq_is_full(q) == true);
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 3);   // queue held one element
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(s_a2[0] == 1);
  ASSERT(s_a2[1] == 2);
  ASSERT(s_a2[2] == 3);

  // Assure the pointer wraps properly
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 0);   // queue was empty
  ASSERT(mu_cirq_write_8(q, a1, 4) == 3);  // queue can only hold three
  ASSERT(mu_cirq_is_empty(q) == false);
  ASSERT(mu_cirq_is_full(q) == true);
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 3);   // queue held one element
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(s_a2[0] == 1);
  ASSERT(s_a2[1] == 2);
  ASSERT(s_a2[2] == 3);

  // capacity = 7
  reset_statics();
  ASSERT(mu_cirq_init(q, store, 8) == q);
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(mu_cirq_capacity(q) == 7);

  ASSERT(mu_cirq_reset(q) == q);
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 0);   // queue was empty
  ASSERT(mu_cirq_write_8(q, a1, 4) == 4);  // now = [1, 2, 3, 4]
  ASSERT(mu_cirq_is_empty(q) == false);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(mu_cirq_write_8(q, a1, 4) == 3);  // now = [1, 2, 3, 4, 1, 2, 3]
  ASSERT(mu_cirq_is_empty(q) == false);
  ASSERT(mu_cirq_is_full(q) == true);
  ASSERT(mu_cirq_read_8(q, s_a2, 3) == 3);   // now = [4, 1, 2, 3];
  ASSERT(s_a2[0] == 1);
  ASSERT(s_a2[1] == 2);
  ASSERT(s_a2[2] == 3);
  ASSERT(mu_cirq_write_8(q, a1, 4) == 3);  // now = [4, 1, 2, 3, 1, 2, 3]
  ASSERT(mu_cirq_read_8(q, s_a2, 4) == 4);   // now = [1, 2, 3]
  ASSERT(s_a2[0] == 4);
  ASSERT(s_a2[1] == 1);
  ASSERT(s_a2[2] == 2);
  ASSERT(s_a2[3] == 3);

  // same, but with item_t sized objects...
  reset_statics();
  ASSERT(mu_cirq_init(q, store, 8) == q);
  ASSERT(mu_cirq_is_empty(q) == true);
  ASSERT(mu_cirq_is_full(q) == false);
  ASSERT(mu_cirq_capacity(q) == 7);

  ASSERT(mu_cirq_reset(q) == q);
  ASSERT(mu_cirq_read_n(q, s_items2, 4, sizeof(item_t)) == 0);   // queue was empty
  ASSERT(mu_cirq_write_n(q, items1, 4, sizeof(item_t)) == 4);    // now = [1, 2, 3, 4, x, x, x, x]
  ASSERT(mu_cirq_is_empty(q) == false);
  ASSERT(mu_cirq_is_full(q) == false);

  ASSERT(mu_cirq_write_n(q, items1, 4, sizeof(item_t)) == 3);    // now = [1, 2, 3, 4, 1, 2, 3, x]
  ASSERT(mu_cirq_is_empty(q) == false);
  ASSERT(mu_cirq_is_full(q) == true);
  ASSERT(mu_cirq_read_n(q, s_items2, 3, sizeof(item_t)) == 3);   // now = [x, x, x, 4, 1, 2, 3, x]
  ASSERT(items_are_equal(&s_items2[0], &items1[0]));
  ASSERT(items_are_equal(&s_items2[1], &items1[1]));
  ASSERT(items_are_equal(&s_items2[2], &items1[2]));

  reset_statics();
  ASSERT(mu_cirq_write_n(q, items1, 4, sizeof(item_t)) == 3);    // now = [2, 3, x, 4, 1, 2, 3, 1]
  ASSERT(mu_cirq_read_n(q, s_items2, 4, sizeof(item_t)) == 4);   // now = [2, 3, x, x, x, x, x, x]
  ASSERT(items_are_equal(&s_items2[0], &items1[3]));
  ASSERT(items_are_equal(&s_items2[1], &items1[0]));
  ASSERT(items_are_equal(&s_items2[2], &items1[1]));
  ASSERT(items_are_equal(&s_items2[3], &items1[2]));
}

// =============================================================================
// private code

static void reset_statics(void) {
  memset(s_a2, 0, sizeof(s_a2));
  memset(s_b2, 0, sizeof(s_b2));
  memset(s_c2, 0, sizeof(s_c2));
  memset(s_items2, 0, sizeof(s_items2));
}

static bool items_are_equal(const item_t *i1, const item_t *i2) {
  return (i1->a == i2->a) && (i1->b == i2->b) && (i1->c == i2->c);
}

// static void print_item(const item_t *item) {
//   printf("item[%p]{.a=%d, .b=%d, .c=%d}\n", item, item->a, item->b, item->c);
// }
