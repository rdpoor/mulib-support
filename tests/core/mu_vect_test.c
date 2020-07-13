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

#include "mu_vect.h"
#include "mu_test_utils.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// private types and definitions

#define VECT_CAPACITY 4

typedef struct {
  char id;
  uint8_t val;
} test_element_t;

// =============================================================================
// private declarations

static bool elements_are_equal(test_element_t *e1, test_element_t *e2);
static int sort_up(void *a, void *b);
static int sort_dn(void *a, void *b);
static void *ids_match(void *e, void *arg);

// =============================================================================
// local storage

static test_element_t s_elements[VECT_CAPACITY];

static test_element_t s_element1 = {.id = 'a', .val=9};
static test_element_t s_element2 = {.id = 'b', .val=8};
static test_element_t s_element3 = {.id = 'c', .val=7};
static test_element_t s_element4 = {.id = 'd', .val=6};
static test_element_t s_element5 = {.id = 'e', .val=5};

// =============================================================================
// public code

void mu_vect_test() {
  mu_vect_t vi;
  mu_vect_t *v = &vi;
  test_element_t element;

  // mu_vect_t *mu_vect_init(mu_vect_t *vect, mu_vect_element_t *elements,
  // size_t capacity);
  ASSERT(mu_vect_init(v, s_elements, VECT_CAPACITY, sizeof(test_element_t)) == v);
  ASSERT(mu_vect_elements(v) == s_elements);
  ASSERT(mu_vect_capacity(v) == VECT_CAPACITY);
  ASSERT(mu_vect_element_size(v) == sizeof(test_element_t));
  ASSERT(mu_vect_count(v) == 0);

  // mu_vect_err_t mu_vect_push(mu_vect_t *vect, void *element);
  ASSERT(mu_vect_is_empty(v) == true);
  ASSERT(mu_vect_is_full(v) == false);
  ASSERT(mu_vect_peek(v, &element) == MU_VECT_ERR_EMPTY);

  ASSERT(mu_vect_push(v, &s_element1) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 1);
  ASSERT(mu_vect_is_empty(v) == false);
  ASSERT(mu_vect_is_full(v) == false);
  ASSERT(mu_vect_peek(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element1));

  ASSERT(mu_vect_push(v, &s_element2) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 2);
  ASSERT(mu_vect_is_empty(v) == false);
  ASSERT(mu_vect_is_full(v) == false);
  ASSERT(mu_vect_peek(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element2));

  ASSERT(mu_vect_push(v, &s_element3) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 3);
  ASSERT(mu_vect_is_empty(v) == false);
  ASSERT(mu_vect_is_full(v) == false);
  ASSERT(mu_vect_peek(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element3));

  ASSERT(mu_vect_push(v, &s_element4) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 4);
  ASSERT(mu_vect_is_empty(v) == false);
  ASSERT(mu_vect_is_full(v) == true);
  ASSERT(mu_vect_peek(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element4));

  ASSERT(mu_vect_push(v, &s_element5) == MU_VECT_ERR_FULL);
  ASSERT(mu_vect_count(v) == 4);
  ASSERT(mu_vect_peek(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element4));

  // mu_vect_err_t mu_vect_pop(mu_vect_t *vect, mu_vect_element_t *element);
  ASSERT(mu_vect_pop(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element4));
  ASSERT(mu_vect_count(v) == 3);

  ASSERT(mu_vect_pop(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element3));
  ASSERT(mu_vect_count(v) == 2);

  ASSERT(mu_vect_pop(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element2));
  ASSERT(mu_vect_count(v) == 1);

  ASSERT(mu_vect_pop(v, &element) == MU_VECT_ERR_NONE);
  ASSERT(elements_are_equal(&element, &s_element1));
  ASSERT(mu_vect_count(v) == 0);
  ASSERT(mu_vect_is_empty(v) == true);
  ASSERT(mu_vect_is_full(v) == false);

  ASSERT(mu_vect_pop(v, &element) == MU_VECT_ERR_EMPTY);
  ASSERT(mu_vect_count(v) == 0);

  mu_vect_push(v, &s_element1);
  ASSERT(mu_vect_reset(v) == v);
  ASSERT(mu_vect_count(v) == 0);

  // mu_vect_err_t mu_vect_insert_at(mu_vect_t *vect, mu_vect_element_t
  // element, size_t index); insert into empty store
  ASSERT(mu_vect_insert_at(v, 0, &s_element1) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 1);
  // insert beyond end
  ASSERT(mu_vect_insert_at(v, 2, &s_element2) == MU_VECT_ERR_INDEX);
  ASSERT(mu_vect_count(v) == 1);
  // insert at top
  ASSERT(mu_vect_insert_at(v, 0, &s_element2) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 2);
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element2));
  ASSERT(elements_are_equal(mu_vect_ref(v, 1), &s_element1));
  // insert not at top
  ASSERT(mu_vect_insert_at(v, 1, &s_element3) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 3);
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element2));
  ASSERT(elements_are_equal(mu_vect_ref(v, 1), &s_element3));
  ASSERT(elements_are_equal(mu_vect_ref(v, 2), &s_element1));
  // insert at bottom
  ASSERT(mu_vect_insert_at(v, 3, &s_element4) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 4);
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element2));
  ASSERT(elements_are_equal(mu_vect_ref(v, 1), &s_element3));
  ASSERT(elements_are_equal(mu_vect_ref(v, 2), &s_element1));
  ASSERT(elements_are_equal(mu_vect_ref(v, 3), &s_element4));
  // insert when full
  ASSERT(mu_vect_insert_at(v, 0, &s_element5) == MU_VECT_ERR_FULL);
  ASSERT(mu_vect_count(v) == 4);

  mu_vect_reset(v);
  // insert with illegal index
  ASSERT(mu_vect_insert_at(v, 1, &s_element1) == MU_VECT_ERR_INDEX);
  ASSERT(mu_vect_count(v) == 0);

  // mu_vect_err_t mu_vect_delete_at(mu_vect_t *vect, mu_vect_element_t
  // *element, size_t index);
  mu_vect_reset(v); // []
  // remove empty
  ASSERT(mu_vect_delete_at(v, 3, &element) == MU_VECT_ERR_EMPTY);
  // load up some elements
  mu_vect_push(v, &s_element1); // [element1]
  mu_vect_push(v, &s_element2); // [element1 element2]
  mu_vect_push(v, &s_element3); // [element1 element2 element3]
  mu_vect_push(v, &s_element4); // [element1 element2 element3 element4]
  ASSERT(mu_vect_count(v) == 4);
  // remove at end [a, b, c, d] => [a, b, c]
  ASSERT(mu_vect_delete_at(v, 3, &element) == MU_VECT_ERR_NONE);
  ASSERT(mu_vect_count(v) == 3);
  ASSERT(elements_are_equal(&element, &s_element4));
  // remove at middle [a, b, c] => [a, c]
  ASSERT(mu_vect_delete_at(v, 1, &element) == MU_VECT_ERR_NONE); // [e1 e3]
  ASSERT(mu_vect_count(v) == 2);
  ASSERT(elements_are_equal(&element, &s_element2));
  // remove at head [a, c] => [c]
  ASSERT(mu_vect_delete_at(v, 0, &element) == MU_VECT_ERR_NONE); // [e3]
  ASSERT(mu_vect_count(v) == 1);
  ASSERT(elements_are_equal(&element, &s_element1));
  // remove at with illegal index [c] => [c]
  ASSERT(mu_vect_delete_at(v, 1, &element) == MU_VECT_ERR_INDEX); // [e1]
  ASSERT(mu_vect_count(v) == 1);
  // remove final element [c] => []
  ASSERT(mu_vect_delete_at(v, 0, &element) == MU_VECT_ERR_NONE); // []
  ASSERT(mu_vect_count(v) == 0);
  ASSERT(elements_are_equal(&element, &s_element3));

  // mu_vect_err_t mu_vect_insert_sorted(mu_vect_t *vect,
  // void *element, mu_compare_fn cmp);
  mu_vect_reset(v);
  // insert first
  ASSERT(mu_vect_insert_sorted(v, &s_element2, sort_up) == MU_VECT_ERR_NONE);
  // [b]
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element2));

  ASSERT(mu_vect_insert_sorted(v, &s_element4, sort_up) == MU_VECT_ERR_NONE);
  // [b d]
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element2));
  ASSERT(elements_are_equal(mu_vect_ref(v, 1), &s_element4));

  ASSERT(mu_vect_insert_sorted(v, &s_element3, sort_up) == MU_VECT_ERR_NONE);
  // [b c d]
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element2));
  ASSERT(elements_are_equal(mu_vect_ref(v, 1), &s_element3));
  ASSERT(elements_are_equal(mu_vect_ref(v, 2), &s_element4));

  ASSERT(mu_vect_insert_sorted(v, &s_element1, sort_up) == MU_VECT_ERR_NONE);
  // [a b c d]
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element1));
  ASSERT(elements_are_equal(mu_vect_ref(v, 1), &s_element2));
  ASSERT(elements_are_equal(mu_vect_ref(v, 2), &s_element3));
  ASSERT(elements_are_equal(mu_vect_ref(v, 3), &s_element4));

  ASSERT(mu_vect_insert_sorted(v, &s_element5, sort_up) == MU_VECT_ERR_FULL);
  // [a b c d]

  // mu_vect_err_t mu_vect_sort(mu_vect_t *vect, mu_compare_fn cmp);
  ASSERT(mu_vect_sort(v, sort_dn) == MU_VECT_ERR_NONE); // [d c b a]
  ASSERT(elements_are_equal(mu_vect_ref(v, 0), &s_element4));
  ASSERT(elements_are_equal(mu_vect_ref(v, 1), &s_element3));
  ASSERT(elements_are_equal(mu_vect_ref(v, 2), &s_element2));
  ASSERT(elements_are_equal(mu_vect_ref(v, 3), &s_element1));

  // mu_vect_traverse, mu_vect_find_index, mu_vect_contains
  mu_vect_reset(v);
  ASSERT(mu_vect_traverse(v, ids_match, &s_element1) == NULL);
  ASSERT(mu_vect_find_index(v, ids_match, &s_element1) == -1);
  ASSERT(mu_vect_contains(v, ids_match, &s_element1) == false);

  mu_vect_push(v, &s_element1);
  ASSERT(elements_are_equal(mu_vect_traverse(v, ids_match, &s_element1), &s_element1));
  ASSERT(mu_vect_find_index(v, ids_match, &s_element1) == 0);
  ASSERT(mu_vect_contains(v, ids_match, &s_element1) == true);
}

// =============================================================================
// private code

static bool elements_are_equal(test_element_t *e1, test_element_t *e2) {
  if ((e1 == NULL) || (e2 == NULL)) {
    return e1 == NULL && e2 == NULL;
  } else {
    return e1->id == e2->id && e1->val == e2->val;
  }
}

static int sort_up(void *a, void *b) {
  test_element_t *ae = (test_element_t *)a;
  test_element_t *be = (test_element_t *)b;
  return ae->id - be->id;
}

static int sort_dn(void *a, void *b) {
  test_element_t *ae = (test_element_t *)a;
  test_element_t *be = (test_element_t *)b;
  return be->id - ae->id;
}

static void *ids_match(void *a, void *b) {
  test_element_t *ae = (test_element_t *)a;
  test_element_t *be = (test_element_t *)b;
  if ((ae == NULL) || (be == NULL)) {
    return NULL;
  } else {
    return ae->id == be->id ? a : NULL;
  }
}
