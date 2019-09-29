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

#include "bcast.h"
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static bool channel_is_valid(bcast_channel_t channel);

bcast_err_t subscribe_one(bcast_subscriber_t *subscriber,
                          bcast_channel_t channel,
                          bcast_function_t function,
                          bcast_arg_t *user_arg);

bcast_err_t unsubscribe_one(bcast_subscriber_t *subscriber);

// =============================================================================
// local storage

// =============================================================================
// public code

void bcast_init(bcast_mgr_t *bcast_mgr,
                bcast_subscriber_t *subscribers,
                int max_subscribers) {
  bcast_mgr->subscribers = subscribers;
  bcast_mgr->max_subscribers = max_subscribers;
  bcast_reset(bcast_mgr);
}

void bcast_reset(bcast_mgr_t *bcast_mgr) {
  for (int i = 0; i < bcast_mgr->max_subscribers; i++) {
    bcast_subscriber_t *subscriber = &(bcast_mgr->subscribers[i]);
    unsubscribe_one(subscriber);
  }
}

bcast_err_t bcast_subscribe(bcast_mgr_t *bcast_mgr,
                            bcast_channel_t channel,
                            bcast_function_t function,
                            bcast_arg_t *user_arg) {
  bcast_subscriber_t *subscriber;
  int first_available_slot = -1;

  if (!channel_is_valid(channel)) return BCAST_ERR_ILLEGAL_CHANNEL;

  for (int i = 0; i < bcast_mgr->max_subscribers; i++) {
    subscriber = &(bcast_mgr->subscribers[i]);
    if (subscriber->function == function && subscriber->channel == channel) {
      // Already subscribed on this channel. Update user arg.
      return subscribe_one(subscriber, channel, function, user_arg);
    } else if (first_available_slot == -1 && subscriber->function == NULL) {
      // make a note of the first available slot
      first_available_slot = i;
    }
  }

  // function was not present.  assign to first available slot.
  if (first_available_slot != -1) {
    subscriber = &(bcast_mgr->subscribers[first_available_slot]);
    return subscribe_one(subscriber, channel, function, user_arg);
  }

  return BCAST_ERR_SUBSCRIBERS_EXHAUSTED;
}

bcast_err_t bcast_unsubscribe(bcast_mgr_t *bcast_mgr,
                              bcast_channel_t channel,
                              bcast_function_t function) {
  bcast_subscriber_t *subscriber;

  // check existing entries.  If function is present, update its channel.
  for (int i = 0; i < bcast_mgr->max_subscribers; i++) {
    subscriber = &(bcast_mgr->subscribers[i]);
    if (subscriber->function == function && subscriber->channel == channel) {
      return unsubscribe_one(subscriber);
    }
  }

  return BCAST_ERR_NOT_FOUND;
}

bcast_err_t bcast_notify(bcast_mgr_t *bcast_mgr,
                         bcast_channel_t channel,
                         bcast_arg_t *arg) {
  if (!channel_is_valid(channel)) return BCAST_ERR_ILLEGAL_CHANNEL;

  for (int i = 0; i < bcast_mgr->max_subscribers; i++) {
    bcast_subscriber_t *subscriber = &(bcast_mgr->subscribers[i]);
    if (subscriber->channel == channel) {
      subscriber->function(channel, arg, subscriber->user_arg);
    }
  }
  return BCAST_ERR_NONE;
}

// =============================================================================
// private code

static bool channel_is_valid(bcast_channel_t channel) {
  return ((channel >= BCAST_CHANNEL_MIN) && (channel <= BCAST_CHANNEL_MAX));
}

bcast_err_t subscribe_one(bcast_subscriber_t *subscriber,
                          bcast_channel_t channel,
                          bcast_function_t function,
                          bcast_arg_t *user_arg) {
  subscriber->channel = channel;
  subscriber->function = function;
  subscriber->user_arg = user_arg;
  return BCAST_ERR_NONE;
}

bcast_err_t unsubscribe_one(bcast_subscriber_t *subscriber) {
  subscriber->channel = BCAST_CHANNEL_UNASSIGNED;
  subscriber->function = NULL;
  subscriber->user_arg = NULL;
  return BCAST_ERR_NONE;
}
