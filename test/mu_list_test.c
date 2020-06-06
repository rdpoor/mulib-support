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

// =============================================================================
// includes

#include "mu_list.h"
#include "mu_test_utils.h"
#include <stdlib.h>
#include <stdio.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static void initialize_items(void);
static mu_list_t *rebuild_list(void);

// =============================================================================
// local storage

static mu_list_t s_item_a, s_item_b, s_item_c, s_item_d;

// =============================================================================
// public code

void mu_list_test() {
  mu_list_t *item;
  mu_list_t list_head;


  // ==========
  // void mu_list_push(mu_list_t *ref, mu_list_t *item);
  initialize_items();
  list_head.next = NULL;

  ASSERT(list_head.next == NULL);
  // push onto empty list
  mu_list_push(&list_head, &s_item_a);
  ASSERT(list_head.next != NULL);
  ASSERT(list_head.next == &s_item_a);
  // push onto non-empty list
  mu_list_push(&list_head, &s_item_b);
  ASSERT(list_head.next == &s_item_b);
  ASSERT(s_item_b.next == &s_item_a);
  ASSERT(s_item_a.next == NULL);
  // push onto non-empty list
  mu_list_push(&list_head, &s_item_c);
  ASSERT(list_head.next == &s_item_c);
  ASSERT(s_item_c.next == &s_item_b);
  ASSERT(s_item_b.next == &s_item_a);
  ASSERT(s_item_a.next == NULL);

  // ==========
  // mu_list_t *mu_list_pop(mu_list_t *ref);
  list_head.next = rebuild_list();
  item = mu_list_pop(&list_head);
  ASSERT(item == &s_item_a);
  ASSERT(item->next == NULL);  // assure item link is null
  item = mu_list_pop(&list_head);
  ASSERT(item == &s_item_b);
  ASSERT(item->next == NULL);  // assure item link is null
  item = mu_list_pop(&list_head);
  ASSERT(item == &s_item_c);
  ASSERT(item->next == NULL);  // assure item link is null
  // pop from empty list
  ASSERT(list_head.next == NULL);
  item = mu_list_pop(&list_head);
  ASSERT(item == NULL);

  // ==========
  // void *mu_list_traverse(mu_list_t *ref, mu_list_traverse_fn fn, void *arg)
  // tested implicitly by the following functions

  // ==========
  // int mu_list_length(mu_list_t *ref);
  list_head.next = rebuild_list();
  ASSERT(mu_list_length(&list_head) == 3);
  list_head.next = NULL;
  ASSERT(mu_list_length(&list_head) == 0);
  ASSERT(mu_list_length(NULL) == 0);

  // =========
  // bool mu_list_contains(mu_list_t *ref, mu_list_t *item);
  list_head.next = rebuild_list();
  ASSERT(mu_list_contains(&list_head, &s_item_a) == true);
  ASSERT(mu_list_contains(&list_head, &s_item_b) == true);
  ASSERT(mu_list_contains(&list_head, &s_item_c) == true);
  ASSERT(mu_list_contains(&list_head, &s_item_d) == false);
  // we could debate whether this should return true or false...
  ASSERT(mu_list_contains(&list_head, NULL) == false);
  list_head.next = NULL;
  ASSERT(mu_list_contains(&list_head, &s_item_a) == false);
  ASSERT(mu_list_contains(NULL, &s_item_a) == false);

  // =========
  // mu_list_t *mu_list_delete(mu_list_t *ref, mu_list_t *item);
  // delete first in list
  list_head.next = rebuild_list();
  ASSERT(mu_list_delete(&list_head, &s_item_a) == &s_item_a);
  ASSERT(list_head.next == &s_item_b);
  ASSERT(s_item_a.next == NULL);
  ASSERT(s_item_b.next == &s_item_c);
  ASSERT(s_item_c.next == NULL);

  // delete mid-head
  list_head.next = rebuild_list();
  ASSERT(mu_list_delete(&list_head, &s_item_b) == &s_item_b);
  ASSERT(list_head.next == &s_item_a);
  ASSERT(s_item_a.next == &s_item_c);
  ASSERT(s_item_b.next == NULL);
  ASSERT(s_item_c.next == NULL);

  // delete last
  list_head.next = rebuild_list();
  ASSERT(mu_list_delete(&list_head, &s_item_c) == &s_item_c);
  ASSERT(list_head.next == &s_item_a);
  ASSERT(s_item_a.next == &s_item_b);
  ASSERT(s_item_b.next == NULL);       // is now end of list
  ASSERT(s_item_c.next == NULL);

  // delete not-in-list
  list_head.next = rebuild_list();
  ASSERT(mu_list_delete(&list_head, &s_item_d) == NULL);
  ASSERT(list_head.next == &s_item_a);
  ASSERT(s_item_a.next == &s_item_b);
  ASSERT(s_item_b.next == &s_item_c);
  ASSERT(s_item_c.next == NULL);

  // delete null item
  list_head.next = rebuild_list();
  ASSERT(mu_list_delete(&list_head, NULL) == NULL);

  // delete from null list
  list_head.next = NULL;
  ASSERT(mu_list_delete(&list_head, &s_item_a) == NULL);

  // delete null reference
  ASSERT(mu_list_delete(NULL, &s_item_a) == NULL);

  // ==========
  // mu_list_t *mu_list_reverse(mu_list_t *ref)
  list_head.next = rebuild_list();
  ASSERT(mu_list_reverse(&list_head) == &list_head);
  ASSERT(list_head.next == &s_item_c);
  ASSERT(s_item_c.next == &s_item_b);
  ASSERT(s_item_b.next == &s_item_a);
  ASSERT(s_item_a.next == NULL);

  // reverse singleton list
  rebuild_list();
  list_head.next = &s_item_c;
  ASSERT(mu_list_reverse(&list_head) == &list_head);
  ASSERT(list_head.next == &s_item_c);
  ASSERT(s_item_c.next == NULL);

  // reverse null list
  list_head.next = NULL;
  ASSERT(mu_list_reverse(&list_head) == &list_head);
  ASSERT(list_head.next == NULL);

  // reverse null reference
  ASSERT(mu_list_reverse(NULL) == NULL);

}


// =============================================================================
// private code

static void initialize_items(void) {
  s_item_a.next = NULL;
  s_item_b.next = NULL;
  s_item_c.next = NULL;
  s_item_d.next = NULL;
}

static mu_list_t *rebuild_list(void) {
  s_item_a.next = &s_item_b;
  s_item_b.next = &s_item_c;
  s_item_c.next = NULL;
  // item_d is intentionally omitted
  s_item_d.next = NULL;

  return &s_item_a;
}
