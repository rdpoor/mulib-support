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

#include "mu_port_iostream.h"
#include "driver_init.h"

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static void txc_cb(const struct usart_async_descriptor *const descr);
static void rxc_cb(const struct usart_async_descriptor *const descr);
static void err_cb(const struct usart_async_descriptor *const descr);

// =============================================================================
// local storage

// provide a static handle for the ISRs
static mu_iostream_t *s_iostream;

// =============================================================================
// public code

void mu_port_iostream_init(mu_iostream_t *iostream, void *hw) {
  s_iostream = iostream;

  usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, txc_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, rxc_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, err_cb);

	usart_async_enable(&EDBG_COM);
}

mu_iostream_err_t mu_port_iostream_write(mu_iostream_t *iostream,
                                         mu_strbuf_t *sb) {
  struct io_descriptor *io;
	usart_async_get_io_descriptor(&EDBG_COM, &io);
  // TODO: check return value.  if positive, indicates the number of bytes
  // actually written to the channel.  if negative, indicates an error.
  char *src = mu_strbuf_data(sb);
	io_write(io, (const uint8_t * const)src, mu_strbuf_length(sb));

  return MU_IOSTREAM_ERR_NONE;
}

bool mu_port_iostream_write_is_busy(mu_iostream_t *iostream) {
  return usart_async_get_status(&EDBG_COM, NULL) == ERR_BUSY;
  // We might need to check that the tx buffer is empty as well:
  // return (usart_async_get_status(&EDBG_COM, NULL) == ERR_BUSY) &&
  //        !usart_async_is_tx_empty(&EDBG_COM);
}

mu_iostream_err_t mu_port_iostream_read(mu_iostream_t *iostream,
                                        mu_strbuf_t *sb) {
  struct io_descriptor *io;
  usart_async_get_io_descriptor(&EDBG_COM, &io);

  // This is a blatant violation of strbuf abstraction.  Figure out how to
  // provide a function to do this.
  size_t available = sb->capacity - sb->length;
  char *dst = &(sb->data[sb->length]);
  int32_t n_read = io_read(io, (uint8_t * const)dst, available);
  sb->length += n_read;

  return MU_IOSTREAM_ERR_NONE;
}

bool mu_port_iostream_read_is_available(mu_iostream_t *iostream) {
  return usart_async_is_rx_not_empty(&EDBG_COM);
}

// =============================================================================
// private code

// txc_cb is called from interrupt level when the write operation completes.
static void txc_cb(const struct usart_async_descriptor *const descr) {
  if (s_iostream->write_cb) {
    mu_sched_add_from_isr(s_iostream->sched, s_iostream->write_cb);
  }
}

// rxc_cb is called from interrupt level when a character becomes available
static void rxc_cb(const struct usart_async_descriptor *const descr) {
  if (s_iostream->read_cb) {
    mu_sched_add_from_isr(s_iostream->sched, s_iostream->read_cb);
  }
}

static void err_cb(const struct usart_async_descriptor *const descr) {
  // TODO: should we provide a callback?
  asm("nop");
}
