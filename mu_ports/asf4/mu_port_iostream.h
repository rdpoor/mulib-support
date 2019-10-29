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

#ifndef MU_PORT_IOSTREAM_H_
#define MU_PORT_IOSTREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes
#include <stdint.h>
#include <stdbool.h>
#include "mu_iostream.h"

// =============================================================================
// types and definitions

// =============================================================================
// declarations

void mu_port_iostream_init(mu_iostream_t *iostream, void *hw);

int mu_port_iostream_write(mu_iostream_t *iostream, const char *src, int n);

bool mu_port_iostream_write_is_busy(mu_iostream_t *iostream);

int mu_port_iostream_read(mu_iostream_t *iostream, char *dst, int n);

bool mu_port_iostream_read_is_available(mu_iostream_t *iostream);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_PORT_IOSTREAM_H_
