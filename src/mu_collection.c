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

void mu_vect_init(mu_vect_t *c, mu_vect_item_t items, size_t n_items) {

}

void mu_vect_reset(mu_vect_t *c) {

}

bool mu_vect_is_empty(mu_vect_t *c) {
  return true;
}

bool mu_vect_is_full(mu_vect_t *c) {
  return true;
}

mu_collection_err_t mu_vect_first(mu_vect_t *c, mu_vect_item_t *item) {
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_last(mu_vect_t *c, mu_vect_item_t *item) {
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_pop(mu_vect_t *c, mu_vect_item_t *item) {
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_push(mu_vect_t *c, mu_vect_item_t item) {
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_append(mu_vect_t *c, mu_vect_item_t item) {
  return MU_COLLECTION_ERR_NONE;
}

mu_collection_err_t mu_vect_remove(mu_vect_t *c, mu_vect_item_t *item) {
  return MU_COLLECTION_ERR_NONE;
}

size_t mu_vect_capacity(mu_vect_t *c) {
  return 0;
}

size_t mu_vect_count(mu_vect_t *c) {
  return 0;
}

size_t mu_vect_from_array(mu_vect_t *c, mu_vect_item_t items, size_t n_items) {
  return 0;
}

size_t mu_vect_to_array(mu_vect_t *c, mu_vect_item_t items, size_t n_items) {
  return 0;
}
