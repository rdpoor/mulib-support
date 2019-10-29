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

#include "mu_iostream.h"
#include "mu_port_iostream.h"
#include "mu_sched.h"
#include "mu_task.h"
#include <stddef.h>

// =============================================================================
// types and definitions

// =============================================================================
// declarations

mu_iostream_t *mu_iostream_init(mu_iostream_t *iostream,
                                mu_sched_t *sched,
                                void *hw) {
  iostream->sched = sched;
  iostream->hw = hw;
  iostream->write_cb = NULL;
  iostream->read_cb = NULL;

  mu_port_iostream_init(iostream, hw);
  return iostream;
}

mu_iostream_err_t mu_iostream_enable(mu_iostream_t *iostream) {
  return MU_IOSTREAM_ERR_NONE; // TBD
}

mu_iostream_err_t mu_iostream_disable(mu_iostream_t *iostream) {
  return MU_IOSTREAM_ERR_NONE; // TBD
}

// writing to the stream

int mu_iostream_write(mu_iostream_t *iostream, const char *src, int n) {
  return mu_port_iostream_write(iostream, src, n);
}

/**
 * \brief Return true if the write operation is in progress.
 */
bool mu_iostream_write_is_busy(mu_iostream_t *iostream) {
  return mu_port_iostream_write_is_busy(iostream);
}

/**
 * \brief Set a callback task to be called whenever the iostream becomes ready
 * for a write operation.
 */
void mu_iostream_set_write_callback(mu_iostream_t *iostream, mu_task_t *task) {
  iostream->write_cb = task;
}

/**
 * \brief Get the write callback.
 */
mu_task_t *mu_iostream_get_write_callback(mu_iostream_t *iostream) {
  return iostream->write_cb;
}

// reading from the stream

/**
 * \brief Read bytes from the iostream, appending to ethe string buffer.
 */
int mu_iostream_read(mu_iostream_t *iostream, char *dst, int n) {
  return mu_port_iostream_read(iostream, dst, n);
}

/**
 * \brief Return true if the data is available for reading.
 */
bool mu_iostream_read_is_available(mu_iostream_t *iostream) {
  return mu_port_iostream_read_is_available(iostream);
}

/**
 * \brief Set a callback task to be called whenever the iostream has more bytes
 * available for a read operation..
 */
void mu_iostream_set_read_callback(mu_iostream_t *iostream, mu_task_t *task) {
  iostream->read_cb = task;
}

/**
 * \brief Get the read callback.
 */
mu_task_t *mu_iostream_get_read_callback(mu_iostream_t *iostream) {
  return iostream->read_cb;
}
