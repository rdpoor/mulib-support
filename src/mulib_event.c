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

#include "mulib_event.h"
#include "mulib_task.h"
#include "../port/port_time.h"

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static mulib_event_t *init_event(mulib_event_t *event,
                                 bool is_immediate,
                                 port_time_t time,
                                 mulib_task_fn fn,
                                 void *arg);

// =============================================================================
// local storage

// =============================================================================
// public code

mulib_event_t *mulib_event_init_immediate(mulib_event_t *event,
                                          mulib_task_fn fn,
                                          void *u_arg) {
  return init_event(event, true, 0, fn, u_arg);
}

mulib_event_t *mulib_event_init_timed(mulib_event_t *event,
                                      port_time_t time,
                                      mulib_task_fn fn,
                                      void *u_arg) {
  return init_event(event, false, time, fn, u_arg);
}

bool mulib_event_is_immediate(mulib_event_t *event) {
  return event->is_immediate;
}

port_time_t mulib_event_time(mulib_event_t *event){
  return event->time;
}

mulib_task_t *mulib_event_task(mulib_event_t *event) {
  return &event->task;
}

bool mulib_event_is_runnable(mulib_event_t *event) {
  if (mulib_event_is_immediate(event)) {
    return true;
  } else if (!port_time_is_before(port_time_now(), mulib_event_time(event))) {
    return true;
  } else {
    return false;
  }
}

void mulib_event_run(mulib_event_t *event, void *d_arg) {
  mulib_task_run(mulib_event_task(event), d_arg);
}

// =============================================================================
// private functions

static mulib_event_t *init_event(mulib_event_t *event,
                                 bool is_immediate,
                                 port_time_t time,
                                 mulib_task_fn fn,
                                 void *u_arg) {
  event->is_immediate = is_immediate;
  event->time = time;
  event->task.fn = fn;
  event->task.u_arg = u_arg;
  return event;
}
