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

#ifndef MU_QUEUE_H_
#define MU_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

// =============================================================================
// types and definitions


//   volatile unsigned int head, tail;
//   volatile char buffer[N];
//   unsigned int inuse() { return head - tail; }
//   void put(char c) { if (inuse() != N) { buffer[head++%N] = c; } }
//   void get(char* c) { if (inuse() != 0) { *c = buffer[tail++%N]; } }

typedef enum {
  MU_QUEUE_ERR_NONE,
  MU_QUEUE_ERR_EMPTY,
  MU_QUEUE_ERR_FULL,
  MU_QUEUE_ERR_SIZE,
} mu_queue_err_t;

// mu_queue manages pointer-sized objects
typedef void * mu_queue_obj_t;

typedef struct {
  unsigned int mask;
  volatile unsigned int head;
  volatile unsigned int tail;
  mu_queue_obj_t *pool;
} mu_queue_t;

// =============================================================================
// declarations

#ifdef __cplusplus
}
#endif

mu_queue_err_t mu_queue_init(mu_queue_t *q, mu_queue_obj_t *pool, unsigned int capacity);
mu_queue_err_t mu_queue_reset(mu_queue_t *q);
unsigned int mu_queue_capacity(mu_queue_t *q);
unsigned int mu_queue_count(mu_queue_t *q);
mu_queue_err_t mu_queue_put(mu_queue_t *q, mu_queue_obj_t obj);
mu_queue_err_t mu_queue_get(mu_queue_t *q, mu_queue_obj_t *obj);

#endif // #ifndef MU_QUEUE_H_
