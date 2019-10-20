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

// Note: The UTEST_ASSERT forms will fail if logging is disabled, so we force
// it to be on for this test file before including other files

#define MU_LOG_ENABLED

#include "mu_log.h"
#include "test_utilities.h"
#include <string.h>

// =============================================================================
// private types and definitions

#define MAX_SUBSCRIBERS 6
#define MAX_MESSAGE_LENGTH 120

#define TEST_TRACE 0
#define TEST_DEBUG 1
#define TEST_INFO 2
#define TEST_WARNING 3
#define TEST_ERROR 4
#define TEST_CRITICAL 5

// =============================================================================
// private declarations

static void logger_fn0(mu_log_level_t severity, char *msg);
static void logger_fn1(mu_log_level_t severity, char *msg);
static void logger_fn2(mu_log_level_t severity, char *msg);
static void logger_fn3(mu_log_level_t severity, char *msg);
static void logger_fn4(mu_log_level_t severity, char *msg);
static void logger_fn5(mu_log_level_t severity, char *msg);
static void logger_fn6(mu_log_level_t severity, char *msg);

// =============================================================================
// local storage

static int s_fn_calls[6];

// =============================================================================
// public code

void mu_log_test() {
  mu_log_subscriber_t subscriber_pool[MAX_SUBSCRIBERS];
  char msg_buf[MAX_MESSAGE_LENGTH];

  MU_LOG_INIT(subscriber_pool, MAX_SUBSCRIBERS, msg_buf, MAX_MESSAGE_LENGTH);
  memset(s_fn_calls, 0, sizeof(s_fn_calls));

  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn0, MU_LOG_TRACE_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn1, MU_LOG_DEBUG_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn2, MU_LOG_INFO_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn3, MU_LOG_WARNING_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn4, MU_LOG_ERROR_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn5, MU_LOG_CRITICAL_LEVEL) == MU_LOG_ERR_NONE);

  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn6, MU_LOG_TRACE_LEVEL) == MU_LOG_ERR_SUBSCRIBERS_EXCEEDED);

  MU_LOG_TRACE("Hello!");
  MU_LOG_DEBUG("Hello!");
  MU_LOG_INFO("Hello!");
  MU_LOG_WARNING("Hello!");
  MU_LOG_ERROR("Hello!");
  MU_LOG_CRITICAL("Hello!");
  MU_LOG_ALWAYS("Hello!");

  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 7);  // logger_fn0 is at trace level: all messages
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 6);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 5);
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 4);
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 3);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 2);  // logger_fn5 receives critical and alwasy only

  // ULOG with explicit severity parameter
  MU_LOG(MU_LOG_INFO_LEVEL, "Hello!");

  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 8);  // logger_fn0 is at trace level: all messages
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 7);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 6);  // logger_fn2 and lower get info messages
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 4);  // logger_fn3 and higher don't get info messages
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 3);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 2);  // logger_fn5 receives critical msgs only

  // reset counters.  Test reassigning levels...
  memset(s_fn_calls, 0, sizeof(s_fn_calls));

  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn0, MU_LOG_CRITICAL_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn1, MU_LOG_ERROR_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn2, MU_LOG_WARNING_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn3, MU_LOG_INFO_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn4, MU_LOG_DEBUG_LEVEL) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn5, MU_LOG_TRACE_LEVEL) == MU_LOG_ERR_NONE);

  UTEST_ASSERT(MU_LOG_SUBSCRIBE(logger_fn6, MU_LOG_TRACE_LEVEL) == MU_LOG_ERR_SUBSCRIBERS_EXCEEDED);

  MU_LOG_TRACE("Hello!");
  MU_LOG_DEBUG("Hello!");
  MU_LOG_INFO("Hello!");
  MU_LOG_WARNING("Hello!");
  MU_LOG_ERROR("Hello!");
  MU_LOG_CRITICAL("Hello!");
  MU_LOG_ALWAYS("Hello!");

  // Don't be fooled by the symbolic names: they've been reassigned...
  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 2);  // logger_fn0 receives critical and always msgs
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 3);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 4);
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 5);
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 6);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 7);  // logger_fn5 is at trace level: all messages

  // reset counters.  Test unsubscribe
  memset(s_fn_calls, 0, sizeof(s_fn_calls));

  UTEST_ASSERT(MU_LOG_UNSUBSCRIBE(logger_fn0) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_UNSUBSCRIBE(logger_fn1) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_UNSUBSCRIBE(logger_fn2) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_UNSUBSCRIBE(logger_fn3) == MU_LOG_ERR_NONE);
  UTEST_ASSERT(MU_LOG_UNSUBSCRIBE(logger_fn4) == MU_LOG_ERR_NONE);
  // leave logger_fn5 subscribed
  // UTEST_ASSERT(MU_LOG_UNSUBSCRIBE(logger_fn5, MU_LOG_TRACE) == MU_LOG_ERR_NONE);

  UTEST_ASSERT(MU_LOG_UNSUBSCRIBE(logger_fn6) == MU_LOG_ERR_NOT_SUBSCRIBED);

  MU_LOG_TRACE("Hello!");
  MU_LOG_DEBUG("Hello!");
  MU_LOG_INFO("Hello!");
  MU_LOG_WARNING("Hello!");
  MU_LOG_ERROR("Hello!");
  MU_LOG_CRITICAL("Hello!");
  MU_LOG_ALWAYS("Hello!");

  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 0);  // not subscribed...
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 7);  // logger_fn5 is at trace level: all messages

  // MU_LOG_LEVEL_NAME
  UTEST_ASSERT(strcmp(MU_LOG_LEVEL_NAME(MU_LOG_TRACE_LEVEL), "TRACE") == 0);
  UTEST_ASSERT(strcmp(MU_LOG_LEVEL_NAME(MU_LOG_DEBUG_LEVEL), "DEBUG") == 0);
  UTEST_ASSERT(strcmp(MU_LOG_LEVEL_NAME(MU_LOG_INFO_LEVEL), "INFO") == 0);
  UTEST_ASSERT(strcmp(MU_LOG_LEVEL_NAME(MU_LOG_WARNING_LEVEL), "WARNING") == 0);
  UTEST_ASSERT(strcmp(MU_LOG_LEVEL_NAME(MU_LOG_ERROR_LEVEL), "ERROR") == 0);
  UTEST_ASSERT(strcmp(MU_LOG_LEVEL_NAME(MU_LOG_CRITICAL_LEVEL), "CRITICAL") == 0);
  UTEST_ASSERT(strcmp(MU_LOG_LEVEL_NAME(MU_LOG_ALWAYS_LEVEL), "ALWAYS") == 0);

}

// =============================================================================
// private code

static void logger_fn0(mu_log_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_TRACE]++;
}

static void logger_fn1(mu_log_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_DEBUG]++;
}

static void logger_fn2(mu_log_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_INFO]++;
}

static void logger_fn3(mu_log_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_WARNING]++;
}

static void logger_fn4(mu_log_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_ERROR]++;
}

static void logger_fn5(mu_log_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_CRITICAL]++;
}

static void logger_fn6(mu_log_level_t severity, char *msg) {
  UTEST_ASSERT("should not get called" == NULL);
}
