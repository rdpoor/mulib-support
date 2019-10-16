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
#include "mu_task.h"
#include "mu_time.h"
#include "stdio.h"

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static mu_evt_t *init_event(mu_evt_t *evt,
                            bool is_immediate,
                            port_time_t time,
                            mu_task_fn fn,
                            void *self);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_evt_t *mu_evt_init_immed(mu_evt_t *evt,
                            mu_task_fn fn,
                            void *self) {
  return init_event(evt, true, 0, fn, self);
}

mu_evt_t *mu_evt_init_at(mu_evt_t *evt,
                         port_time_t time,
                         mu_task_fn fn,
                         void *self) {
  return init_event(evt, false, time, fn, self);
}

bool mu_evt_is_immediate(mu_evt_t *evt) { return evt->is_immediate; }

port_time_t mu_evt_time(mu_evt_t *evt) { return evt->time; }

mu_task_t *mu_evt_task(mu_evt_t *evt) { return &evt->task; }

bool mu_evt_is_after(mu_evt_t *e1, mu_evt_t *e2) {
  if (mu_evt_is_immediate(e1)) {
    return false;
  } else if (mu_evt_is_immediate(e2)) {
    return true;
  } else {
    return port_time_is_after(mu_evt_time(e1), mu_evt_time(e2));
  }
}

bool mu_evt_is_runnable(mu_evt_t *evt, port_time_t at) {
  if (mu_evt_is_immediate(evt)) {
    return true;
  } else if (!port_time_is_before(at, mu_evt_time(evt))) {
    return true;
  } else {
    return false;
  }
}

void mu_evt_call(mu_evt_t *evt, void *arg) {
  mu_task_call(mu_evt_task(evt), arg);
}

// =============================================================================
// private functions

static mu_evt_t *init_event(mu_evt_t *evt,
                            bool is_immediate,
                            port_time_t time,
                            mu_task_fn fn,
                            void *self) {
  evt->next = (mu_evt_t *)NULL;
  evt->is_immediate = is_immediate;
  evt->time = time;
  evt->task.fn = fn;
  evt->task.self = self;
  return evt;
}
