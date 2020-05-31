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

#include "mu_queue.h"
#include "mu_types.h"
#include <stdbool.h>
#include <stddef.h>

// #include <stdio.h>  // debugging

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

// these should go in their own file
static mu_link_t *mu_list_push(mu_link_t *list, mu_link_t *item);
static mu_link_t *mu_list_pop(mu_link_t **list);
static bool mu_list_contains(mu_link_t *list, mu_link_t *item);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_queue_t *mu_queue_init(mu_queue_t *q) {
  q->head = NULL;
  q->tail = NULL;
  return q;
}

mu_link_t *mu_queue_head(mu_queue_t *q) {
  return q->head;
}

mu_link_t *mu_queue_tail(mu_queue_t *q) {
  return q->tail;
}

mu_queue_t *mu_queue_add(mu_queue_t *q, mu_link_t *item) {
  if (q->tail == NULL) {
    q->head = item;      // adding first item
  } else {
    mu_list_push(item, q->tail);
  }
  q->tail = item;
  return q;
}

mu_link_t *mu_queue_remove(mu_queue_t *q) {
  mu_link_t *item = mu_list_pop(&q->head);
  if (q->head == NULL) {
    q->tail = NULL;       // removing last item;
  }
  return item;
}

bool mu_queue_is_empty(mu_queue_t *q) {
  return q->head == NULL;   // can check head or tail
}

bool mu_queue_contains(mu_queue_t *q, mu_link_t *item) {
  return mu_list_contains(q->head, item);
}

// =============================================================================
// local (static) code

static mu_link_t *mu_list_push(mu_link_t *list, mu_link_t *item) {
  item->next = list;
  return item;
}

static mu_link_t *mu_list_pop(mu_link_t **list) {
  mu_link_t *item = *list;
  if (item) {
    *list = item->next;
    item->next = NULL;   // housekeeping
  }
  return item;
}

static bool mu_list_contains(mu_link_t *list, mu_link_t *item) {
  while (list) {
    if (list == item) {
      return true;
    }
    list = list->next;
  }
  return false;
}
