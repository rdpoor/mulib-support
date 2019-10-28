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

#ifndef MU_STRBUF_H_
#define MU_STRBUF_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stddef.h>

// =============================================================================
// types and definitions

typedef struct {
  char *data;
  size_t capacity;
  size_t length;
} mu_strbuf_t;

// =============================================================================
// declarations

mu_strbuf_t *mu_strbuf_init(mu_strbuf_t *sb, char *buf, size_t capacity);
mu_strbuf_t *mu_strbuf_reset(mu_strbuf_t *sb);
size_t mu_strbuf_capacity(mu_strbuf_t *sb);
size_t mu_strbuf_length(mu_strbuf_t *sb);
size_t mu_strbuf_available(mu_strbuf_t *sb);
size_t mu_strbuf_append(mu_strbuf_t *sb, const char *s);
size_t mu_strbuf_printf(mu_strbuf_t *sb, const char *fmt, ...);
char *mu_strbuf_data(mu_strbuf_t *sb);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_STRBUF_H_
