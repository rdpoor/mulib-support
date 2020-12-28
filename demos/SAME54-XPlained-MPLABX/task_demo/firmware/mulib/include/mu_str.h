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

#ifndef _MU_STR_H_
#define _MU_STR_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// types and definitions

typedef char mu_str_data_t;

typedef enum {
  MU_STR_ERR_NONE,
  MU_STR_ERR_INDEX
} mu_str_err_t;

typedef struct {
  mu_str_data_t *data;  // backing store
  size_t capacity;      // length of backing store
} mu_str_t;


// =============================================================================
// declarations

mu_str_t *mu_str_init(mu_str_t *s, mu_str_data_t *data, size_t capacity);

mu_str_t *mu_str_init_from_cstr(mu_str_t *s, const char *cstr);

mu_str_t *mu_str_to_cstr(mu_str_t *s, char *cstr, size_t cstr_length);

size_t mu_str_capacity(mu_str_t *s);

mu_str_data_t *mu_str_data(mu_str_t *s);

mu_str_err_t mu_str_ref(mu_str_t *s, size_t index, mu_str_data_t **p);

mu_str_err_t mu_str_get(mu_str_t *s, size_t index, mu_str_data_t *d);

mu_str_err_t mu_str_put(mu_str_t *s, size_t index, mu_str_data_t d);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_STR_H_ */
