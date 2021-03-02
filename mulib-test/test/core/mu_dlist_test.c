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
} element_t;

// =============================================================================
// private declarations

/**
 * @brief Reset all static storage for this test.
 */
static void reset(void);

/**
 * @brief Return a reference to the list associated with the given element.
 */
static mu_dlist_t *detl(element_t *element);

/**
 * @brief Return a reference to the element associated with the given list.
 */
static element_t *defl(mu_dlist_t *list);

// =============================================================================
// local storage

static mu_dlist_t s_list;

static element_t s_element_a, s_element_b, s_element_c, s_element_d;

// =============================================================================
// public code

void mu_dlist_test() {

  // ==========
  // MU_DLIST_REF and MU_DLIST_CONTAINER
  reset();
  ASSERT(detl(&s_element_a) == &s_element_a.list);
  ASSERT(defl(&s_element_a.list) == &s_element_a);

  // ==========
  // reset();
  reset();
  ASSERT(s_list.next == &s_list);
  ASSERT(s_list.prev == &s_list);

  ASSERT(s_element_a.list.next == NULL);
  ASSERT(s_element_a.list.prev == NULL);
  ASSERT(s_element_a.value = 1.0);
  ASSERT(s_element_a.id = 'a');

  ASSERT(s_element_b.list.next == NULL);
  ASSERT(s_element_b.list.prev == NULL);
  ASSERT(s_element_b.value = 2.0);
  ASSERT(s_element_b.id = 'b');

  ASSERT(s_element_c.list.next == NULL);
  ASSERT(s_element_c.list.prev == NULL);
  ASSERT(s_element_c.value = 3.0);
  ASSERT(s_element_c.id = 'c');

  ASSERT(s_element_d.list.next == NULL);
  ASSERT(s_element_d.list.prev == NULL);
  ASSERT(s_element_d.value = 4.0);
  ASSERT(s_element_d.id = 'd');

  // ==========
  // operations on an empty list
  reset();
  ASSERT(mu_dlist_is_empty(&s_list) == true);
  ASSERT(mu_dlist_length(&s_list) == 0);
  ASSERT(mu_dlist_contains(&s_list, detl(&s_element_a)) == false);
  ASSERT(mu_dlist_first(&s_list) == NULL);
  ASSERT(mu_dlist_last(&s_list) == NULL);

  // ==========
  // mu_dlist_push()

  // mu_dlist_push() on empty list
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 1);
  ASSERT(mu_dlist_contains(&s_list, detl(&s_element_a)) == true);
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));

  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == &s_list);


  // mu_dlist_push() on non-empty list
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 2);
  ASSERT(mu_dlist_contains(&s_list, detl(&s_element_a)) == true);
  ASSERT(mu_dlist_contains(&s_list, detl(&s_element_b)) == true);
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_b));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));

  ASSERT(mu_dlist_element_is_linked(detl(&s_element_b)) == true);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_b)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_b)) == detl(&s_element_a));
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == detl(&s_element_b));
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == &s_list);

  // ==========
  // mu_dlist_lpush()

  // mu_dlist_lpush() on empty list
  reset();
  ASSERT(mu_dlist_lpush(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 1);
  ASSERT(mu_dlist_contains(&s_list, detl(&s_element_a)) == true);
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));

  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == &s_list);

  // mu_dlist_lpush() on non-empty list
  reset();
  ASSERT(mu_dlist_lpush(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_lpush(&s_list, detl(&s_element_b)) == &s_list);
  // list = [a, b]
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 2);
  ASSERT(mu_dlist_contains(&s_list, detl(&s_element_a)) == true);
  ASSERT(mu_dlist_contains(&s_list, detl(&s_element_b)) == true);
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_b));

  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_b)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == detl(&s_element_b));
  ASSERT(mu_dlist_prev_element(detl(&s_element_b)) == detl(&s_element_a));
  ASSERT(mu_dlist_next_element(detl(&s_element_b)) == &s_list);

  // ==========
  // mu_dlist_pop()

  // mu_dlist_pop() on empty list
  reset();
  ASSERT(mu_dlist_pop(&s_list) == NULL);

  // mu_dlist_pop() on singleton list
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  // list = [a]
  ASSERT(mu_dlist_pop(&s_list) == detl(&s_element_a));
  // list = []
  ASSERT(mu_dlist_is_empty(&s_list) == true);
  ASSERT(mu_dlist_length(&s_list) == 0);
  ASSERT(mu_dlist_first(&s_list) == NULL);
  ASSERT(mu_dlist_last(&s_list) == NULL);

  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == false);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == NULL);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == NULL);


  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_b));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));

  ASSERT(mu_dlist_pop(&s_list) == detl(&s_element_b));
  // list = [a]
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 1);
  ASSERT(mu_dlist_pop(&s_list) == detl(&s_element_a));
  // list = []
  ASSERT(mu_dlist_first(&s_list) == NULL);
  ASSERT(mu_dlist_last(&s_list) == NULL);
  ASSERT(mu_dlist_is_empty(&s_list) == true);
  ASSERT(mu_dlist_length(&s_list) == 0);

  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == false);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == NULL);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == NULL);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_b)) == false);
  ASSERT(mu_dlist_prev_element(detl(&s_element_b)) == NULL);
  ASSERT(mu_dlist_next_element(detl(&s_element_b)) == NULL);

  // ==========
  // mu_dlist_lpop()

  // mu_dlist_lpop() on empty list
  reset();
  ASSERT(mu_dlist_lpop(&s_list) == NULL);

  // mu_dlist_pop() on singleton list
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_lpop(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_is_empty(&s_list) == true);
  ASSERT(mu_dlist_length(&s_list) == 0);

  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_dlist_lpop(&s_list) == detl(&s_element_a));
  // list = [b]
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 1);
  ASSERT(mu_dlist_lpop(&s_list) == detl(&s_element_b));
  // list = []
  ASSERT(mu_dlist_is_empty(&s_list) == true);
  ASSERT(mu_dlist_length(&s_list) == 0);

  // ==========
  // mu_dlist_length() - already tested

  // ==========
  // mu_dlist_find()
  reset();

  // mu_dlist_find() on empty list
  ASSERT(mu_dlist_find(&s_list, detl(&s_element_a)) == NULL);

  // mu_dlist_find() on first and second elements of non-empty list
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_dlist_find(&s_list, detl(&s_element_a)) == detl(&s_element_a));
  ASSERT(mu_dlist_find(&s_list, detl(&s_element_b)) == detl(&s_element_b));
  ASSERT(mu_dlist_find(&s_list, detl(&s_element_c)) == NULL);

  // ==========
  // mu_dlist_lfind()
  reset();

  // mu_dlist_lfind() on empty list
  ASSERT(mu_dlist_find(&s_list, detl(&s_element_a)) == NULL);

  // mu_dlist_find() on first and second elements of non-empty list
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_dlist_lfind(&s_list, detl(&s_element_a)) == detl(&s_element_a));
  ASSERT(mu_dlist_lfind(&s_list, detl(&s_element_b)) == detl(&s_element_b));
  ASSERT(mu_dlist_lfind(&s_list, detl(&s_element_c)) == NULL);

  // ==========
  // mu_dlist_reverse()

  // mu_dlist_reverse() an empty list
  reset();
  ASSERT(mu_dlist_reverse(&s_list) == &s_list);
  ASSERT(mu_dlist_is_empty(&s_list) == true);
  ASSERT(mu_dlist_length(&s_list) == 0);

  // mu_dlist_reverse() a singleton list
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  // list is [a]
  ASSERT(mu_dlist_reverse(&s_list) == &s_list);
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 1);
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));

  // mu_dlist_reverse() a multi-element list
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_c)) == &s_list);
  // list is [c, b, a]
  ASSERT(mu_dlist_reverse(&s_list) == &s_list);
  // list is [a, b, c]
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 3);
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_c));

  // =========
  // mu_dlist_traverse() is intrinsically tested by other functions

  // ==========
  // mu_dlist_is_linked(), mu_dlist_unlink(), mu_dlist_next_element(), mu_dlist_previous_element()

  // mu_dlist_unlink() on singleton element
  reset();
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == false);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  // list = [a]
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 1);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == &s_list);

  ASSERT(mu_dlist_unlink_element(detl(&s_element_a)) == detl(&s_element_a));
  // list = []
  ASSERT(mu_dlist_first(&s_list) == NULL);
  ASSERT(mu_dlist_last(&s_list) == NULL);
  ASSERT(mu_dlist_is_empty(&s_list) == true);
  ASSERT(mu_dlist_length(&s_list) == 0);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == false);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == NULL);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == NULL);

  // mu_dlist_unlink() on last element
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_b));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 2);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_b)) == true);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_b)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_b)) == detl(&s_element_a));
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == detl(&s_element_b));
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == &s_list);

  ASSERT(mu_dlist_unlink_element(detl(&s_element_a)) == detl(&s_element_a));
  // list is [b]
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_b));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_b));
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 1);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_b)) == true);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == false);
  ASSERT(mu_dlist_prev_element(detl(&s_element_b)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_b)) == &s_list);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == NULL);
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == NULL);

  // mu_dlist_unlink() on middle element
  reset();
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_a)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_b)) == &s_list);
  ASSERT(mu_dlist_push(&s_list, detl(&s_element_c)) == &s_list);
  // list is now [c, b, a]
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_c));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 3);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_c)) == true);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_b)) == true);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_c)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_c)) == detl(&s_element_b));
  ASSERT(mu_dlist_prev_element(detl(&s_element_b)) == detl(&s_element_c));
  ASSERT(mu_dlist_next_element(detl(&s_element_b)) == detl(&s_element_a));
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == detl(&s_element_b));
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == &s_list);

  ASSERT(mu_dlist_unlink_element(detl(&s_element_b)) == detl(&s_element_b));
  // list is now [c, a]
  ASSERT(mu_dlist_first(&s_list) == detl(&s_element_c));
  ASSERT(mu_dlist_last(&s_list) == detl(&s_element_a));
  ASSERT(mu_dlist_is_empty(&s_list) == false);
  ASSERT(mu_dlist_length(&s_list) == 2);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_c)) == true);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_b)) == false);
  ASSERT(mu_dlist_element_is_linked(detl(&s_element_a)) == true);
  ASSERT(mu_dlist_prev_element(detl(&s_element_c)) == &s_list);
  ASSERT(mu_dlist_next_element(detl(&s_element_c)) == detl(&s_element_a));
  ASSERT(mu_dlist_prev_element(detl(&s_element_b)) == NULL);
  ASSERT(mu_dlist_next_element(detl(&s_element_b)) == NULL);
  ASSERT(mu_dlist_prev_element(detl(&s_element_a)) == detl(&s_element_c));
  ASSERT(mu_dlist_next_element(detl(&s_element_a)) == &s_list);
}

// =============================================================================
// private code

/**
 * @brief Reset all static storage for this test.
 */
static void reset(void) {
  mu_dlist_init(&s_list);
  s_element_a = (element_t){.value = 1.0, .list.next = NULL, .id = 'a'};
  s_element_b = (element_t){.value = 2.0, .list.next = NULL, .id = 'b'};
  s_element_c = (element_t){.value = 3.0, .list.next = NULL, .id = 'c'};
  s_element_d = (element_t){.value = 4.0, .list.next = NULL, .id = 'd'};
}

/**
 * @brief Return a reference to the list associated with the given element.
 */
static mu_dlist_t *detl(element_t *element) {
  return MU_DLIST_REF(element, list);
}

/**
 * @brief Return a reference to the element associated with the given list.
 */
static element_t *defl(mu_dlist_t *list) {
  return MU_DLIST_CONTAINER(list, element_t, list);
}
