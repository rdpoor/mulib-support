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

#include "../src/mu_queue.h"
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

mu_queue_obj_t obj_a;
mu_queue_obj_t obj_b;
mu_queue_obj_t obj_c;
mu_queue_obj_t obj_d;
mu_queue_obj_t obj_e;
mu_queue_obj_t s_pool[QUEUE_SIZE];

// =============================================================================
// public code

void mu_queue_test() {
  mu_queue_t queue;
  mu_queue_obj_t obj;

  // Require power of two queue size
  UTEST_ASSERTEQ_INT(mu_queue_init(&queue, s_pool, QUEUE_SIZE-1), MU_QUEUE_ERR_SIZE);

  // Valid initialization
  UTEST_ASSERTEQ_INT(mu_queue_init(&queue, s_pool, QUEUE_SIZE), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_capacity(&queue), QUEUE_SIZE);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), 0);

  // basic put / get
  UTEST_ASSERTEQ_INT(mu_queue_put(&queue, obj_a), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), 1);
  UTEST_ASSERTEQ_INT(mu_queue_get(&queue, &obj), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_a);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), 0);

  // put until overflow
  UTEST_ASSERTEQ_INT(mu_queue_put(&queue, obj_a), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_put(&queue, obj_b), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_put(&queue, obj_c), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_put(&queue, obj_d), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), QUEUE_SIZE);
  UTEST_ASSERTEQ_INT(mu_queue_put(&queue, obj_e), MU_QUEUE_ERR_FULL);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), QUEUE_SIZE);

  // get until underflow
  UTEST_ASSERTEQ_INT(mu_queue_get(&queue, &obj), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_a);
  UTEST_ASSERTEQ_INT(mu_queue_get(&queue, &obj), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_b);
  UTEST_ASSERTEQ_INT(mu_queue_get(&queue, &obj), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_c);
  UTEST_ASSERTEQ_INT(mu_queue_get(&queue, &obj), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_PTR(obj, obj_d);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), 0);
  UTEST_ASSERTEQ_INT(mu_queue_get(&queue, &obj), MU_QUEUE_ERR_EMPTY);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), 0);

  // reset
  UTEST_ASSERTEQ_INT(mu_queue_put(&queue, obj_a), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), 1);
  UTEST_ASSERTEQ_INT(mu_queue_reset(&queue), MU_QUEUE_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_queue_count(&queue), 0);
}

// =============================================================================
// private code
