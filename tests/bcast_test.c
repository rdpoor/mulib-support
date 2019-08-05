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

#include "../src/bcast.h"
#include "test_utilities.h"
#include <stdint.h>

#define N_SUBSCRIBERS 5

static bcast_subscriber_t s_subscribers[N_SUBSCRIBERS];

static int s_a1_notified, s_a2_notified, s_a3_notified;

const uint16_t CHAN_A = 1;
const uint16_t CHAN_B = 2;

const uintptr_t BCAST_ARG = 44;
const uintptr_t ARG_A1 = 55;
const uintptr_t ARG_A2 = 66;

static void action_a1(bcast_channel_t chan, void *arg, void *user_arg) {
    s_a1_notified++;
    UNIT_TEST_ASSERT((uint32_t)arg == BCAST_ARG);
    UNIT_TEST_ASSERT((uint32_t)user_arg == ARG_A1);
}

static void action_a2(bcast_channel_t chan, void *arg, void *user_arg) {
    s_a2_notified++;
    UNIT_TEST_ASSERT((uint32_t)arg == BCAST_ARG);
    UNIT_TEST_ASSERT((uint32_t)user_arg == ARG_A2);
}

static void action_a3(bcast_channel_t chan, void *arg, void *user_arg) {
    s_a3_notified++;
}

void bcast_test() {
    bcast_mgr_t bcast_mgr;

    s_a1_notified = 0;
    s_a2_notified = 0;
    s_a3_notified = 0;

    bcast_init(&bcast_mgr, s_subscribers, N_SUBSCRIBERS);
    UNIT_TEST_ASSERT(bcast_subscribe(&bcast_mgr, BCAST_CHANNEL_MIN-1, action_a1, (void *)0) == BCAST_ERR_ILLEGAL_CHANNEL);

    // action_a1 and action_a2 subscribed to CHAN_A
    // action_a2 and action_a3 subscribed to CHAN_B
    UNIT_TEST_ASSERT(bcast_subscribe(&bcast_mgr, CHAN_A, action_a1, (void *)ARG_A1) == BCAST_ERR_NONE);
    UNIT_TEST_ASSERT(bcast_subscribe(&bcast_mgr, CHAN_A, action_a2, (void *)ARG_A2) == BCAST_ERR_NONE);
    UNIT_TEST_ASSERT(bcast_subscribe(&bcast_mgr, CHAN_B, action_a2, (void *)ARG_A2) == BCAST_ERR_NONE);
    UNIT_TEST_ASSERT(bcast_subscribe(&bcast_mgr, CHAN_B, action_a3, (void *)0) == BCAST_ERR_NONE);

    // notify with a legal channel
    UNIT_TEST_ASSERT(bcast_notify(&bcast_mgr, CHAN_A, (void *)BCAST_ARG) == BCAST_ERR_NONE);
    UNIT_TEST_ASSERT(s_a1_notified == 1);
    UNIT_TEST_ASSERT(s_a2_notified == 1);
    UNIT_TEST_ASSERT(s_a3_notified == 0);

    // notify with a legal channel
    UNIT_TEST_ASSERT(bcast_notify(&bcast_mgr, CHAN_B, (void *)BCAST_ARG) == BCAST_ERR_NONE);
    UNIT_TEST_ASSERT(s_a1_notified == 1);
    UNIT_TEST_ASSERT(s_a2_notified == 2);
    UNIT_TEST_ASSERT(s_a3_notified == 1);

    // subscribe when already subscribed
    UNIT_TEST_ASSERT(bcast_subscribe(&bcast_mgr, CHAN_A, action_a1, (void *)ARG_A1) == BCAST_ERR_NONE);
    // action_a1 should receive only one (additional) notification
    UNIT_TEST_ASSERT(bcast_notify(&bcast_mgr, CHAN_A, (void *)BCAST_ARG) == BCAST_ERR_NONE);
    UNIT_TEST_ASSERT(s_a1_notified == 2);
    UNIT_TEST_ASSERT(s_a2_notified == 3);
    UNIT_TEST_ASSERT(s_a3_notified == 1);

    // unsubscribe action_a2 from CHAN_A (only)
    UNIT_TEST_ASSERT(bcast_unsubscribe(&bcast_mgr, CHAN_A, action_a2) == BCAST_ERR_NONE);
    // action_a2 should not receive the notification..
    UNIT_TEST_ASSERT(bcast_notify(&bcast_mgr, CHAN_A, (void *)BCAST_ARG) == BCAST_ERR_NONE);
    UNIT_TEST_ASSERT(s_a1_notified == 3);
    UNIT_TEST_ASSERT(s_a2_notified == 3);
    UNIT_TEST_ASSERT(s_a3_notified == 1);

    // notify with an broadcast all channel

    // unsubscribe when not subscribed
    UNIT_TEST_ASSERT(bcast_unsubscribe(&bcast_mgr, CHAN_A, action_a3) == BCAST_ERR_NOT_FOUND);
}
