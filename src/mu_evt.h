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

#ifndef MU_EVT_H_
#define MU_EVT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mu_task.h"
#include "mu_time.h"
#include <stdbool.h>

typedef struct _mu_evt_t {
  struct _mu_evt_t *next;  // link to next older event
  mu_task_t task;          // function to call
  mu_time_t time;          // time of this event (ignored if is_immediate)
  bool is_immediate;       // true if this event is scheduled for "now".
} mu_evt_t;

mu_evt_t *mu_evt_init_immed(mu_evt_t *evt,
                            mu_task_fn fn,
                            void *self,
                            const char *name);

mu_evt_t *mu_evt_init_at(mu_evt_t *evt,
                         mu_time_t time,
                         mu_task_fn fn,
                         void *self,
                         const char *name);

bool mu_evt_is_immediate(mu_evt_t *evt);

mu_time_t mu_evt_time(mu_evt_t *evt);

mu_task_t *mu_evt_task(mu_evt_t *evt);

bool mu_evt_is_runnable(mu_evt_t *evt, mu_time_t at);

// Return true if e1 occurs after e2
bool mu_evt_is_after(mu_evt_t *e1, mu_evt_t *e2);

void mu_evt_call(mu_evt_t *evt, void *arg);

// Set the event's time to t
mu_evt_t *mu_evt_set_time(mu_evt_t *evt, mu_time_t t);

// Offset the event's time by dt
mu_evt_t *mu_evt_offset_time(mu_evt_t *evt, mu_time_dt dt);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_EVT_H_
