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

#include "../src/mulog.h"
#include "test_utilities.h"
#include <string.h>

// =============================================================================
// private types and definitions

#define TEST_TRACE 0
#define TEST_DEBUG 1
#define TEST_INFO 2
#define TEST_WARNING 3
#define TEST_ERROR 4
#define TEST_CRITICAL 5

// =============================================================================
// private declarations

static void logger_fn0(mulog_level_t severity, char *msg);
static void logger_fn1(mulog_level_t severity, char *msg);
static void logger_fn2(mulog_level_t severity, char *msg);
static void logger_fn3(mulog_level_t severity, char *msg);
static void logger_fn4(mulog_level_t severity, char *msg);
static void logger_fn5(mulog_level_t severity, char *msg);
static void logger_fn6(mulog_level_t severity, char *msg);

// =============================================================================
// local storage

static int s_fn_calls[6];

// =============================================================================
// public code

void mulog_test() {
  MULOG_INIT();
  memset(s_fn_calls, 0, sizeof(s_fn_calls));

  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn0, MULOG_TRACE_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn1, MULOG_DEBUG_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn2, MULOG_INFO_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn3, MULOG_WARNING_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn4, MULOG_ERROR_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn5, MULOG_CRITICAL_LEVEL) == MULOG_ERR_NONE);

  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn6, MULOG_TRACE_LEVEL) == MULOG_ERR_SUBSCRIBERS_EXCEEDED);

  MULOG_TRACE("Hello!");
  MULOG_DEBUG("Hello!");
  MULOG_INFO("Hello!");
  MULOG_WARNING("Hello!");
  MULOG_ERROR("Hello!");
  MULOG_CRITICAL("Hello!");
  MULOG_ALWAYS("Hello!");

  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 7);  // logger_fn0 is at trace level: all messages
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 6);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 5);
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 4);
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 3);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 2);  // logger_fn5 receives critical and alwasy only

  // ULOG with explicit severity parameter
  MULOG(MULOG_INFO_LEVEL, "Hello!");

  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 8);  // logger_fn0 is at trace level: all messages
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 7);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 6);  // logger_fn2 and lower get info messages
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 4);  // logger_fn3 and higher don't get info messages
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 3);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 2);  // logger_fn5 receives critical msgs only

  // reset counters.  Test reassigning levels...
  memset(s_fn_calls, 0, sizeof(s_fn_calls));

  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn0, MULOG_CRITICAL_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn1, MULOG_ERROR_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn2, MULOG_WARNING_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn3, MULOG_INFO_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn4, MULOG_DEBUG_LEVEL) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn5, MULOG_TRACE_LEVEL) == MULOG_ERR_NONE);

  UTEST_ASSERT(MULOG_SUBSCRIBE(logger_fn6, MULOG_TRACE_LEVEL) == MULOG_ERR_SUBSCRIBERS_EXCEEDED);

  MULOG_TRACE("Hello!");
  MULOG_DEBUG("Hello!");
  MULOG_INFO("Hello!");
  MULOG_WARNING("Hello!");
  MULOG_ERROR("Hello!");
  MULOG_CRITICAL("Hello!");
  MULOG_ALWAYS("Hello!");

  // Don't be fooled by the symbolic names: they've been reassigned...
  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 2);  // logger_fn0 receives critical and always msgs
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 3);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 4);
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 5);
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 6);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 7);  // logger_fn5 is at trace level: all messages

  // reset counters.  Test unsubscribe
  memset(s_fn_calls, 0, sizeof(s_fn_calls));

  UTEST_ASSERT(MULOG_UNSUBSCRIBE(logger_fn0) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_UNSUBSCRIBE(logger_fn1) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_UNSUBSCRIBE(logger_fn2) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_UNSUBSCRIBE(logger_fn3) == MULOG_ERR_NONE);
  UTEST_ASSERT(MULOG_UNSUBSCRIBE(logger_fn4) == MULOG_ERR_NONE);
  // leave logger_fn5 subscribed
  // UTEST_ASSERT(MULOG_UNSUBSCRIBE(logger_fn5, MULOG_TRACE) == MULOG_ERR_NONE);

  UTEST_ASSERT(MULOG_UNSUBSCRIBE(logger_fn6) == MULOG_ERR_NOT_SUBSCRIBED);

  MULOG_TRACE("Hello!");
  MULOG_DEBUG("Hello!");
  MULOG_INFO("Hello!");
  MULOG_WARNING("Hello!");
  MULOG_ERROR("Hello!");
  MULOG_CRITICAL("Hello!");
  MULOG_ALWAYS("Hello!");

  UTEST_ASSERT(s_fn_calls[TEST_TRACE] == 0);  // not subscribed...
  UTEST_ASSERT(s_fn_calls[TEST_DEBUG] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_INFO] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_WARNING] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_ERROR] == 0);
  UTEST_ASSERT(s_fn_calls[TEST_CRITICAL] == 7);  // logger_fn5 is at trace level: all messages

  // MULOG_LEVEL_NAME
  UTEST_ASSERT(strcmp(MULOG_LEVEL_NAME(MULOG_TRACE_LEVEL), "TRACE") == 0);
  UTEST_ASSERT(strcmp(MULOG_LEVEL_NAME(MULOG_DEBUG_LEVEL), "DEBUG") == 0);
  UTEST_ASSERT(strcmp(MULOG_LEVEL_NAME(MULOG_INFO_LEVEL), "INFO") == 0);
  UTEST_ASSERT(strcmp(MULOG_LEVEL_NAME(MULOG_WARNING_LEVEL), "WARNING") == 0);
  UTEST_ASSERT(strcmp(MULOG_LEVEL_NAME(MULOG_ERROR_LEVEL), "ERROR") == 0);
  UTEST_ASSERT(strcmp(MULOG_LEVEL_NAME(MULOG_CRITICAL_LEVEL), "CRITICAL") == 0);
  UTEST_ASSERT(strcmp(MULOG_LEVEL_NAME(MULOG_ALWAYS_LEVEL), "ALWAYS") == 0);

}

// =============================================================================
// private code

static void logger_fn0(mulog_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_TRACE]++;
}

static void logger_fn1(mulog_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_DEBUG]++;
}

static void logger_fn2(mulog_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_INFO]++;
}

static void logger_fn3(mulog_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_WARNING]++;
}

static void logger_fn4(mulog_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_ERROR]++;
}

static void logger_fn5(mulog_level_t severity, char *msg) {
  UTEST_ASSERT(strcmp(msg, "Hello!") == 0);
  s_fn_calls[TEST_CRITICAL]++;
}

static void logger_fn6(mulog_level_t severity, char *msg) {
  UTEST_ASSERT("should not get called" == NULL);
}
