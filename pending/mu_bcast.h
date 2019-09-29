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

#ifndef MULIB_BCAST_H_
#define MULIB_BCAST_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>

// =============================================================================
// types and definitions

typedef enum {
  BCAST_ERR_NONE = 0,
  BCAST_ERR_EVENTS_EXHAUSTED,
  BCAST_ERR_SUBSCRIBERS_EXHAUSTED,
  BCAST_ERR_ILLEGAL_CHANNEL,
  BCAST_ERR_NOT_FOUND,
} bcast_err_t;

typedef uint16_t bcast_channel_t;

#define BCAST_CHANNEL_UNASSIGNED 0
#define BCAST_CHANNEL_MIN 1
#define BCAST_CHANNEL_MAX UINT16_MAX

/**
 * &brief: an bcast_arg_t is a pointer-sized reference.
 */
typedef void bcast_arg_t;

/**
 * @brief: prototype for exch callback functions.
 */
typedef void (*bcast_function_t)(bcast_channel_t channel,
                                bcast_arg_t *arg,
                                bcast_arg_t *user_arg);

typedef struct _subscriber {
  bcast_channel_t channel;
  bcast_function_t function;
  bcast_arg_t *user_arg;
} bcast_subscriber_t;

typedef struct _manager {
  bcast_subscriber_t *subscribers;
  int max_subscribers;
} bcast_mgr_t;

// =============================================================================
// declarations

/**
 * @brief: Initialize the exch system.
 */
void bcast_init(bcast_mgr_t *bcast_mgr,
                bcast_subscriber_t *subscribers,
                int max_subscribers);

/**
 * @brief: Clear all subscribers
 */
void bcast_reset(bcast_mgr_t *bcast_mgr);

/**
 * @brief: subscribe to notifications on the specified channel.
 */
bcast_err_t bcast_subscribe(bcast_mgr_t *bcast_mgr,
                            bcast_channel_t channel,
                            bcast_function_t function,
                            bcast_arg_t *user_arg);

/**
 * @brief: stop receiving notifications
 */
bcast_err_t bcast_unsubscribe(bcast_mgr_t *bcast_mgr,
                              bcast_channel_t channel,
                              bcast_function_t function);

/**
 * @brief: notify every subscriber on the given channel.
 */
bcast_err_t bcast_notify(bcast_mgr_t *bcast_mgr,
                         bcast_channel_t channel,
                         bcast_arg_t *arg);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_BCAST_H_
