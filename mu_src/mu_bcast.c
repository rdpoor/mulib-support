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
#include "mu_task.h"
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static bool channel_is_valid(mu_bcast_channel_t channel);

static bool channel_matches(mu_bcast_subscriber_t *subscriber,
                            mu_bcast_channel_t channel);

static mu_bcast_err_t subscribe_one(mu_bcast_subscriber_t *subscriber,
                                    mu_bcast_channel_t channel,
                                    mu_task_t *task);

static mu_bcast_err_t unsubscribe_one(mu_bcast_subscriber_t *subscriber);

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
                                  mu_task_t *task) {
  mu_bcast_subscriber_t *subscriber;
  int first_available_slot = -1;

  if (!channel_is_valid(channel)) {
    return MU_BCAST_ERR_ILLEGAL_CHANNEL;
  }

  // See if task is already subscribed, and if not, find the first free slot.
  for (int i = 0; i < mu_bcast_mgr->max_subscribers; i++) {
    subscriber = &(mu_bcast_mgr->subscribers[i]);

    if ((subscriber->channel == channel) && (subscriber->task == task)) {
      // Already subscribed on this channel.
      return MU_BCAST_ERR_NONE;

    } else if (first_available_slot == -1 && subscriber->task == NULL) {
      // make a note of the first available slot
      first_available_slot = i;
    }
  }

  // Endgame: assign target.function to first available slot.
  if (first_available_slot != -1) {
    subscriber = &(mu_bcast_mgr->subscribers[first_available_slot]);
    return subscribe_one(subscriber, channel, task);
  }

  // Sorry, no room left
  return MU_BCAST_ERR_FULL;
}

mu_bcast_err_t mu_bcast_unsubscribe(mu_bcast_mgr_t *mu_bcast_mgr,
                                    mu_bcast_channel_t channel,
                                    mu_task_t *task) {
  mu_bcast_subscriber_t *subscriber;

  for (int i = 0; i < mu_bcast_mgr->max_subscribers; i++) {
    subscriber = &(mu_bcast_mgr->subscribers[i]);
    if ((subscriber->task == task) && channel_matches(subscriber, channel)) {
      return unsubscribe_one(subscriber);
    }
  }
  return MU_BCAST_ERR_NOT_FOUND;
}

mu_bcast_err_t mu_bcast_notify(mu_bcast_mgr_t *mu_bcast_mgr,
                               mu_bcast_channel_t channel,
                               void *arg) {
  if (!channel_is_valid(channel)) {
    return MU_BCAST_ERR_ILLEGAL_CHANNEL;
  }

  for (int i = 0; i < mu_bcast_mgr->max_subscribers; i++) {
    mu_bcast_subscriber_t *subscriber = &(mu_bcast_mgr->subscribers[i]);

    if (channel_matches(subscriber, channel)) {
      // mu_task_call handles the case of a null msg.fn
      mu_task_call(subscriber->task, arg);
    }
  }
  return MU_BCAST_ERR_NONE;
}

// =============================================================================
// private code

static bool channel_is_valid(mu_bcast_channel_t channel) {
  return ((channel >= MU_BCAST_CH_MIN) && (channel <= MU_BCAST_CH_MAX));
}

static bool channel_matches(mu_bcast_subscriber_t *subscriber,
                            mu_bcast_channel_t channel) {
  if (channel == MU_BCAST_CH_ALL) {
    // wildcard doesn't match if the subscriber is unassigned
    return (subscriber->channel != MU_BCAST_CH_UNASSIGNED);
  } else {
    // exact match needed
    return (subscriber->channel == channel);
  }
}

static mu_bcast_err_t subscribe_one(mu_bcast_subscriber_t *subscriber,
                                    mu_bcast_channel_t channel,
                                    mu_task_t *task) {
  subscriber->channel = channel;
  subscriber->task = task;
  return MU_BCAST_ERR_NONE;
}

static mu_bcast_err_t unsubscribe_one(mu_bcast_subscriber_t *subscriber) {
  subscriber->channel = MU_BCAST_CH_UNASSIGNED;
  subscriber->task = NULL;
  return MU_BCAST_ERR_NONE;
}
