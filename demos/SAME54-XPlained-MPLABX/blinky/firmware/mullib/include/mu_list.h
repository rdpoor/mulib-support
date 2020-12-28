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

#ifndef _MU_LIST_H_
#define _MU_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// types and definitions

typedef struct _mu_list {
  struct _mu_list *next;
} mu_list_t;

typedef void *(*mu_list_traverse_fn)(mu_list_t *ref, void *arg);

/**
 * @brief Given a pointer to a structure and the name of the slot containing a
 * mu_list_t, return a pointer to the mu_list_t.
 */
#define MU_LIST_REF(_struct_p, _list_slot) (&((_struct_p)->_list_slot))

/**
 * @brief Given a pointer to a mu_list_t slot within a containing structure,
 * return a pointer to the containing structure.
 */
#define MU_LIST_CONTAINER(_ptr, _type, _member) \
  ((_type *)((char *)(1 ? (_ptr) : &((_type *)0)->_member) - offsetof(_type, _member)))

// =============================================================================
// declarations

void mu_list_push(mu_list_t *ref, mu_list_t *item);

mu_list_t *mu_list_pop(mu_list_t *ref);

/**
 * @brief Call traverse_fn with each successive element of a list, stopping at
 * the end of the list or when the traverse_fn returns a non-NULL value.
 */
void *mu_list_traverse(mu_list_t *ref, mu_list_traverse_fn fn, void *arg);

int mu_list_length(mu_list_t *ref);

bool mu_list_contains(mu_list_t *ref, mu_list_t *item);

mu_list_t *mu_list_delete(mu_list_t *ref, mu_list_t *item);

mu_list_t *mu_list_reverse(mu_list_t *ref);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_LIST_H_ */
