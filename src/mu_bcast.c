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

#include "mu_bcast.h"
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static bool channel_is_valid(mu_bcast_channel_t channel);

mu_bcast_err_t subscribe_one(mu_bcast_subscriber_t *subscriber,
                             mu_bcast_channel_t channel,
                             mu_msg_fn function,
                             void *self);

mu_bcast_err_t unsubscribe_one(mu_bcast_subscriber_t *subscriber);

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_bcast_init(mu_bcast_mgr_t *mu_bcast_mgr,
                   mu_bcast_subscriber_t *subscribers,
                   int max_subscribers) {
  mu_bcast_mgr->subscribers = subscribers;
  mu_bcast_mgr->max_subscribers = max_subscribers;
  mu_bcast_reset(mu_bcast_mgr);
}

void mu_bcast_reset(mu_bcast_mgr_t *mu_bcast_mgr) {
  for (int i = 0; i < mu_bcast_mgr->max_subscribers; i++) {
    mu_bcast_subscriber_t *subscriber = &(mu_bcast_mgr->subscribers[i]);
    unsubscribe_one(subscriber);
  }
}

mu_bcast_err_t mu_bcast_subscribe(mu_bcast_mgr_t *mu_bcast_mgr,
                                  mu_bcast_channel_t channel,
                                  mu_msg_fn function,
                                  void *self) {
  mu_bcast_subscriber_t *subscriber;
  int first_available_slot = -1;

  if (!channel_is_valid(channel)) {
    return MU_BCAST_ERR_ILLEGAL_CHANNEL;
  }

  for (int i = 0; i < mu_bcast_mgr->max_subscribers; i++) {
    subscriber = &(mu_bcast_mgr->subscribers[i]);
    if (subscriber->msg.fn == function && subscriber->channel == channel) {
      // Already subscribed on this channel. Update `self` arg.
      return subscribe_one(subscriber, channel, function, self);
    } else if (first_available_slot == -1 && subscriber->msg.fn == NULL) {
      // make a note of the first available slot
      first_available_slot = i;
    }
  }

  // function was not present.  assign to first available slot.
  if (first_available_slot != -1) {
    subscriber = &(mu_bcast_mgr->subscribers[first_available_slot]);
    return subscribe_one(subscriber, channel, function, self);
  }

  return MU_BCAST_ERR_SUBSCRIBERS_EXHAUSTED;
}

mu_bcast_err_t mu_bcast_unsubscribe(mu_bcast_mgr_t *mu_bcast_mgr,
                                    mu_bcast_channel_t channel,
                                    mu_msg_fn function) {
  mu_bcast_subscriber_t *subscriber;

  for (int i = 0; i < mu_bcast_mgr->max_subscribers; i++) {
    subscriber = &(mu_bcast_mgr->subscribers[i]);
    if (subscriber->msg.fn == function && ((MU_BCAST_ALL_CHANNELS == channel) ||
                                           (subscriber->channel == channel))) {
      return unsubscribe_one(subscriber);
    }
  }
  return MU_BCAST_ERR_NOT_FOUND;
}

mu_bcast_err_t mu_bcast_notify(mu_bcast_mgr_t *mu_bcast_mgr,
                               mu_bcast_channel_t channel,
                               void *arg) {
  if (!((channel == MU_BCAST_ALL_CHANNELS) || channel_is_valid(channel))) {
    return MU_BCAST_ERR_ILLEGAL_CHANNEL;
  }

  for (int i = 0; i < mu_bcast_mgr->max_subscribers; i++) {
    mu_bcast_subscriber_t *subscriber = &(mu_bcast_mgr->subscribers[i]);
    if ((subscriber->msg.fn != NULL) && ((MU_BCAST_ALL_CHANNELS == channel) ||
                                         (subscriber->channel == channel))) {
      mu_msg_call(&subscriber->msg, arg);
    }
  }
  return MU_BCAST_ERR_NONE;
}

// =============================================================================
// private code

static bool channel_is_valid(mu_bcast_channel_t channel) {
  return ((channel >= MU_BCAST_CHANNEL_MIN) &&
          (channel <= MU_BCAST_CHANNEL_MAX));
}

mu_bcast_err_t subscribe_one(mu_bcast_subscriber_t *subscriber,
                             mu_bcast_channel_t channel,
                             mu_msg_fn function,
                             void *self) {
  subscriber->channel = channel;
  subscriber->msg.fn = function;
  subscriber->msg.self = self;
  return MU_BCAST_ERR_NONE;
}

mu_bcast_err_t unsubscribe_one(mu_bcast_subscriber_t *subscriber) {
  subscriber->channel = MU_BCAST_CHANNEL_UNASSIGNED;
  subscriber->msg.fn = NULL;
  subscriber->msg.self = NULL;
  return MU_BCAST_ERR_NONE;
}
