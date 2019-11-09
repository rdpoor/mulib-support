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

// port specific
#include "driver_init.h"

// =============================================================================
// local types and definitions

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

// port specific forward declarations
static void port_async_init(mu_async_t *async, void *hw);
int port_async_read(mu_async_t *async, char *dst, int n);
int port_async_write(mu_async_t *async, const char *src, int n);
static void rxc_cb(const struct usart_async_descriptor *const descr);
static void txc_cb(const struct usart_async_descriptor *const descr);
static void err_cb(const struct usart_async_descriptor *const descr);

// =============================================================================
// local storage

// port specific - the ISR routines require a handle to get back to the async
static mu_async_t *s_async;

// =============================================================================
// public code

mu_async_t *mu_async_init(mu_async_t *async,
                          mu_sched_t *sched,
                          void *port_driver,
                          char *buf,
                          size_t buf_size) {
  int n2 = buf_size / 2;

  async->sched = sched;
  async->port_driver = port_driver;
  async->user_read_cb = NULL;
  async->user_write_cb = NULL;
  mu_string_init(&(async->buffer_a), buf, n2);
  mu_string_init(&(async->buffer_b), &buf[n2], n2);
  mu_task_init_immed(&(async->internal_write_cb),
                     internal_write_cb_fn,
                     async,
                     "Async Write CB");

  port_async_init(async, port_driver);
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

mu_async_t *mu_async_set_read_cb(mu_async_t *async, mu_task_t *read_cb) {
  async->user_read_cb = read_cb;
  return async;
}

mu_task_t *mu_async_get_read_cb(mu_async_t *async) {
  return async->user_read_cb;
}

mu_async_err_t mu_async_read(mu_async_t *async, mu_string_t *s) {
  int n_read = port_async_read(
      async, &(mu_string_cstring(s)[mu_string_end(s)]), mu_string_available(s));
  if (n_read > 0) {
    s->end += n_read;
    mu_string_cstring(s)[mu_string_end(s)] = '\0';
  }
  return MU_ASYNC_ERR_NONE; // could be better
}

mu_async_t *mu_async_set_write_cb(mu_async_t *async, mu_task_t *write_cb) {
  async->user_write_cb = write_cb;
  return async;
}

mu_task_t *mu_async_get_write_cb(mu_async_t *async) {
  return async->user_write_cb;
}

mu_async_err_t mu_async_reserve_write_buffer(mu_async_t *async,
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

mu_async_err_t mu_async_post_write_buffer(mu_async_t *async) {
  mu_async_owner_t a_own = get_buffer_a_owner(async);
  mu_async_owner_t b_own = get_buffer_b_owner(async);

  if (a_own == MU_ASYNC_USER_OWNS) {
    set_buffer_a_owner(async, MU_ASYNC_ISR_OWNS);
    // Actually write the data to the underlying o driver
    // TODO: report error return
    port_async_write(async,
                     mu_string_cstring(&async->buffer_a),
                     mu_string_length(&async->buffer_a));
    return MU_ASYNC_ERR_NONE;

  } else if (b_own == MU_ASYNC_USER_OWNS) {
    set_buffer_b_owner(async, MU_ASYNC_ISR_OWNS);
    // Actually write the data to the underlying o driver
    // TODO: report error return
    port_async_write(async,
                     mu_string_cstring(&async->buffer_b),
                     mu_string_length(&async->buffer_b));
    return MU_ASYNC_ERR_NONE;

  } else {
    return MU_ASYNC_ERR_NOT_RESERVED;
  }
}

bool mu_async_is_busy(mu_async_t *async) {
  // Return true if the underlying driver is still processing the buffer.
  return (get_buffer_a_owner(async) == MU_ASYNC_ISR_OWNS) ||
         (get_buffer_b_owner(async) == MU_ASYNC_ISR_OWNS);
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

// =============================================================================
// port specific code -- to be refactored later

static void port_async_init(mu_async_t *async, void *hw) {
  s_async = async; // needed for ISR level callbacks

  usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, rxc_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, txc_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, err_cb);

  usart_async_enable(&EDBG_COM);
}

// return as many characters as are available.  the rxc_cb will be called
// when there are more
int port_async_read(mu_async_t *async, char *dst, int n) {
  struct io_descriptor *io;
  usart_async_get_io_descriptor(&EDBG_COM, &io);

  return io_read(io, (uint8_t *const)dst, n);
}

// Initiate a write operation.  the txc_cb will be called on completion.
int port_async_write(mu_async_t *async, const char *src, int n) {
  struct io_descriptor *io;
  usart_async_get_io_descriptor(&EDBG_COM, &io);
  return io_write(io, (const uint8_t *const)src, n);
}

// rxc_cb is called from interrupt level when a character becomes available
static void rxc_cb(const struct usart_async_descriptor *const descr) {
  if (s_async->user_read_cb) {
    mu_sched_add_from_isr(s_async->sched, s_async->user_read_cb);
  }
}

// txc_cb is called from interrupt level when the write operation completes.
static void txc_cb(const struct usart_async_descriptor *const descr) {
  if (&s_async->internal_write_cb) {
    mu_sched_add_from_isr(s_async->sched, &s_async->internal_write_cb);
  }
}

static void err_cb(const struct usart_async_descriptor *const descr) {
  // TODO: should we provide a callback?
  asm("nop");
}
