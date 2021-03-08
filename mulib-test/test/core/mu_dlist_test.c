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
#include <stdlib.h>
#include <stdio.h>

// =============================================================================
// private types and definitions

// Define a data type to be linked into a list.  We've intentionally chosen a
// structure in which the link field not the first field in order to test the
// MU_DLIST_REF and MU_DLIST_CONTAINER macros.
//
typedef struct {
  float value;
  mu_dlist_t list;  // list is not first slot in structure
  char id;
} test_item_t;

// =============================================================================
// private declarations

/**
 * @brief Reset all static storage for this test.
 */
static void reset(void);

// =============================================================================
// local storage

static mu_dlist_t s_head;

static mu_dlist_t s_element_a, s_element_b, s_element_c, s_element_d;

// =============================================================================
// public code

void mu_dlist_test() {
  test_item_t item_a;

  // ==========
  // MU_DLIST_REF and MU_DLIST_CONTAINER
  ASSERT(MU_DLIST_REF(&item_a, list) == &item_a.list);
  ASSERT(MU_DLIST_CONTAINER(&item_a.list, test_item_t, list) == &item_a);

  // ==========
  // reset();
  reset();
  ASSERT(s_head.next == &s_head);
  ASSERT(s_head.prev == &s_head);

  ASSERT(s_element_a.next == &s_element_a);
  ASSERT(s_element_a.prev == &s_element_a);

  ASSERT(s_element_b.next == &s_element_b);
  ASSERT(s_element_b.prev == &s_element_b);

  ASSERT(s_element_c.next == &s_element_c);
  ASSERT(s_element_c.prev == &s_element_c);

  ASSERT(s_element_d.next == &s_element_d);
  ASSERT(s_element_d.prev == &s_element_d);

  // ======================================
  // operations on individual list elements

  reset();
  // mu_dlist_t *mu_dlist_init(mu_dlist_t *e);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_a);

  // bool mu_dlist_is_empty(mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_is_empty(&s_element_a) == true);
  mu_dlist_insert_next(&s_element_a, &s_element_b);
  // list = [a, b]
  ASSERT(mu_dlist_is_empty(&s_element_a) == false);

  // mu_dlist_t *mu_dlist_next(mu_dlist_t *e);
  reset();
  mu_dlist_insert_next(&s_element_a, &s_element_b);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_a);

  // mu_dlist_t *mu_dlist_prev(mu_dlist_t *e);
  reset();
  mu_dlist_insert_next(&s_element_a, &s_element_b);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_a);

  // mu_dlist_t *mu_dlist_insert_next(mu_dlist_t *list, mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_insert_next(&s_element_a, &s_element_b) == &s_element_b);
  ASSERT(mu_dlist_insert_next(&s_element_b, &s_element_c) == &s_element_c);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_c);
  ASSERT(mu_dlist_next(&s_element_c) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_c);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_c) == &s_element_b);

  // mu_dlist_t *mu_dlist_insert_prev(mu_dlist_t *list, mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_insert_prev(&s_element_a, &s_element_d) == &s_element_d);
  ASSERT(mu_dlist_insert_prev(&s_element_d, &s_element_c) == &s_element_c);
  ASSERT(mu_dlist_insert_prev(&s_element_c, &s_element_b) == &s_element_b);
  // [a, b, c, d]
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_c);
  ASSERT(mu_dlist_next(&s_element_c) == &s_element_d);
  ASSERT(mu_dlist_next(&s_element_d) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_d);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_c) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_element_d) == &s_element_c);

  // bool mu_dlist_is_linked(mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_is_linked(&s_element_a) == false);
  mu_dlist_insert_next(&s_element_a, &s_element_b);
  ASSERT(mu_dlist_is_linked(&s_element_a) == true);

  // mu_dlist_t *mu_dlist_unlink(mu_dlist_t *e);
  ASSERT(mu_dlist_unlink(&s_element_b) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_a); // a is now empty
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_b); // b is now unlinked
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_b);

  ASSERT(mu_dlist_unlink(&s_element_b) == NULL); // b is already unlinked

  // ======================================
  // operations on a list head

  // mu_dlist_t *mu_dlist_first(mu_dlist_t *head);
  reset();
  ASSERT(mu_dlist_first(&s_head) == NULL);
  mu_dlist_push(&s_head, &s_element_a);
  ASSERT(mu_dlist_first(&s_head) == &s_element_a);

  // mu_dlist_t *mu_dlist_last(mu_dlist_t *head);
  reset();
  ASSERT(mu_dlist_last(&s_head) == NULL);
  mu_dlist_push_prev(&s_head, &s_element_a);
  ASSERT(mu_dlist_last(&s_head) == &s_element_a);

  // size_t mu_dlist_length(mu_dlist_t *head);
  reset();
  ASSERT(mu_dlist_length(&s_head) == 0);
  mu_dlist_push(&s_head, &s_element_a);
  ASSERT(mu_dlist_length(&s_head) == 1);

  // mu_dlist_t *mu_dlist_find(mu_dlist_t *head, mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_find(&s_head, &s_element_a) == NULL);
  mu_dlist_push(&s_head, &s_element_a);
  ASSERT(mu_dlist_find(&s_head, &s_element_a) == &s_element_a);

  // mu_dlist_t *mu_dlist_find_prev(mu_dlist_t *head, mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_find_prev(&s_head, &s_element_a) == NULL);
  mu_dlist_push(&s_head, &s_element_a);
  ASSERT(mu_dlist_find_prev(&s_head, &s_element_a) == &s_element_a);

  // mu_dlist_t *mu_dlist_push(mu_dlist_t *head, mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_push(&s_head, &s_element_a) == &s_head);
  ASSERT(mu_dlist_push(&s_head, &s_element_b) == &s_head);
  // list = [b, a]
  ASSERT(mu_dlist_next(&s_head) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_a) == &s_head);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_head) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_head);

  // mu_dlist_t *mu_dlist_push_prev(mu_dlist_t *head, mu_dlist_t *e);
  reset();
  ASSERT(mu_dlist_push_prev(&s_head, &s_element_a) == &s_head);
  ASSERT(mu_dlist_push_prev(&s_head, &s_element_b) == &s_head);
  // list = [a, b]
  ASSERT(mu_dlist_next(&s_head) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_b) == &s_head);
  ASSERT(mu_dlist_prev(&s_head) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_head);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_a);

  // mu_dlist_t *mu_dlist_pop(mu_dlist_t *head);
  reset();
  mu_dlist_push(&s_head, &s_element_a);
  mu_dlist_push(&s_head, &s_element_b);

  // pop([b, a]) => [a]
  ASSERT(mu_dlist_pop(&s_head) == &s_element_b);
  ASSERT(mu_dlist_next(&s_head) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_a) == &s_head);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_head) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_head);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_b);

  // list = [a]
  ASSERT(mu_dlist_pop(&s_head) == &s_element_a);
  ASSERT(mu_dlist_next(&s_head) == &s_head);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_head) == &s_head);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_b);

  // list = []
  ASSERT(mu_dlist_pop(&s_head) == NULL);

  // mu_dlist_t *mu_dlist_pop_prev(mu_dlist_t *head);
  reset();
  mu_dlist_push(&s_head, &s_element_a);
  mu_dlist_push(&s_head, &s_element_b);

  // pop_prev([b, a]) => [b]
  ASSERT(mu_dlist_pop_prev(&s_head) == &s_element_a);
  ASSERT(mu_dlist_next(&s_head) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_b) == &s_head);
  ASSERT(mu_dlist_prev(&s_head) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_head);

  // list = [b]
  ASSERT(mu_dlist_pop_prev(&s_head) == &s_element_b);
  ASSERT(mu_dlist_next(&s_head) == &s_head);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_b) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_head) == &s_head);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_b);

  // list = []
  ASSERT(mu_dlist_pop_prev(&s_head) == NULL);



  // void *mu_dlist_traverse(mu_dlist_t *head, mu_dlist_traverse_fn fn, void *arg);
  // tested implicitly via mu_dlist_find()

  // void *mu_dlist_traverse_prev(mu_dlist_t *head, mu_dlist_traverse_fn fn, void *arg);
  // tested implicitly via mu_dlist_find_prev()

  // mu_dlist_t *mu_dlist_reverse(mu_dlist_t *head);
  reset();

  // on empty list.
  ASSERT(mu_dlist_reverse(&s_head) == &s_head);
  ASSERT(mu_dlist_next(&s_head) == &s_head);
  ASSERT(mu_dlist_prev(&s_head) == &s_head);


  reset();
  mu_dlist_push(&s_head, &s_element_a);
  // on singleton list [a]
  ASSERT(mu_dlist_reverse(&s_head) == &s_head);
  ASSERT(mu_dlist_next(&s_head) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_a) == &s_head);
  ASSERT(mu_dlist_prev(&s_head) == &s_element_a);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_head);

  reset();
  mu_dlist_push(&s_head, &s_element_a);
  mu_dlist_push(&s_head, &s_element_b);
  // reverse([b, a]) => [a, b]
  ASSERT(mu_dlist_reverse(&s_head) == &s_head);
  ASSERT(mu_dlist_next(&s_head) == &s_element_a);
  ASSERT(mu_dlist_next(&s_element_a) == &s_element_b);
  ASSERT(mu_dlist_next(&s_element_b) == &s_head);
  ASSERT(mu_dlist_prev(&s_head) == &s_element_b);
  ASSERT(mu_dlist_prev(&s_element_a) == &s_head);
  ASSERT(mu_dlist_prev(&s_element_b) == &s_element_a);
}

// =============================================================================
// private code

/**
 * @brief Reset all static storage for this test.
 */
static void reset(void) {
  mu_dlist_init(&s_head);
  mu_dlist_init(&s_element_a);
  mu_dlist_init(&s_element_b);
  mu_dlist_init(&s_element_c);
  mu_dlist_init(&s_element_d);
}
