/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

#include "mu_test_utils.h"
#include "mulib.h"

// =============================================================================
// private types and definitions

// Define a simple task context for testing.
typedef struct {
  mu_task_t task;
  mu_fsm_t fsm;
} task_a_t;

// Define the four states of Task A.
#define DEFINE_FSM_STATES                                                      \
  DEFINE_FSM_STATE(TASK_A_STATE_INIT, task_a_state_init)                       \
  DEFINE_FSM_STATE(TASK_A_STATE_A, task_a_state_a)                             \
  DEFINE_FSM_STATE(TASK_A_STATE_ERROR, task_a_state_error)                     \
  DEFINE_FSM_STATE(TASK_A_STATE_SUCCESS, task_a_state_success)

// =============================================================================
// private declarations

static void task_a_notify(void *receiver, void *sender);

static void reset(void);

// Expand the forward declarations for the state functions
#undef DEFINE_FSM_STATE
#define DEFINE_FSM_STATE(_name, _fn)                                           \
  static void _fn(void *receiver, void *sender);
DEFINE_FSM_STATES

// Expand an enumeration for the state names
#undef DEFINE_FSM_STATE
#define DEFINE_FSM_STATE(_name, _fn) _name,
typedef enum { DEFINE_FSM_STATES } task_a_state_t;

// =============================================================================
// local storage

static task_a_t s_task_a;

static int s_init_call_count;
static int s_a_call_count;
static int s_error_call_count;
static int s_success_call_count;

// Expand the state function dispatch table
#undef DEFINE_FSM_STATE
#define DEFINE_FSM_STATE(_name, _fn) _fn,
static mu_fsm_state_fn_t s_task_a_state_fns[] = {DEFINE_FSM_STATES};

// (Optinal) Expand the state name table.
#undef DEFINE_FSM_STATE
#define DEFINE_FSM_STATE(_name, _fn) #_name,
static const char *s_task_a_state_names[] = {DEFINE_FSM_STATES};

// Define the number of states.
static const size_t TASK_A_STATE_MAX =
    sizeof(s_task_a_state_fns) / sizeof(mu_fsm_state_fn_t);

// =============================================================================
// public code

void mu_fsm_test() {
  task_a_t *self = &s_task_a;

  reset();
  mu_fsm_init(&self->fsm,
              s_task_a_state_fns,
              s_task_a_state_names,
              TASK_A_STATE_INIT,
              TASK_A_STATE_MAX);
  ASSERT(mu_fsm_get_state(&self->fsm) == TASK_A_STATE_INIT);
  task_a_notify(self, NULL);
  ASSERT(s_init_call_count == 1);
  ASSERT(s_a_call_count == 10);
  ASSERT(s_error_call_count == 0);
  ASSERT(s_success_call_count == 1);
  ASSERT(strcmp("TASK_A_STATE_INIT",
                mu_fsm_state_name(&self->fsm, TASK_A_STATE_INIT)) == 0);
  ASSERT(strcmp("TASK_A_STATE_A",
                mu_fsm_state_name(&self->fsm, TASK_A_STATE_A)) == 0);
  ASSERT(strcmp("TASK_A_STATE_ERROR",
                mu_fsm_state_name(&self->fsm, TASK_A_STATE_ERROR)) == 0);
  ASSERT(strcmp("TASK_A_STATE_SUCCESS",
                mu_fsm_state_name(&self->fsm, TASK_A_STATE_SUCCESS)) == 0);
  ASSERT(strcmp("unknown state", mu_fsm_state_name(&self->fsm, 5)) == 0);
}

// =============================================================================
// private code

static void reset(void) {
  s_init_call_count = 0;
  s_a_call_count = 0;
  s_error_call_count = 0;
  s_success_call_count = 0;
}

static void task_a_notify(void *receiver, void *sender) {
  task_a_t *self = (task_a_t *)receiver;
  mu_fsm_dispatch(&self->fsm, receiver, sender);
}

static void task_a_state_init(void *receiver, void *sender) {
  task_a_t *self = (task_a_t *)receiver;
  s_init_call_count += 1;
  mu_fsm_set_state(&self->fsm, TASK_A_STATE_A);
  mu_fsm_dispatch(&self->fsm, receiver, sender);
}

static void task_a_state_a(void *receiver, void *sender) {
  task_a_t *self = (task_a_t *)receiver;
  s_a_call_count += 1;
  if (s_a_call_count < 10) {
    mu_fsm_set_state(&self->fsm, TASK_A_STATE_A);
  } else {
    mu_fsm_set_state(&self->fsm, TASK_A_STATE_SUCCESS);
  }
  mu_fsm_dispatch(&self->fsm, receiver, sender);
}

static void task_a_state_error(void *receiver, void *sender) {
  // task_a_t *self = (task_a_t *)receiver;
  s_error_call_count += 1;
}

static void task_a_state_success(void *receiver, void *sender) {
  // task_a_t *self = (task_a_t *)receiver;
  s_success_call_count += 1;
}
