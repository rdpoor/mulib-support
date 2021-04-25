/**
 * MIT License
 *
 * Copyright (c) 2020 R. D. Poor <rdpoor@gmail.com>
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

#ifndef _JOINER_WTO_H_
#define _JOINER_WTO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Includes

#include "mu_platform.h"
#include "mulib.h"

// =============================================================================
// Types and definitions

typedef struct {
  mu_task_t task;
  int pending_count;
  mu_task_t timeout_task;
  mu_task_t *on_completion;
} joiner_wto_ctx_t;

// =============================================================================
// Declarations

mu_task_t *joiner_wto_init(joiner_wto_ctx_t *ctx, mu_task_t *on_completion);

mu_task_t *joiner_wto_add_task(joiner_wto_ctx_t *ctx);

void joiner_wto_set_timeout_at(joiner_wto_ctx_t *ctx, mu_time_t at);

#ifdef __cplusplus
}
#endif

#endif // _JOINER_WTO_H_
