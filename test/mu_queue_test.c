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

 #include "mu_test_utils.h"
 #include "mu_queue.h"
 #include <string.h>
 #include <stdlib.h>
 #include <stdint.h>

 #include <stdio.h>

// =============================================================================
// private types and definitions

typedef struct {
  mu_link_t link;
  char ch;
} test_queue_t;

// =============================================================================
// private declarations

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_queue_test() {

  mu_queue_t q1i;
  mu_queue_t *q1 = &q1i;
  test_queue_t item1 = {.ch = 'a'};
  test_queue_t item2 = {.ch = 'b'};

  ASSERT(mu_queue_init(q1) == q1);
  ASSERT(mu_queue_is_empty(q1) == true);
  ASSERT(mu_queue_head(q1) == NULL);
  ASSERT(mu_queue_tail(q1) == NULL);
  ASSERT(mu_queue_remove(q1) == NULL);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item1) == false);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item2) == false);

  ASSERT(mu_queue_add(q1, (mu_link_t *)&item1) == q1);
  ASSERT(mu_queue_is_empty(q1) == false);
  ASSERT(mu_queue_head(q1) == (mu_link_t *)&item1);
  ASSERT(mu_queue_tail(q1) == (mu_link_t *)&item1);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item1) == true);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item2) == false);
  ASSERT(mu_queue_remove(q1) == (mu_link_t *)&item1);
  ASSERT(mu_queue_is_empty(q1) == true);

  ASSERT(mu_queue_add(q1, (mu_link_t *)&item1) == q1);
  ASSERT(mu_queue_add(q1, (mu_link_t *)&item2) == q1);
  ASSERT(mu_queue_is_empty(q1) == false);
  ASSERT(mu_queue_head(q1) == (mu_link_t *)&item1);
  ASSERT(mu_queue_tail(q1) == (mu_link_t *)&item2);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item1) == true);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item2) == true);
  ASSERT(mu_queue_remove(q1) == (mu_link_t *)&item1);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item1) == false);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item2) == true);
  ASSERT(mu_queue_remove(q1) == (mu_link_t *)&item2);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item1) == false);
  ASSERT(mu_queue_contains(q1, (mu_link_t *)&item2) == false);
  ASSERT(mu_queue_is_empty(q1) == true);
}
