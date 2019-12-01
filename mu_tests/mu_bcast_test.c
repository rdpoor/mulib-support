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

#include "mu_bcast.h"
#include "test_utilities.h"
#include <stdint.h>
#include <stddef.h>

#define N_SUBSCRIBERS 4

typedef struct {
  int call_count;
} test_obj_t;

static mu_bcast_subscriber_t s_subscribers[N_SUBSCRIBERS];

// ARM compiler complains that CHAN_A + 1 is not constant
// const mu_bcast_channel_t CHAN_A = MU_BCAST_CH_MIN;
// const mu_bcast_channel_t CHAN_B = CHAN_A + 1;
// const mu_bcast_channel_t CHAN_C = CHAN_B + 1;

#define CHAN_A (MU_BCAST_CH_MIN)
#define CHAN_B (CHAN_A + 1)
#define CHAN_C (CHAN_B + 1)

const uintptr_t BCAST_ARG = (uintptr_t)123;

static mu_task_t s_task_1;
static mu_task_t s_task_2;

static test_obj_t s_test_obj_1;
static test_obj_t s_test_obj_2;

static void task_1_fn(void *self, void *arg) {
  test_obj_t *obj = (test_obj_t *)self;
  obj->call_count += 1;
  UTEST_ASSERT((uintptr_t)arg == BCAST_ARG);
}

static void task_2_fn(void *self, void *arg) {
  test_obj_t *obj = (test_obj_t *)self;
  obj->call_count += 1;
  UTEST_ASSERT((uintptr_t)arg == BCAST_ARG);
}

void mu_bcast_test() {
  mu_bcast_mgr_t mu_bcast_mgr;

  // initialize the tasks
  mu_task_init(&s_task_1, task_1_fn, &s_test_obj_1, "Task 1");
  mu_task_init(&s_task_2, task_2_fn, &s_test_obj_2, "Task 2");

  // initialize the bcast manager
  mu_bcast_init(&mu_bcast_mgr, s_subscribers, N_SUBSCRIBERS);

  // mu_bcast_subscribe()
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        MU_BCAST_CH_MIN - 1,
                                        &s_task_1),
                     MU_BCAST_ERR_ILLEGAL_CHANNEL);
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        MU_BCAST_CH_MAX + 1,
                                        &s_task_1),
                     MU_BCAST_ERR_ILLEGAL_CHANNEL);
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        CHAN_A,
                                        &s_task_1),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        CHAN_A,
                                        &s_task_1),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        CHAN_B,
                                        &s_task_2),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        CHAN_C,
                                        &s_task_2),
                     MU_BCAST_ERR_NONE);
  // at this point:
  // task1 subscribes to CHAN_A
  // task1 and task2 subscribes to CHAN_B
  // task2 subscribes to CHAN_C
  // ... and the subscriber pool is full...
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        CHAN_C,
                                        &s_task_1),
                     MU_BCAST_ERR_FULL);

  // notification with illegal channel
  UTEST_ASSERTEQ_INT(mu_bcast_notify(&mu_bcast_mgr,
                                     MU_BCAST_CH_UNASSIGNED,
                                     (void *)BCAST_ARG),
                     MU_BCAST_ERR_ILLEGAL_CHANNEL);

  // notifications with a legal channel
  UTEST_ASSERTEQ_INT(mu_bcast_notify(&mu_bcast_mgr, CHAN_A, (void *)BCAST_ARG),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(s_test_obj_1.call_count, 1);
  UTEST_ASSERTEQ_INT(s_test_obj_2.call_count, 0);

  UTEST_ASSERTEQ_INT(mu_bcast_notify(&mu_bcast_mgr, CHAN_B, (void *)BCAST_ARG),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(s_test_obj_1.call_count, 2);
  UTEST_ASSERTEQ_INT(s_test_obj_2.call_count, 1);

  UTEST_ASSERTEQ_INT(mu_bcast_notify(&mu_bcast_mgr, CHAN_C, (void *)BCAST_ARG),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(s_test_obj_1.call_count, 2);
  UTEST_ASSERTEQ_INT(s_test_obj_2.call_count, 2);

  // subscribe when already subscribed
  UTEST_ASSERTEQ_INT(mu_bcast_subscribe(&mu_bcast_mgr,
                                        CHAN_A,
                                        &s_task_1),
                     MU_BCAST_ERR_NONE);
  // action_a1 should receive only one (additional) notification
  UTEST_ASSERTEQ_INT(mu_bcast_notify(&mu_bcast_mgr, CHAN_A, (void *)BCAST_ARG),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(s_test_obj_1.call_count, 3);
  UTEST_ASSERTEQ_INT(s_test_obj_2.call_count, 2);

  // unsubscribe task1 from CHAN_A (only)
  UTEST_ASSERTEQ_INT(mu_bcast_unsubscribe(&mu_bcast_mgr, CHAN_A, &s_task_1),
                     MU_BCAST_ERR_NONE);
  // at this point:
  // task1 and task2 subscribes to CHAN_B
  // task2 subscribes to CHAN_C

  // nobody listening on CHAN_A
  UTEST_ASSERTEQ_INT(mu_bcast_notify(&mu_bcast_mgr, CHAN_A, (void *)BCAST_ARG),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERTEQ_INT(s_test_obj_1.call_count, 3);
  UTEST_ASSERTEQ_INT(s_test_obj_2.call_count, 2);

  // notify on all channels
  UTEST_ASSERTEQ_INT(mu_bcast_notify(&mu_bcast_mgr,
                                     MU_BCAST_CH_WILDCARD,
                                     (void *)BCAST_ARG),
                     MU_BCAST_ERR_NONE);
  UTEST_ASSERT(s_test_obj_1.call_count == 4);
  UTEST_ASSERT(s_test_obj_2.call_count == 4);  // increments on CHAN_B & CHAN_C

  // unsubscribe when not subscribed
  UTEST_ASSERTEQ_INT(mu_bcast_unsubscribe(&mu_bcast_mgr, CHAN_A, &s_task_1),
                     MU_BCAST_ERR_NOT_FOUND);
}
