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
 #include "core/mu_queue.h"
 #include <string.h>
 #include <stdlib.h>
 #include <stdint.h>

// =============================================================================
// private types and definitions

// Define a data type to be linked via mu_queue.  We've intentionally chosen a
// structure in which the link field not the first field in order to demonstrate
// the MU_LIST_REF and MU_LIST_CONTAINER macros.
//
typedef struct {
  float value;
  mu_list_t link;
  char id;
} queue_item_t;

// =============================================================================
// private declarations

static void reset(void);
static queue_item_t *queue_item_init(queue_item_t *item, float value, char id);

// =============================================================================
// local storage

static mu_queue_t s_queue1;
static queue_item_t s_item1;
static queue_item_t s_item2;
static queue_item_t s_item3;

// =============================================================================
// public code

void mu_queue_test() {

  mu_queue_t *q = &s_queue1;
  reset();

  // operations on an empty queue
  ASSERT(mu_queue_init(q) == q);
  ASSERT(mu_queue_is_empty(q) == true);
  ASSERT(mu_queue_length(q) == 0);
  ASSERT(mu_queue_remove(q) == NULL);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item1, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item2, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item3, link)) == false);

  // operations on a queue with one item
  ASSERT(mu_queue_add(q, MU_LIST_REF(&s_item1, link)) == q);
  ASSERT(mu_queue_is_empty(q) == false);
  ASSERT(mu_queue_length(q) == 1);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item1, link)) == true);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item2, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item3, link)) == false);

  // empty the queue and check
  ASSERT(mu_queue_remove(q) == MU_LIST_REF(&s_item1, link));
  ASSERT(mu_queue_is_empty(q) == true);
  ASSERT(mu_queue_length(q) == 0);
  ASSERT(mu_queue_remove(q) == NULL);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item1, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item2, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item3, link)) == false);

  // operations on a queue with two items
  ASSERT(mu_queue_add(q, MU_LIST_REF(&s_item1, link)) == q);
  ASSERT(mu_queue_add(q, MU_LIST_REF(&s_item2, link)) == q);

  ASSERT(mu_queue_is_empty(q) == false);
  ASSERT(mu_queue_length(q) == 2);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item1, link)) == true);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item2, link)) == true);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item3, link)) == false);

  // remove one item
  ASSERT(mu_queue_remove(q) == MU_LIST_REF(&s_item1, link));
  ASSERT(mu_queue_is_empty(q) == false);
  ASSERT(mu_queue_length(q) == 1);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item1, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item2, link)) == true);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item3, link)) == false);

  // remove remaining item
  ASSERT(mu_queue_remove(q) == MU_LIST_REF(&s_item2, link));
  ASSERT(mu_queue_is_empty(q) == true);
  ASSERT(mu_queue_length(q) == 0);
  ASSERT(mu_queue_remove(q) == NULL);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item1, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item2, link)) == false);
  ASSERT(mu_queue_contains(q, MU_LIST_REF(&s_item3, link)) == false);
}

// =============================================================================
// private code

static void reset(void) {
  queue_item_init(&s_item1, 1.0, 'a');
  queue_item_init(&s_item2, 2.0, 'b');
  queue_item_init(&s_item3, 3.0, 'c');
}

static queue_item_t *queue_item_init(queue_item_t *item, float value, char id) {
  item->value = value;
  item->id = id;
  item->link.next = NULL;
  return item;
}
