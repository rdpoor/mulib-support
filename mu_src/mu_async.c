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

#include "mu_async.h"
#include "mu_sched.h"
#include "mu_string.h"
#include "mu_task.h"

// =============================================================================
// local types and definitions

typedef enum {
  MU_ASYNC_STATE_00,    // A and B are available
  MU_ASYNC_STATE_01,    // A owned by user, B available
  MU_ASYNC_STATE_02,    // A owned by ISR, B available
  MU_ASYNC_STATE_10,    // A available, B owned by user
  MU_ASYNC_STATE_12,    // A owned by ISR, B owned by user
  MU_ASYNC_STATE_20,    // A available, B owned by isr
  MU_ASYNC_STATE_21,    // A owned by user, B owned by ISR
} mu_async_state_t;

// =============================================================================
// local (forward) declarations

/**
 * \brief Function to be called when underlying hw finishes with a buffer.
 */
static void internal_write_cb_fn(void *self, void *arg);

static mu_async_owner_t get_buffer_a_owner(mu_async_t *async);
static void set_buffer_a_owner(mu_async_t *async, mu_async_owner_t owner);
static mu_async_owner_t get_buffer_b_owner(mu_async_t *async);
static void set_buffer_b_owner(mu_async_t *async, mu_async_owner_t owner);

// =============================================================================
// local storage

// =============================================================================
// public code

mu_async_t *mu_async_init(mu_async_t *async,
                          mu_sched_t *sched,
                          void *port_driver,
                          char *buf,
                          size_t buf_size) {
  async->sched = sched;
  async->port_driver = port_driver;
  int n2 = buf_size / 2;
  mu_string_init(&(async->buffer_a), buf, n2);
  mu_string_init(&(async->buffer_b), &buf[n2], n2);
  mu_task_init_immed(&(async->internal_write_cb),
                     internal_write_cb_fn,
                     async,
                     "Async Write CB");
  return mu_async_reset(async);
}

mu_async_t *mu_async_reset(mu_async_t *async) {
  mu_sched_remove(async->sched, &(async->internal_write_cb));
  mu_string_reset(&(async->buffer_a));
  mu_string_reset(&(async->buffer_b));
  set_buffer_a_owner(async, MU_ASYNC_AVAILABLE);
  set_buffer_b_owner(async, MU_ASYNC_AVAILABLE);
  return async;
}

mu_async_t *mu_async_set_write_cb(mu_async_t *async, mu_task_t *write_cb) {
  async->user_write_cb = write_cb;
  return async;
}

mu_task_t *mu_async_get_write_cb(mu_async_t *async) {
  return async->user_write_cb;
}

mu_async_err_t mu_async_reserve_buffer(mu_async_t *async,
                                       mu_string_t **buffer) {
   mu_async_owner_t a_own = get_buffer_a_owner(async);
   mu_async_owner_t b_own = get_buffer_b_owner(async);

  if (a_own == MU_ASYNC_AVAILABLE) {
    set_buffer_a_owner(async, MU_ASYNC_USER_OWNS);
    *buffer = mu_string_reset(&(async->buffer_a));
    return MU_ASYNC_ERR_NONE;

  } else if (b_own == MU_ASYNC_AVAILABLE) {
    set_buffer_b_owner(async, MU_ASYNC_USER_OWNS);
    *buffer = mu_string_reset(&(async->buffer_b));
    return MU_ASYNC_ERR_NONE;

  } else if ((a_own == MU_ASYNC_USER_OWNS) || (b_own == MU_ASYNC_USER_OWNS)) {
    return MU_ASYNC_ERR_ALREADY_RESERVED;

  } else {
    return MU_ASYNC_ERR_UNAVAILABLE;
  }
}

mu_async_err_t mu_async_post_buffer(mu_async_t *async) {
  mu_async_owner_t a_own = get_buffer_a_owner(async);
  mu_async_owner_t b_own = get_buffer_b_owner(async);

  if (a_own == MU_ASYNC_USER_OWNS) {
    set_buffer_a_owner(async, MU_ASYNC_ISR_OWNS);
    // POST BUFFER A TO HARDWARE HERE
    return MU_ASYNC_ERR_NONE;

  } else if (b_own == MU_ASYNC_USER_OWNS) {
    set_buffer_b_owner(async, MU_ASYNC_ISR_OWNS);
    // POST BUFFER B TO HARDWARE HERE
    return MU_ASYNC_ERR_NONE;

  } else {
    return MU_ASYNC_ERR_NOT_RESERVED;
  }
}

// =============================================================================
// local (static) code

static void internal_write_cb_fn(void *self, void *arg) {
  mu_async_t *async = (mu_async_t *)self;

  if (get_buffer_a_owner(async) == MU_ASYNC_ISR_OWNS) {
    set_buffer_a_owner(async, MU_ASYNC_AVAILABLE);
  }
  if (get_buffer_b_owner(async) == MU_ASYNC_ISR_OWNS) {
    set_buffer_b_owner(async, MU_ASYNC_AVAILABLE);
  }
  if (async->user_write_cb) {
    mu_sched_add(async->sched, async->user_write_cb);
  }
}

static mu_async_owner_t get_buffer_a_owner(mu_async_t *async) {
  return async->a_owner;
}

static void set_buffer_a_owner(mu_async_t *async, mu_async_owner_t owner) {
  async->a_owner = owner;
}

static mu_async_owner_t get_buffer_b_owner(mu_async_t *async) {
  return async->b_owner;
}

static void set_buffer_b_owner(mu_async_t *async, mu_async_owner_t owner) {
  async->b_owner = owner;
}
