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

// =============================================================================
// includes

#include "mu_evt.h"
#include "mu_msg.h"
#include "mu_time.h"

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static mu_evt_t *init_event(mu_evt_t *evt,
                            bool is_immediate,
                            mu_time_t time,
                            mu_msg_fn fn,
                            void *self);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_evt_t *mu_evt_init_immediate(mu_evt_t *evt, mu_msg_fn fn, void *self) {
  return init_event(evt, true, 0, fn, self);
}

mu_evt_t *mu_evt_init_timed(mu_evt_t *evt,
                            mu_time_t time,
                            mu_msg_fn fn,
                            void *self) {
  return init_event(evt, false, time, fn, self);
}

bool mu_evt_is_immediate(mu_evt_t *evt) { return evt->is_immediate; }

mu_time_t mu_evt_time(mu_evt_t *evt) { return evt->time; }

mu_msg_t *mu_evt_msg(mu_evt_t *evt) { return &evt->msg; }

bool mu_evt_has_arrived(mu_evt_t *evt, mu_time_t now) {
  if (mu_evt_is_immediate(evt)) {
    return true;
  } else if (!port_time_is_before(now, mu_evt_time(evt))) {
    return true;
  } else {
    return false;
  }
}

void mu_evt_call(mu_evt_t *evt, void *arg) {
  mu_msg_call(mu_evt_msg(evt), arg);
}

// =============================================================================
// private functions

static mu_evt_t *init_event(mu_evt_t *evt,
                            bool is_immediate,
                            mu_time_t time,
                            mu_msg_fn fn,
                            void *self) {
  evt->is_immediate = is_immediate;
  evt->time = time;
  evt->msg.fn = fn;
  evt->msg.self = self;
  return evt;
}
