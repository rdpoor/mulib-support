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

#include "../src/mu_bcast.h"
#include "test_utilities.h"
#include <stdint.h>
#include <stddef.h>

#define N_SUBSCRIBERS 5

typedef struct {
  int call_count;
} test_obj_t;

static mu_bcast_subscriber_t s_subscribers[N_SUBSCRIBERS];

const uint16_t CHAN_A = 1;
const uint16_t CHAN_B = 2;

const uintptr_t BCAST_ARG = 44;
const uintptr_t ARG_A1 = 55;
const uintptr_t ARG_A2 = 66;

static test_obj_t s_obj1;
static test_obj_t s_obj2;
static int s_a3_called;

static void action_a1(void *self, void *arg) {
  test_obj_t *obj = (test_obj_t *)self;
  obj->call_count += 1;
  UTEST_ASSERT((uintptr_t)arg == BCAST_ARG);
}

static void action_a2(void *self, void *arg) {
  test_obj_t *obj = (test_obj_t *)self;
  obj->call_count += 1;
  UTEST_ASSERT((uintptr_t)arg == BCAST_ARG);
}

static void action_a3(void *self, void *arg) {
  s_a3_called += 1;
  UTEST_ASSERT(self == NULL);
  UTEST_ASSERT((uintptr_t)arg == BCAST_ARG);
}

void mu_bcast_test() {
  mu_bcast_mgr_t mu_bcast_mgr;

  s_obj1.call_count = 0;
  s_obj2.call_count = 0;
  s_a3_called = 0;

  mu_bcast_init(&mu_bcast_mgr, s_subscribers, N_SUBSCRIBERS);
  UTEST_ASSERT(mu_bcast_subscribe(&mu_bcast_mgr,
                                  MU_BCAST_CHANNEL_MIN - 1,
                                  action_a1,
                                  &s_obj1) == MU_BCAST_ERR_ILLEGAL_CHANNEL);

  // action_a1 and action_a2 subscribed to CHAN_A
  // action_a2 and action_a3 subscribed to CHAN_B
  UTEST_ASSERT(
      mu_bcast_subscribe(&mu_bcast_mgr, CHAN_A, action_a1, &s_obj1) ==
      MU_BCAST_ERR_NONE);
  UTEST_ASSERT(
      mu_bcast_subscribe(&mu_bcast_mgr, CHAN_A, action_a2, &s_obj2) ==
      MU_BCAST_ERR_NONE);
  UTEST_ASSERT(
      mu_bcast_subscribe(&mu_bcast_mgr, CHAN_B, action_a2, &s_obj2) ==
      MU_BCAST_ERR_NONE);
  UTEST_ASSERT(
      mu_bcast_subscribe(&mu_bcast_mgr, CHAN_B, action_a3, NULL) ==
      MU_BCAST_ERR_NONE);

  // notify with a legal channel
  UTEST_ASSERT(mu_bcast_notify(&mu_bcast_mgr, CHAN_A, (void *)BCAST_ARG) ==
               MU_BCAST_ERR_NONE);
  UTEST_ASSERT(s_obj1.call_count == 1);
  UTEST_ASSERT(s_obj2.call_count == 1);
  UTEST_ASSERT(s_a3_called == 0);

  // notify with a legal channel
  UTEST_ASSERT(mu_bcast_notify(&mu_bcast_mgr, CHAN_B, (void *)BCAST_ARG) ==
               MU_BCAST_ERR_NONE);
  UTEST_ASSERT(s_obj1.call_count == 1);
  UTEST_ASSERT(s_obj2.call_count == 2);
  UTEST_ASSERT(s_a3_called == 1);

  // subscribe when already subscribed
  UTEST_ASSERT(
      mu_bcast_subscribe(&mu_bcast_mgr, CHAN_A, action_a1, &s_obj1) ==
      MU_BCAST_ERR_NONE);
  // action_a1 should receive only one (additional) notification
  UTEST_ASSERT(mu_bcast_notify(&mu_bcast_mgr, CHAN_A, (void *)BCAST_ARG) ==
               MU_BCAST_ERR_NONE);
  UTEST_ASSERT(s_obj1.call_count == 2);
  UTEST_ASSERT(s_obj2.call_count == 3);
  UTEST_ASSERT(s_a3_called == 1);

  // unsubscribe obj2.action_a2 from CHAN_A (only)
  UTEST_ASSERT(mu_bcast_unsubscribe(&mu_bcast_mgr, CHAN_A, action_a2, &s_obj2) ==
               MU_BCAST_ERR_NONE);
  // action_a2 should not receive the notification..
  UTEST_ASSERT(mu_bcast_notify(&mu_bcast_mgr, CHAN_A, (void *)BCAST_ARG) ==
               MU_BCAST_ERR_NONE);
  UTEST_ASSERT(s_obj1.call_count == 3);
  UTEST_ASSERT(s_obj2.call_count == 3);
  UTEST_ASSERT(s_a3_called == 1);

  // notify on all channels
  UTEST_ASSERT(mu_bcast_notify(&mu_bcast_mgr,
                               MU_BCAST_ALL_CHANNELS,
                               (void *)BCAST_ARG) == MU_BCAST_ERR_NONE);
  UTEST_ASSERT(s_obj1.call_count == 4);
  UTEST_ASSERT(s_obj2.call_count == 4);
  UTEST_ASSERT(s_a3_called == 2);

  // unsubscribe when not subscribed
  UTEST_ASSERT(mu_bcast_unsubscribe(&mu_bcast_mgr, CHAN_A, action_a3, NULL) ==
               MU_BCAST_ERR_NOT_FOUND);
}
