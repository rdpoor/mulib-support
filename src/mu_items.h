/** \
 * MIT License \
 * \
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com> \
 * \
 * Permission is hereby granted, free of charge, to any person obtaining a copy \
 * of this software and associated documentation files (the "Software"), to deal \
 * in the Software without restriction, including without limitation the rights \
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell \
 * copies of the Software, and to permit persons to whom the Software is \
 * furnished to do so, subject to the following conditions: \
 * \
 * The above copyright notice and this permission notice shall be included in \
 * all copies or substantial portions of the Software. \
 * \
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE \
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, \
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE \
 * SOFTWARE. \
 */

#ifndef MU_ITEMS_H_
#define MU_ITEMS_H_

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

#define DEFINE_COLLECTION(ctype_name, ctype) \
                                                                               \
typedef struct {                                                               \
  size_t capacity;                                                             \
  size_t count;                                                                \
  ctype *data;                                                                 \
} mu_vect ## ctype_name ## _t;                               \
                                                                               \
typedef struct {                                                               \
  mu_vect ## ctype_name ## _t *c;                             \
  size_t index;                                                                \
} mu_vref ## ctype_name ## _t;                               \
 \
typedef bool (mu_vect ## ctype_name ## _traverse_fn)(mu_vref ## ctype_name ## _t *r, void *target); \
 \
mu_vect ## ctype_name ## _t *mu_vect ## ctype_name ## _init(mu_vect ## ctype_name ## _t *c, ctype *items, size_t n_items); \
mu_vect ## ctype_name ## _t *mu_vect ## ctype_name ## _reset(mu_vect ## ctype_name ## _t *c); \
 \
bool mu_vect ## ctype_name ## _is_empty(mu_vect ## ctype_name ## _t *c); \
bool mu_vect ## ctype_name ## _is_full(mu_vect ## ctype_name ## _t *c); \
 \
mu_collection_err_t mu_vect ## ctype_name ## _first(mu_vect ## ctype_name ## _t *c, ctype *item); \
mu_collection_err_t mu_vect ## ctype_name ## _last(mu_vect ## ctype_name ## _t *c, ctype *item); \
mu_collection_err_t mu_vect ## ctype_name ## _pop(mu_vect ## ctype_name ## _t *c, ctype *item); \
mu_collection_err_t mu_vect ## ctype_name ## _push(mu_vect ## ctype_name ## _t *c, ctype item); \
mu_collection_err_t mu_vect ## ctype_name ## _append(mu_vect ## ctype_name ## _t *c, ctype item); \
mu_collection_err_t mu_vect ## ctype_name ## _remove(mu_vect ## ctype_name ## _t *c, ctype *item); \
 \
mu_collection_err_t mu_vect ## ctype_name ## _deref(mu_vref ## ctype_name ## _t *r, ctype *item); \
mu_collection_err_t mu_vref ## ctype_name ## _first(mu_vect ## ctype_name ## _t *c, mu_vref ## ctype_name ## _t *r); \
mu_collection_err_t mu_vref ## ctype_name ## _last(mu_vect ## ctype_name ## _t *c, mu_vref ## ctype_name ## _t *r); \
mu_collection_err_t mu_vref ## ctype_name ## _next(mu_vref ## ctype_name ## _t *r); \
mu_collection_err_t mu_vref ## ctype_name ## _prev(mu_vref ## ctype_name ## _t *r); \
mu_collection_err_t mu_vref ## ctype_name ## _push(mu_vref ## ctype_name ## _t *r, ctype i); \
mu_collection_err_t mu_vref ## ctype_name ## _pop(mu_vref ## ctype_name ## _t *r, ctype *i); \
mu_collection_err_t mu_vref ## ctype_name ## _append(mu_vref ## ctype_name ## _t *r, ctype i); \
mu_collection_err_t mu_vref ## ctype_name ## _remove(mu_vref ## ctype_name ## _t *r, ctype *i); \
 \
size_t mu_vect ## ctype_name ## _capacity(mu_vect ## ctype_name ## _t *c); \
size_t mu_vect ## ctype_name ## _count(mu_vect ## ctype_name ## _t *c); \
size_t mu_vect ## ctype_name ## _from_array(mu_vect ## ctype_name ## _t *c, ctype items, size_t n_items); \
size_t mu_vect ## ctype_name ## _to_array(mu_vect ## ctype_name ## _t *c, ctype items, size_t n_items); \
 \
void mu_vect ## ctype_name ## _traverse(mu_vect ## ctype_name ## _t *c, mu_vect ## ctype_name ## _traverse_fn fn, ctype target); \

DEFINE_COLLECTION(_foo, foo_t)
DEFINE_COLLECTION(_u8, uint8_t)
DEFINE_COLLECTION(, ctype )

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_ITEMS_H_
