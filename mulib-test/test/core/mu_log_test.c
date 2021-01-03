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

#include "mulib.h"
#include "mu_test_utils.h"
#include <string.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

static void reset(void);

static void logger1(mu_log_level_t severity, const char *msg);
static void logger2(mu_log_level_t severity, const char *msg);
static void logger3(mu_log_level_t severity, const char *msg);
static void logger4(mu_log_level_t severity, const char *msg);
static void logger5(mu_log_level_t severity, const char *msg);
static void logger6(mu_log_level_t severity, const char *msg);
static void logger7(mu_log_level_t severity, const char *msg);
static void logger_a(mu_log_level_t severity, const char *msg);
static void logger_b(mu_log_level_t severity, const char *msg);


// =============================================================================
// local storage

static int s_call_count1;
static int s_call_count2;
static int s_call_count3;
static int s_call_count4;
static int s_call_count5;
static int s_call_count6;
static int s_call_count7;
static int s_call_count_a;
static int s_call_count_b;

// =============================================================================
// public code

#ifndef MU_LOG_ENABLED
#error MU_LOG_ENABLED must be defined to run these tests
#endif

void mu_log_test() {
  reset();
  ASSERT(MU_LOG_SUBSCRIBE(logger1, MU_LOG_TRACE_LEVEL) == MU_LOG_ERR_NONE);
  ASSERT(MU_LOG_SUBSCRIBE(logger2, MU_LOG_DEBUG_LEVEL) == MU_LOG_ERR_NONE);
  ASSERT(MU_LOG_SUBSCRIBE(logger3, MU_LOG_INFO_LEVEL) == MU_LOG_ERR_NONE);
  ASSERT(MU_LOG_SUBSCRIBE(logger4, MU_LOG_WARN_LEVEL) == MU_LOG_ERR_NONE);
  ASSERT(MU_LOG_SUBSCRIBE(logger5, MU_LOG_ERROR_LEVEL) == MU_LOG_ERR_NONE);
  ASSERT(MU_LOG_SUBSCRIBE(logger6, MU_LOG_CRITICAL_LEVEL) == MU_LOG_ERR_NONE);
  ASSERT(MU_LOG_SUBSCRIBE(logger7, MU_LOG_INFO_LEVEL) == MU_LOG_ERR_SUBSCRIBERS_EXCEEDED);

  ASSERT(s_call_count1 == 0);
  ASSERT(s_call_count2 == 0);
  ASSERT(s_call_count3 == 0);
  ASSERT(s_call_count4 == 0);
  ASSERT(s_call_count5 == 0);
  ASSERT(s_call_count6 == 0);
  ASSERT(s_call_count7 == 0);

  MU_LOG_TRACE("TRACE");
  ASSERT(s_call_count1 == 1);
  ASSERT(s_call_count2 == 0);
  ASSERT(s_call_count3 == 0);
  ASSERT(s_call_count4 == 0);
  ASSERT(s_call_count5 == 0);
  ASSERT(s_call_count6 == 0);
  ASSERT(s_call_count7 == 0);

  MU_LOG_DEBUG("DEBUG");
  ASSERT(s_call_count1 == 2);
  ASSERT(s_call_count2 == 1);
  ASSERT(s_call_count3 == 0);
  ASSERT(s_call_count4 == 0);
  ASSERT(s_call_count5 == 0);
  ASSERT(s_call_count6 == 0);
  ASSERT(s_call_count7 == 0);

  MU_LOG_INFO("INFO");
  ASSERT(s_call_count1 == 3);
  ASSERT(s_call_count2 == 2);
  ASSERT(s_call_count3 == 1);
  ASSERT(s_call_count4 == 0);
  ASSERT(s_call_count5 == 0);
  ASSERT(s_call_count6 == 0);
  ASSERT(s_call_count7 == 0);

  MU_LOG_WARN("WARN");
  ASSERT(s_call_count1 == 4);
  ASSERT(s_call_count2 == 3);
  ASSERT(s_call_count3 == 2);
  ASSERT(s_call_count4 == 1);
  ASSERT(s_call_count5 == 0);
  ASSERT(s_call_count6 == 0);
  ASSERT(s_call_count7 == 0);

  MU_LOG_ERROR("ERROR");
  ASSERT(s_call_count1 == 5);
  ASSERT(s_call_count2 == 4);
  ASSERT(s_call_count3 == 3);
  ASSERT(s_call_count4 == 2);
  ASSERT(s_call_count5 == 1);
  ASSERT(s_call_count6 == 0);
  ASSERT(s_call_count7 == 0);

  MU_LOG_CRITICAL("CRITICAL");
  ASSERT(s_call_count1 == 6);
  ASSERT(s_call_count2 == 5);
  ASSERT(s_call_count3 == 4);
  ASSERT(s_call_count4 == 3);
  ASSERT(s_call_count5 == 2);
  ASSERT(s_call_count6 == 1);
  ASSERT(s_call_count7 == 0);

  ASSERT(MU_LOG_UNSUBSCRIBE(logger1) == MU_LOG_ERR_NONE);

  MU_LOG_CRITICAL("CRITICAL");
  ASSERT(s_call_count1 == 6);  // logger1 no is longer called.
  ASSERT(s_call_count2 == 6);
  ASSERT(s_call_count3 == 5);
  ASSERT(s_call_count4 == 4);
  ASSERT(s_call_count5 == 3);
  ASSERT(s_call_count6 == 2);
  ASSERT(s_call_count7 == 0);

  reset();
  ASSERT(MU_LOG_SUBSCRIBE(logger_b, MU_LOG_INFO_LEVEL) == MU_LOG_ERR_NONE);
  ASSERT(MU_LOG_SUBSCRIBE(logger_a, MU_LOG_INFO_LEVEL) == MU_LOG_ERR_NONE);
  // at this point, b would be called before a.  but...
  ASSERT(MU_LOG_SUBSCRIBE(logger_b, MU_LOG_INFO_LEVEL) == MU_LOG_ERR_NONE);
  // ...re-subscribing moves it to the end of the list.  logger_b will verify
  // that logger_a was called before logger_b
  MU_LOG_INFO("anything");
  ASSERT(s_call_count_a == 1);
  ASSERT(s_call_count_b == 1);

  // mu_log_level_name()
  ASSERT(strcmp(mu_log_level_name(MU_LOG_TRACE_LEVEL), "TRACE") == 0);
  ASSERT(strcmp(mu_log_level_name(MU_LOG_DEBUG_LEVEL), "DEBUG") == 0);
  ASSERT(strcmp(mu_log_level_name(MU_LOG_INFO_LEVEL), "INFO") == 0);
  ASSERT(strcmp(mu_log_level_name(MU_LOG_WARN_LEVEL), "WARN") == 0);
  ASSERT(strcmp(mu_log_level_name(MU_LOG_ERROR_LEVEL), "ERROR") == 0);
  ASSERT(strcmp(mu_log_level_name(MU_LOG_CRITICAL_LEVEL), "CRITICAL") == 0);

  ASSERT(strcmp(mu_log_level_name(MU_LOG_TRACE_LEVEL-1), "UNKNOWN") == 0);
  ASSERT(strcmp(mu_log_level_name(MU_LOG_CRITICAL_LEVEL+1), "UNKNOWN") == 0);
}

// =============================================================================
// private code

static void reset(void) {
  MU_LOG_INIT();
  s_call_count1 = 0;
  s_call_count2 = 0;
  s_call_count3 = 0;
  s_call_count4 = 0;
  s_call_count5 = 0;
  s_call_count6 = 0;
  s_call_count7 = 0;
  s_call_count_a = 0;
  s_call_count_b = 0;
}

void logger1(mu_log_level_t severity, const char *msg) {
  ASSERT(strcmp(MU_LOG_LEVEL_NAME(severity), msg) == 0);
  s_call_count1 += 1;
}

void logger2(mu_log_level_t severity, const char *msg) {
  ASSERT(strcmp(MU_LOG_LEVEL_NAME(severity), msg) == 0);
  s_call_count2 += 1;
}

void logger3(mu_log_level_t severity, const char *msg) {
  ASSERT(strcmp(MU_LOG_LEVEL_NAME(severity), msg) == 0);
  s_call_count3 += 1;
}

void logger4(mu_log_level_t severity, const char *msg) {
  ASSERT(strcmp(MU_LOG_LEVEL_NAME(severity), msg) == 0);
  s_call_count4 += 1;
}

void logger5(mu_log_level_t severity, const char *msg) {
  ASSERT(strcmp(MU_LOG_LEVEL_NAME(severity), msg) == 0);
  s_call_count5 += 1;
}

void logger6(mu_log_level_t severity, const char *msg) {
  ASSERT(strcmp(MU_LOG_LEVEL_NAME(severity), msg) == 0);
  s_call_count6 += 1;
}

// never gets called...
void logger7(mu_log_level_t severity, const char *msg) {
  ASSERT(strcmp(MU_LOG_LEVEL_NAME(severity), msg) == 0);
  s_call_count7 += 1;
}

void logger_a(mu_log_level_t severity, const char *msg) {
  s_call_count_a += 1;
}

void logger_b(mu_log_level_t severity, const char *msg) {
  // Assure that logger_a was called before logger_b
  ASSERT(s_call_count_a == 1);
  s_call_count_b += 1;
}
