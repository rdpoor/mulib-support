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
// MU_LIST_REF and MU_LIST_CONTAINER macros.
//
typedef struct {
  float value;
  mu_list_t link;  // link is not first slot in structure
  char id;
} element_t;

// =============================================================================
// private declarations

static element_t *rebuild_list(void);

// =============================================================================
// local storage

static element_t s_element_a, s_element_b, s_element_c, s_element_d;

// =============================================================================
// public code

void mu_list_test() {
  mu_list_t list;

  // ==========
  // MU_LIST_REF and MU_LIST_CONTAINER
  ASSERT(MU_LIST_REF(&s_element_a, link) == &s_element_a.link);
  ASSERT(MU_LIST_CONTAINER(&s_element_a.link, element_t, link) == &s_element_a);

  // ==========
  // rebuild_list()
  mu_list_set_rest(&list, MU_LIST_REF(rebuild_list(), link));
  ASSERT(mu_list_rest(&list) == MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_a, link)) == MU_LIST_REF(&s_element_b, link));
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_b, link)) == MU_LIST_REF(&s_element_c, link));
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_c, link)) == MU_LIST_END);

  // ==========
  // mu_list_push()
  rebuild_list();
  mu_list_set_rest(&list, MU_LIST_END);
  // mu_list_push() onto an empty list
  ASSERT(mu_list_push(&list, MU_LIST_REF(&s_element_a, link)) == &list);
  // verify that a was pushed onto list
  ASSERT(mu_list_rest(&list) == MU_LIST_REF(&s_element_a, link));
  // verify that a.rest was unlinked from its original list
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_a, link)) == MU_LIST_END);

  // mu_list_push onto a non-empty list
  ASSERT(mu_list_push(&list, MU_LIST_REF(&s_element_b, link)) == &list);
  // verify that b was pushed onto list
  ASSERT(mu_list_rest(&list) == MU_LIST_REF(&s_element_b, link));
  // verify that b.next points to a
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_b, link)) == MU_LIST_REF(&s_element_a, link));

  // ==========
  // mu_list_pop()
  rebuild_list();
  // mu_list_pop() from an empty list
  ASSERT(mu_list_pop(MU_LIST_REF(&s_element_d, link)) == MU_LIST_END);
  // mu_list_pop() from a non-empty list
  mu_list_set_rest(&list, MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_pop(&list) == MU_LIST_REF(&s_element_a, link));
  // verify that a.rest was unlinked
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_a, link)) == MU_LIST_END);
  // run down the remainder of the list
  ASSERT(mu_list_pop(&list) == MU_LIST_REF(&s_element_b, link));
  ASSERT(mu_list_pop(&list) == MU_LIST_REF(&s_element_c, link));
  ASSERT(mu_list_pop(&list) == MU_LIST_END);
  // mu_list_pop() on an empty list is allowed...
  ASSERT(mu_list_pop(&list) == MU_LIST_END);

  // ==========
  // mu_list_length()
  rebuild_list();
  mu_list_set_rest(&list, MU_LIST_END);
  ASSERT(mu_list_length(&list) == 0);
  mu_list_set_rest(&list, MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_length(&list) == 3);

  // ==========
  // mu_list_find()
  rebuild_list();
  mu_list_set_rest(&list, MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_find(&list, MU_LIST_REF(&s_element_a, link)) == MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_find(&list, MU_LIST_REF(&s_element_b, link)) == MU_LIST_REF(&s_element_b, link));
  ASSERT(mu_list_find(&list, MU_LIST_REF(&s_element_c, link)) == MU_LIST_REF(&s_element_c, link));
  ASSERT(mu_list_find(&list, MU_LIST_REF(&s_element_d, link)) == MU_LIST_END);

  // ==========
  // mu_list_delete()
  // delete first
  rebuild_list();
  mu_list_set_rest(&list, MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_delete(&list, MU_LIST_REF(&s_element_a, link)) == MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_a, link)) == MU_LIST_END);
  ASSERT(mu_list_rest(&list) == MU_LIST_REF(&s_element_b, link));

  // delete not first
  rebuild_list();
  mu_list_set_rest(&list, MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_delete(&list, MU_LIST_REF(&s_element_b, link)) == MU_LIST_REF(&s_element_b, link));
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_b, link)) == MU_LIST_END);
  ASSERT(mu_list_rest(&list) == MU_LIST_REF(&s_element_a, link));

  // delete last
  rebuild_list();
  mu_list_set_rest(&list, MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_delete(&list, MU_LIST_REF(&s_element_c, link)) == MU_LIST_REF(&s_element_c, link));
  ASSERT(mu_list_rest(MU_LIST_REF(&s_element_c, link)) == MU_LIST_END);
  ASSERT(mu_list_rest(&list) == MU_LIST_REF(&s_element_a, link));

  // delete not in list
  rebuild_list();
  mu_list_set_rest(&list, MU_LIST_REF(&s_element_a, link));
  ASSERT(mu_list_delete(&list, MU_LIST_REF(&s_element_d, link)) == MU_LIST_END);
  ASSERT(mu_list_rest(&list) == MU_LIST_REF(&s_element_a, link));
}

// =============================================================================
// private code

static element_t *rebuild_list(void) {
  mu_list_set_rest(MU_LIST_REF(&s_element_a, link), MU_LIST_REF(&s_element_b, link));
  mu_list_set_rest(MU_LIST_REF(&s_element_b, link), MU_LIST_REF(&s_element_c, link));
  mu_list_set_rest(MU_LIST_REF(&s_element_c, link), MU_LIST_END);
  // element_d is an empty list.
  mu_list_set_rest(MU_LIST_REF(&s_element_d, link), MU_LIST_END);

  return &s_element_a;
}
