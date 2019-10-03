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

#ifndef MU_COLLECTION_H_
#define MU_COLLECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

typedef enum {
  MU_COLLECTION_ERR_NONE,
  MU_COLLECTION_ERR_EMPTY,
  MU_COLLECTION_ERR_FULL,
  MU_COLLECTION_ERR_BOUNDS,
} mu_collection_err_t;

typedef void * mu_vect_item_t;

typedef struct {
  size_t capacity;
  size_t count;
  void **data;
} mu_vect_t;

typedef struct {
  mu_vect_t *c;
  size_t index;
} mu_vref_t;

typedef bool (mu_vect_traverse_fn)(mu_vref_t *r, void *target);

mu_vect_t *mu_vect_init(mu_vect_t *c, void **items, size_t n_items);
mu_vect_t *mu_vect_reset(mu_vect_t *c);

bool mu_vect_is_empty(mu_vect_t *c);
bool mu_vect_is_full(mu_vect_t *c);

mu_collection_err_t mu_vect_first(mu_vect_t *c, void **item);
mu_collection_err_t mu_vect_last(mu_vect_t *c, void **item);
mu_collection_err_t mu_vect_pop(mu_vect_t *c, void **item);
mu_collection_err_t mu_vect_push(mu_vect_t *c, void *item);
mu_collection_err_t mu_vect_append(mu_vect_t *c, void *item);
mu_collection_err_t mu_vect_remove(mu_vect_t *c, void **item);

mu_collection_err_t mu_vect_deref(mu_vref_t *r, void **item);
mu_collection_err_t mu_vref_first(mu_vect_t *c, mu_vref_t *r);
mu_collection_err_t mu_vref_last(mu_vect_t *c, mu_vref_t *r);
mu_collection_err_t mu_vref_next(mu_vref_t *r);
mu_collection_err_t mu_vref_prev(mu_vref_t *r);
mu_collection_err_t mu_vref_push(mu_vref_t *r, void *i);
mu_collection_err_t mu_vref_pop(mu_vref_t *r, void **i);
mu_collection_err_t mu_vref_append(mu_vref_t *r, void *i);
mu_collection_err_t mu_vref_remove(mu_vref_t *r, void **i);

size_t mu_vect_capacity(mu_vect_t *c);
size_t mu_vect_count(mu_vect_t *c);
size_t mu_vect_from_array(mu_vect_t *c, void *items, size_t n_items);
size_t mu_vect_to_array(mu_vect_t *c, void *items, size_t n_items);

void mu_vect_traverse(mu_vect_t *c, mu_vect_traverse_fn fn, void *target);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_COLLECTION_H_
