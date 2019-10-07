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

#ifndef MU_BCAST_H_
#define MU_BCAST_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_task.h"
#include <stdint.h>

// =============================================================================
// types and definitions

typedef enum {
  MU_BCAST_ERR_NONE = 0,
  MU_BCAST_ERR_EVENTS_EXHAUSTED,
  MU_BCAST_ERR_SUBSCRIBERS_EXHAUSTED,
  MU_BCAST_ERR_ILLEGAL_CHANNEL,
  MU_BCAST_ERR_NOT_FOUND,
} mu_bcast_err_t;

typedef uint16_t mu_bcast_channel_t;

#define MU_BCAST_CHANNEL_UNASSIGNED 0
#define MU_BCAST_ALL_CHANNELS 0
#define MU_BCAST_CHANNEL_MIN 1
#define MU_BCAST_CHANNEL_MAX UINT16_MAX

typedef struct _subscriber {
  mu_bcast_channel_t channel;
  mu_task_t msg;
} mu_bcast_subscriber_t;

typedef struct _manager {
  mu_bcast_subscriber_t *subscribers;
  int max_subscribers;
} mu_bcast_mgr_t;

// =============================================================================
// declarations

/**
 * @brief: Initialize the exch system.
 */
void mu_bcast_init(mu_bcast_mgr_t *mu_bcast_mgr,
                   mu_bcast_subscriber_t *subscribers,
                   int max_subscribers);

/**
 * @brief: Clear all subscribers
 */
void mu_bcast_reset(mu_bcast_mgr_t *mu_bcast_mgr);

/**
 * @brief: subscribe to notifications on the specified channel.
 *
 */
mu_bcast_err_t mu_bcast_subscribe(mu_bcast_mgr_t *mu_bcast_mgr,
                                  mu_bcast_channel_t channel,
                                  mu_task_fn function,
                                  void *target);

/**
 * @brief: stop receiving notifications
 *
 * If function is NULL, all subscribers matching target are unsubscribed.
 * If target is NULL, all subscribers matching function are unsubscribed.
 * if channel == MU_BCAST_ALL_CHANNELS, matching subscribers are removed from
 * all chanels.
 */
mu_bcast_err_t mu_bcast_unsubscribe(mu_bcast_mgr_t *mu_bcast_mgr,
                                    mu_bcast_channel_t channel,
                                    mu_task_fn function,
                                    void *target);

/**
 * @brief: notify every subscriber on the given channel.
 */
mu_bcast_err_t mu_bcast_notify(mu_bcast_mgr_t *mu_bcast_mgr,
                               mu_bcast_channel_t channel,
                               void *arg);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MU_BCAST_H_
