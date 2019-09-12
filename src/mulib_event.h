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

#include <stdbool.h>
#include "mulib_task.h"
#include "../port/port_time.h"

typedef struct {
  bool is_immediate;        // true if this event is scheduled asap.
  port_time_t time;          // time of this event (ignored if is_immediate)
  mulib_task_t task;        // function to call
} mulib_event_t;

mulib_event_t *mulib_event_init_immediate(mulib_event_t *event,
                                          mulib_task_fn fn,
                                          void *arg);

mulib_event_t *mulib_event_init_timed(mulib_event_t *event,
                                      port_time_t time,
                                      mulib_task_fn fn,
                                      void *arg);

bool mulib_event_is_immediate(mulib_event_t *event);

port_time_t mulib_event_time(mulib_event_t *event);

mulib_task_t *mulib_event_task(mulib_event_t *event);

bool mulib_event_is_runnable(mulib_event_t *event);

void mulib_event_run(mulib_event_t *event, void *d_arg);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_EVENT_H
