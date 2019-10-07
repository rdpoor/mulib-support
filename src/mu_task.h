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

#ifndef mu_task_H_
#define mu_task_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A `mu_task` is essentially a function that can be called later.  It comprises
 * a function pointer (`msg_msg_fn`) and a context (`void *self`).  When called,
 * the function is passed the self argument and a `void *` argument.
 */

// Ths signature of o mu_task function.
typedef void (*mu_task_fn)(void *self, void *arg);

typedef struct {
  mu_task_fn fn;
  void *self;
} mu_task_t;

mu_task_t *mu_task_init(mu_task_t *msg, mu_task_fn fn, void *self);

void mu_task_call(mu_task_t *msg, void *arg);

#ifdef __cplusplus
}
#endif

#endif // #ifndef mu_task_H_