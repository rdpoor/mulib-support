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

#ifndef MU_IOSTREAM_H_
#define MU_IOSTREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_sched.h"
#include "mu_task.h"
#include "mu_strbuf.h"

// =============================================================================
// types and definitions

typedef struct {
  mu_sched_t *sched;
  void *hw;
  mu_task_t *write_cb;
  mu_task_t *read_cb;
} mu_iostream_t;

typedef enum {
  MU_IOSTREAM_ERR_NONE,
} mu_iostream_err_t;

// =============================================================================
// declarations

mu_iostream_t *mu_iostream_init(mu_iostream_t *iostream,
                                mu_sched_t *sched,
                                void *hw);

mu_iostream_err_t mu_iostream_enable(mu_iostream_t *iostream);

mu_iostream_err_t mu_iostream_disable(mu_iostream_t *iostream);

// writing to the stream

/**
 * \brief Write the contents of a string buffer to the iostream.
 */
mu_iostream_err_t mu_iostream_write(mu_iostream_t *iostream, mu_strbuf_t *sb);

/**
 * \brief Return true if the write operation is in progress.
 */
bool mu_iostream_write_is_busy(mu_iostream_t *iostream);

/**
 * \brief Set a callback task to be called whenever the iostream becomes ready
 * for a write operation.
 */
void mu_iostream_set_write_callback(mu_iostream_t *iostream, mu_task_t *task);

/**
 * \brief Get the write callback.
 */
mu_task_t *mu_iostream_get_write_callback(mu_iostream_t *iostream);

// reading from the stream

/**
 * \brief Read bytes from the iostream, appending to ethe string buffer.
 */
mu_iostream_err_t mu_iostream_read(mu_iostream_t *iostream, mu_strbuf_t *sb);

/**
 * \brief Return true if the data is available for reading.
 */
bool mu_iostream_read_is_available(mu_iostream_t *iostream);

/**
 * \brief Set a callback task to be called whenever the iostream has more bytes
 * available for a read operation..
 */
void mu_iostream_set_read_callback(mu_iostream_t *iostream, mu_task_t *task);

/**
 * \brief Get the read callback.
 */
mu_task_t *mu_iostream_get_read_callback(mu_iostream_t *iostream);



#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_IOSTREAM_H_
