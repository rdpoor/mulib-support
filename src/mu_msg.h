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

#ifndef MU_MSG_H_
#define MU_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A mu_msg comprises a function (`msg_msg_fn`) and a context (void *self).
 * When called, the function is passed the self argument and a void * argument,
 * and is expected to return a pointer-sized object (frequently the self arg
 * on success and NULL on failure).
 */
typedef void *(*mu_msg_fn)(void *self, void *arg);

typedef struct {
  mu_msg_fn fn;
  void *self;
} mu_msg_t;

mu_msg_t *mu_msg_init(mu_msg_t *msg, mu_msg_fn fn, void *self);

void mu_msg_call(mu_msg_t *msg, void *arg);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_MSG_H_
