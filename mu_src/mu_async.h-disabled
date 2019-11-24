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

#ifndef MU_ASYNC_H_
#define MU_ASYNC_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_sched.h"
#include "mu_string.h"
#include "mu_task.h"

// =============================================================================
// types and definitions

typedef enum {
  MU_ASYNC_ERR_NONE,
  MU_ASYNC_ERR_UNAVAILABLE,
  MU_ASYNC_ERR_ALREADY_RESERVED,
  MU_ASYNC_ERR_NOT_RESERVED,
} mu_async_err_t;

typedef enum {
  MU_ASYNC_AVAILABLE,
  MU_ASYNC_USER_OWNS,
  MU_ASYNC_ISR_OWNS
} mu_async_owner_t;

typedef struct {
  mu_sched_t *sched;
  void *port_driver;
  mu_task_t *user_read_cb;
  mu_task_t *user_write_cb;
  mu_task_t internal_write_cb;
  mu_string_t buffer_a;
  mu_string_t buffer_b;
  mu_async_owner_t a_owner;
  mu_async_owner_t b_owner;
} mu_async_t;

// =============================================================================
// declarations

/**
 * \brief initialize the async module.
 *
 * The buffer is split into two equal parts to create a pair of mu_string
 * objects.  These are used for double buffered output.
 */
mu_async_t *mu_async_init(mu_async_t *async,
                          mu_sched_t *sched,
                          void *port_driver,
                          char *buf,
                          size_t buf_size);

/**
 * \brief  Reset the internal state of the async processor.
 *
 * This will also stop any actve read or write operations in progress.
 */
mu_async_t *mu_async_reset(mu_async_t *async);

/**
 * \brief set/get a taks to be called when new read data is available.
 */
mu_async_t *mu_async_set_read_cb(mu_async_t *async, mu_task_t *read_cb);
mu_task_t *mu_async_get_read_cb(mu_async_t *async);

/**
 * Read any available data and append to the given string buffer.
 *
 * If the low-level read operation has not yet started, this will start it.
 */
mu_async_err_t mu_async_read(mu_async_t *async, mu_string_t *buf);

/**
 * \brief Set/get a task to be called when a buffer comes available.
 */
mu_async_t *mu_async_set_write_cb(mu_async_t *async, mu_task_t *write_cb);
mu_task_t *mu_async_get_write_cb(mu_async_t *async);

/**
 * \brief Get a buffer for write operations.
 *
 * If a buffer is available, it is returned.  If not, NULL is returned.
 */
mu_async_err_t mu_async_reserve_write_buffer(mu_async_t *async,
                                             mu_string_t **buffer);

/**
 * \brief Post current buffer for writing.
 *
 * After writing data into the string buffer previously acquired by a call to
 * mu_async_get_buffer, you post it for writing here.  You will get an error
 * return if the buffer cannot be posted.
 */
mu_async_err_t mu_async_post_write_buffer(mu_async_t *async);

/**
 * Return true if the hardware is actively writing.
 */
bool mu_async_is_busy(mu_async_t *async);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MU_ASYNC_H_ */
