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

#include "../src/mu_ring.h"
#include "test_utilities.h"

// =============================================================================
// private types and definitions

// must be a power of 2
#define QUEUE_SIZE 4

// =============================================================================
// private declarations

// =============================================================================
// local storage

//   #define N 128
//   volatile unsigned int head, tail;
//   volatile char buffer[N];
//   unsigned int inuse() { return head - tail; }
//   void put(char c) { if (inuse() != N) { buffer[head++%N] = c; } }
//   void get(char* c) { if (inuse() != 0) { *c = buffer[tail++%N]; } }

mu_ring_obj_t obj_a;
mu_ring_obj_t obj_b;
mu_ring_obj_t obj_c;
mu_ring_obj_t obj_d;
mu_ring_obj_t obj_e;
mu_ring_obj_t s_pool[QUEUE_SIZE];

// =============================================================================
// public code

void mu_ring_test() {
  mu_ring_t queue;
  mu_ring_t *q = &queue;
  mu_ring_obj_t obj;

  // Require power of two queue size
  UTEST_ASSERTEQ_INT(mu_ring_init(q, s_pool, QUEUE_SIZE-1), MU_RING_ERR_SIZE);

  // Valid initialization
  UTEST_ASSERTEQ_INT(mu_ring_init(q, s_pool, QUEUE_SIZE), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_capacity(q), QUEUE_SIZE);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 0);

  // basic put / get
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_a), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 1);
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_b), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 2);
  UTEST_ASSERTEQ_INT(mu_ring_get(q, &obj), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_b);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 1);
  UTEST_ASSERTEQ_INT(mu_ring_get(q, &obj), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_a);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 0);

  // put until overflow
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_a), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_b), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_c), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_d), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), QUEUE_SIZE);
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_e), MU_RING_ERR_FULL);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), QUEUE_SIZE);

  // get until underflow
  UTEST_ASSERTEQ_INT(mu_ring_get(q, &obj), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_a);
  UTEST_ASSERTEQ_INT(mu_ring_get(q, &obj), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_b);
  UTEST_ASSERTEQ_INT(mu_ring_get(q, &obj), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_c);
  UTEST_ASSERTEQ_INT(mu_ring_get(q, &obj), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_d);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 0);
  UTEST_ASSERTEQ_INT(mu_ring_get(q, &obj), MU_RING_ERR_EMPTY);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 0);

  // reset
  UTEST_ASSERTEQ_INT(mu_ring_put(q, obj_a), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 1);
  UTEST_ASSERTEQ_INT(mu_ring_reset(q), MU_RING_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_ring_count(q), 0);
}

// =============================================================================
// private code
