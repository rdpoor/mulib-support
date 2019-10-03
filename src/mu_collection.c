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

#include "./mu_collection.h"
#include <string.h>

static bool ref_is_valid(mu_vect_ref_t *r);
static mu_collection_err_t validate_ref(mu_vect_ref_t *r);
static mu_collection_err_t ref_push(mu_vect_ref_t *r,
                                    mu_vect_item_t item,
                                    size_t before);
static mu_collection_err_t ref_pop(mu_vect_ref_t *r, mu_vect_item_t *item);

// =============================================================================
// public code

mu_vect_t *mu_vect_init(mu_vect_t *c, mu_vect_item_t *items, size_t n_items) {
  c->capacity = n_items;
  c->count = 0;
  c->data = items;
  return c;
}

mu_vect_t *mu_vect_reset(mu_vect_t *c) {
  c->count = 0;
  return c;
}

size_t mu_vect_capacity(mu_vect_t *c) {
  return c->capacity;
}

size_t mu_vect_count(mu_vect_t *c) {
  return c->count;
}

bool mu_vect_is_empty(mu_vect_t *c) {
  return c->count == 0;
}

bool mu_vect_is_full(mu_vect_t *c) {
  return c->count == c->capacity;
}

mu_collection_err_t mu_vect_first(mu_vect_t *c, mu_vect_item_t *item) {
  if (mu_vect_is_empty(c)) {
    return MU_COLLECTION_ERR_EMPTY;
  }
  *item = c->data[0];
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_last(mu_vect_t *c, mu_vect_item_t *item) {
  if (mu_vect_is_empty(c)) {
    return MU_COLLECTION_ERR_EMPTY;
  }
  *item = c->data[c->count - 1];
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_push(mu_vect_t *c, mu_vect_item_t item) {
  if (mu_vect_is_full(c)) {
    return MU_COLLECTION_ERR_FULL;
  }
  // shift elements down to leave a hole
  memmove(&(c->data[1]), &(c->data[0]), c->count * sizeof(mu_vect_item_t));
  c->data[0] = item;
  c->count += 1;
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_pop(mu_vect_t *c, mu_vect_item_t *item) {
  if (mu_vect_is_empty(c)) {
    return MU_COLLECTION_ERR_EMPTY;
  }
  *item = c->data[0];
  // shift elements up to close gap
  c->count -= 1;
  memmove(&(c->data[0]), &(c->data[1]), c->count * sizeof(mu_vect_item_t));
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_append(mu_vect_t *c, mu_vect_item_t item) {
  if (mu_vect_is_full(c)) {
    return MU_COLLECTION_ERR_FULL;
  }
  c->data[c->count++] = item;
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_remove(mu_vect_t *c, mu_vect_item_t *item) {
  if (mu_vect_is_empty(c)) {
    return MU_COLLECTION_ERR_EMPTY;
  }
  *item = c->data[--c->count];
  return MU_COLLECTION_ERR_NONE;
}

size_t mu_vect_from_array(mu_vect_t *c, mu_vect_item_t items, size_t n_items) {
  c->count = (n_items > c->capacity) ? c->capacity : n_items;
  memmove(c->data, items, c->count * sizeof(mu_vect_item_t));
  return c->count;
}

size_t mu_vect_to_array(mu_vect_t *c, mu_vect_item_t items, size_t n_items) {
  int n = (n_items > c->count) ? c->count : n_items;
  memmove(items, c->data, n * sizeof(mu_vect_item_t));
  return n;
}

mu_collection_err_t mu_vect_deref(mu_vect_ref_t *r, mu_vect_item_t *item) {
  if (!ref_is_valid(r)) {
    return MU_COLLECTION_ERR_BOUNDS;
  }
  *item = r->c->data[r->index];
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_ref_first(mu_vect_t *c, mu_vect_ref_t *r) {
  r->c = c;
  r->index = 0;
  return validate_ref(r);
}

mu_collection_err_t mu_vect_ref_last(mu_vect_t *c, mu_vect_ref_t *r) {
  r->c = c;
  r->index = c->count - 1;
  return validate_ref(r);
}

mu_collection_err_t mu_vect_ref_next(mu_vect_ref_t *r) {
  r->index += 1;
  return validate_ref(r);
}

mu_collection_err_t mu_vect_ref_prev(mu_vect_ref_t *r) {
  r->index -= 1;
  return validate_ref(r);
}

mu_collection_err_t mu_vect_ref_push(mu_vect_ref_t *r, mu_vect_item_t i) {
  mu_collection_err_t err = ref_push(r, i, r->index);
  if (err == MU_COLLECTION_ERR_NONE) {
    r->index += 1;
  }
  return err;
}

mu_collection_err_t mu_vect_ref_append(mu_vect_ref_t *r, mu_vect_item_t i) {
  return ref_push(r, i, r->index+1);
}

mu_collection_err_t mu_vect_ref_pop(mu_vect_ref_t *r, mu_vect_item_t *i) {
  return ref_pop(r, i);
}

mu_collection_err_t mu_vect_ref_remove(mu_vect_ref_t *r, mu_vect_item_t *i) {
  mu_collection_err_t err = ref_pop(r, i);
  if (err == MU_COLLECTION_ERR_NONE) {
    r->index -= 1;
  }
  return err;
}

void mu_vect_traverse(mu_vect_t *c,  mu_vect_traverse_fn fn, void *target) {
  mu_vect_ref_t r;
  mu_vect_ref_first(c, &r);
  while (ref_is_valid(&r)) {
    if (!fn(&r, target)) break;
    mu_vect_ref_next(&r);
  }
}

// =============================================================================
// private code

static bool ref_is_valid(mu_vect_ref_t *r) {
  return (r->c != NULL) && (r->index < r->c->count);
}

static mu_collection_err_t validate_ref(mu_vect_ref_t *r) {
  if (ref_is_valid(r)) {
    return MU_COLLECTION_ERR_NONE;
  }
  r->c = NULL;  // invalidate future attempts to dereference
  return MU_COLLECTION_ERR_BOUNDS;
}

static mu_collection_err_t ref_push(mu_vect_ref_t *r,
                                    mu_vect_item_t item,
                                    size_t before) {
  if (!ref_is_valid(r)) {
    return MU_COLLECTION_ERR_BOUNDS;
  } else if (mu_vect_is_full(r->c)) {
    return MU_COLLECTION_ERR_FULL;
  }

  mu_vect_t *c = r->c;
  int n_moved = mu_vect_count(c) - before;

  memmove(&(c->data[before+1]),
          &(c->data[before]),
          n_moved * sizeof(mu_vect_item_t));
  c->data[before] = item;
  c->count += 1;
  return MU_COLLECTION_ERR_NONE;
}

static mu_collection_err_t ref_pop(mu_vect_ref_t *r, mu_vect_item_t *item) {
  if (!ref_is_valid(r)) {
    return MU_COLLECTION_ERR_BOUNDS;
  } else if (mu_vect_is_empty(r->c)) {
    return MU_COLLECTION_ERR_EMPTY;
  }

  mu_vect_t *c = r->c;
  int index = r->index;
  int n_moved = mu_vect_count(c) - index - 1;

  *item = c->data[index];
  memmove(&(c->data[index]),
          &(c->data[index+1]),
          n_moved * sizeof(mu_vect_item_t));

  c->count -= 1;
  return MU_COLLECTION_ERR_NONE;
}
