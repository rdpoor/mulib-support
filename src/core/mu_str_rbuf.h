/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

#ifndef _MU_STR_RBUF_H_
#define _MU_STR_RBUF_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Includes

#include <stddef.h>
#include <stdint.h>

// =============================================================================
// Types and definitions

typedef struct {
  const uint8_t * store;   // backing store
  size_t capacity;
} mu_str_rbuf_t;

// =============================================================================
// Declarations

mu_str_rbuf_t *mu_str_rbuf_init(mu_str_rbuf_t *rbuf,
                                const uint8_t *const store,
                                size_t capacity);

mu_str_rbuf_t *mu_str_rbuf_init_from_cstr(mu_str_rbuf_t *rbuf,
                                          const uint8_t *const cstr);

const uint8_t *const mu_str_rbuf_store(const mu_str_rbuf_t *const rbuf);

size_t mu_str_rbuf_capacity(const mu_str_rbuf_t *const rbuf);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_STR_RBUF_H_ */
