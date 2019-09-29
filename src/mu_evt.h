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

#ifndef MULIB_EVENT_H
#define MULIB_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mu_msg.h"
#include "mu_time.h"
#include <stdbool.h>

typedef struct {
  bool is_immediate; // true if this event is scheduled for "now".
  mu_time_t time;    // time of this event (ignored if is_immediate)
  mu_msg_t msg;      // function to call
} mu_evt_t;

mu_evt_t *mu_evt_init_immediate(mu_evt_t *evt, mu_msg_fn fn, void *self);

mu_evt_t *mu_evt_init_timed(mu_evt_t *evt,
                            mu_time_t time,
                            mu_msg_fn fn,
                            void *self);

bool mu_evt_is_immediate(mu_evt_t *evt);

mu_time_t mu_evt_time(mu_evt_t *evt);

mu_msg_t *mu_evt_msg(mu_evt_t *evt);

bool mu_evt_has_arrived(mu_evt_t *evt, mu_time_t now);

void mu_evt_call(mu_evt_t *evt, void *arg);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_EVENT_H
