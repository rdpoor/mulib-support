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
// structure in which the next field not the first field in order to test the
// MU_LIST_REF and MU_LIST_CONTAINER macros.
//
typedef struct {
  float value;
  mu_list_t list;  // list is not first slot in structure
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
static mu_list_t *etl(element_t *element);

/**
 * @brief Return a reference to the element associated with the given list.
 */
static element_t *efl(mu_list_t *list);

// =============================================================================
// local storage

static mu_list_t s_list;

static element_t s_element_a, s_element_b, s_element_c, s_element_d;

// =============================================================================
// public code

void mu_list_test() {

  // ==========
  // MU_LIST_REF and MU_LIST_CONTAINER
  reset();
  ASSERT(etl(&s_element_a) == &s_element_a.list);
  ASSERT(efl(&s_element_a.list) == &s_element_a);

  // ==========
  // reset();
  reset();
  ASSERT(s_list.next == NULL);

  ASSERT(s_element_a.list.next == NULL);
  ASSERT(s_element_a.value = 1.0);
  ASSERT(s_element_a.id = 'a');

  ASSERT(s_element_b.list.next == NULL);
  ASSERT(s_element_b.value = 2.0);
  ASSERT(s_element_b.id = 'b');

  ASSERT(s_element_c.list.next == NULL);
  ASSERT(s_element_c.value = 3.0);
  ASSERT(s_element_c.id = 'c');

  ASSERT(s_element_d.list.next == NULL);
  ASSERT(s_element_d.value = 4.0);
  ASSERT(s_element_d.id = 'd');

  // ==========
  // operations on an empty list
  reset();
  ASSERT(mu_list_is_empty(&s_list) == true);
  ASSERT(mu_list_length(&s_list) == 0);
  ASSERT(mu_list_contains(&s_list, etl(&s_element_a)) == false);
  ASSERT(mu_list_first(&s_list) == NULL);

  // ==========
  // mu_list_push()

  // mu_list_push() on empty list
  reset();
  ASSERT(mu_list_push(&s_list, etl(&s_element_a)) == &s_list);
  ASSERT(mu_list_is_empty(&s_list) == false);
  ASSERT(mu_list_length(&s_list) == 1);
  ASSERT(mu_list_contains(&s_list, etl(&s_element_a)) == true);
  ASSERT(mu_list_first(&s_list) == etl(&s_element_a));

  // mu_list_push() on non-empty list
  reset();
  ASSERT(mu_list_push(&s_list, etl(&s_element_a)) == &s_list);
  ASSERT(mu_list_push(&s_list, etl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_list_is_empty(&s_list) == false);
  ASSERT(mu_list_length(&s_list) == 2);
  ASSERT(mu_list_contains(&s_list, etl(&s_element_a)) == true);
  ASSERT(mu_list_contains(&s_list, etl(&s_element_b)) == true);
  ASSERT(mu_list_first(&s_list) == etl(&s_element_b));

  // ==========
  // mu_list_pop()

  // mu_list_pop() on empty list
  reset();
  ASSERT(mu_list_pop(&s_list) == NULL);

  // mu_list_pop() on singleton list
  reset();
  ASSERT(mu_list_push(&s_list, etl(&s_element_a)) == &s_list);
  ASSERT(mu_list_pop(&s_list) == etl(&s_element_a));
  ASSERT(mu_list_is_empty(&s_list) == true);
  ASSERT(mu_list_length(&s_list) == 0);

  reset();
  ASSERT(mu_list_push(&s_list, etl(&s_element_a)) == &s_list);
  ASSERT(mu_list_push(&s_list, etl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_list_pop(&s_list) == etl(&s_element_b));
  ASSERT(mu_list_is_empty(&s_list) == false);
  ASSERT(mu_list_length(&s_list) == 1);
  ASSERT(mu_list_pop(&s_list) == etl(&s_element_a));
  ASSERT(mu_list_is_empty(&s_list) == true);
  ASSERT(mu_list_length(&s_list) == 0);

  // ==========
  // mu_list_length() - already tested

  // ==========
  // mu_list_find()
  reset();

  // mu_list_find() on empty list
  ASSERT(mu_list_find(&s_list, etl(&s_element_a)) == NULL);

  // mu_list_find() on first and second elements of non-empty list
  ASSERT(mu_list_push(&s_list, etl(&s_element_a)) == &s_list);
  ASSERT(mu_list_push(&s_list, etl(&s_element_b)) == &s_list);
  // list = [b, a]
  ASSERT(mu_list_find(&s_list, etl(&s_element_a)) == etl(&s_element_a));
  ASSERT(mu_list_find(&s_list, etl(&s_element_b)) == etl(&s_element_b));
  ASSERT(mu_list_find(&s_list, etl(&s_element_c)) == NULL);

  // ==========
  // mu_list_reverse()

  // mu_list_reverse() an empty list
  reset();
  ASSERT(mu_list_reverse(&s_list) == &s_list);
  ASSERT(mu_list_is_empty(&s_list) == true);
  ASSERT(mu_list_length(&s_list) == 0);

  // mu_list_reverse() a singleton list
  reset();
  ASSERT(mu_list_push(&s_list, etl(&s_element_a)) == &s_list);
  // list is [a]
  ASSERT(mu_list_reverse(&s_list) == &s_list);
  ASSERT(mu_list_is_empty(&s_list) == false);
  ASSERT(mu_list_length(&s_list) == 1);
  ASSERT(mu_list_first(&s_list) == etl(&s_element_a));

  // mu_list_reverse() a multi-element list
  reset();
  ASSERT(mu_list_push(&s_list, etl(&s_element_a)) == &s_list);
  ASSERT(mu_list_push(&s_list, etl(&s_element_b)) == &s_list);
  ASSERT(mu_list_push(&s_list, etl(&s_element_c)) == &s_list);
  // list is [c, b, a]
  ASSERT(mu_list_reverse(&s_list) == &s_list);
  // list is [a, b, c]
  ASSERT(mu_list_is_empty(&s_list) == false);
  ASSERT(mu_list_length(&s_list) == 3);
  ASSERT(mu_list_first(&s_list) == etl(&s_element_a));

  // =========
  // mu_list_traverse() is intrinsically tested by other functions

}

// =============================================================================
// private code

/**
 * @brief Reset all static storage for this test.
 */
static void reset(void) {
  s_list.next = NULL;
  s_element_a = (element_t){.value = 1.0, .list.next = NULL, .id = 'a'};
  s_element_b = (element_t){.value = 2.0, .list.next = NULL, .id = 'b'};
  s_element_c = (element_t){.value = 3.0, .list.next = NULL, .id = 'c'};
  s_element_d = (element_t){.value = 4.0, .list.next = NULL, .id = 'd'};
}

/**
 * @brief Return a reference to the list associated with the given element.
 */
static mu_list_t *etl(element_t *element) {
  return MU_LIST_REF(element, list);
}

/**
 * @brief Return a reference to the element associated with the given list.
 */
static element_t *efl(mu_list_t *list) {
  return MU_LIST_CONTAINER(list, element_t, list);
}
