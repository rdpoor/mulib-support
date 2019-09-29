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

/**
 * \file
 *
 * \brief uLog: lightweight logging for embedded systems
 *
 * A quick intro by example:
 *
 *     #include "mulog.h"
 *
 *     // To use muLog, you must define a function to process logging messages.
 *     // It can write the messages to a console, to a file, to an in-memory
 *     // buffer: the choice is yours.  And you get to choose the format of
 *     // the message.  This example prints to the console.
 *     //
 *     void my_console_logger(ulog_level_t level, const char *msg) {
 *         printf("%s [%s]: %s\n",
 *             get_timestamp(),
 *             ulog_level_name(level),
 *             msg);
 *     }
 *
 *     int main() {
 *         MU_LOG_INIT();
 *
 *         // log messages to the console that are WARNING or more severe.  You
 *         // can dynamically re-subscribe a function to change the severity
 *         // level.
 *         MU_LOG_SUBSCRIBE(my_console_logger, MU_LOG_WARNING);
 *
 *         // log to a file messages that are DEBUG or more severe
 *         MU_LOG_SUBSCRIBE(my_file_logger, MU_LOG_DEBUG);
 *
 *         int arg = 42;
 *         MU_LOG_INFO("Arg is %d", arg);  // logs to file but not console
 *     }
 */

#ifndef MU_LOG_H_
#define MU_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

// =============================================================================
// types and definitions

typedef enum {
  MU_LOG_TRACE_LEVEL=100,
  MU_LOG_DEBUG_LEVEL,
  MU_LOG_INFO_LEVEL,
  MU_LOG_WARNING_LEVEL,
  MU_LOG_ERROR_LEVEL,
  MU_LOG_CRITICAL_LEVEL,
  MU_LOG_ALWAYS_LEVEL
} mu_log_level_t;

// Unless MU_LOG_ENABLED is defined at compile time, all logging is disabled and
// no logging code is generated.  To enable logging, uncomment the next line or
// add -DMU_LOG_ENABLED to your compiler switches.
//
// #define MU_LOG_ENABLED

#ifdef MU_LOG_ENABLED
  #define MU_LOG_INIT() mu_log_init()
  #define MU_LOG_SUBSCRIBE(a, b) mu_log_subscribe(a, b)
  #define MU_LOG_UNSUBSCRIBE(a) mu_log_unsubscribe(a)
  #define MU_LOG_LEVEL_NAME(a) mu_log_level_name(a)
  #define MU_LOG(...) mu_log_message(__VA_ARGS__)
  #define MU_LOG_TRACE(...) mu_log_message(MU_LOG_TRACE_LEVEL, __VA_ARGS__)
  #define MU_LOG_DEBUG(...) mu_log_message(MU_LOG_DEBUG_LEVEL, __VA_ARGS__)
  #define MU_LOG_INFO(...) mu_log_message(MU_LOG_INFO_LEVEL, __VA_ARGS__)
  #define MU_LOG_WARNING(...) mu_log_message(MU_LOG_WARNING_LEVEL, __VA_ARGS__)
  #define MU_LOG_ERROR(...) mu_log_message(MU_LOG_ERROR_LEVEL, __VA_ARGS__)
  #define MU_LOG_CRITICAL(...) mu_log_message(MU_LOG_CRITICAL_LEVEL, __VA_ARGS__)
  #define MU_LOG_ALWAYS(...) mu_log_message(MU_LOG_ALWAYS_LEVEL, __VA_ARGS__)
#else
  // uLog vanishes when disabled at compile time...
  #define MU_LOG_INIT() do {} while(0)
  #define MU_LOG_SUBSCRIBE(a, b) do {} while(0)
  #define MU_LOG_UNSUBSCRIBE(a) do {} while(0)
  #define MU_LOG_LEVEL_NAME(A) do {} while(0)
  #define MU_LOG(s, f, ...) do {} while(0)
  #define MU_LOG_TRACE(f, ...) do {} while(0)
  #define MU_LOG_DEBUG(f, ...) do {} while(0))
  #define MU_LOG_INFO(f, ...) do {} while(0)
  #define MU_LOG_WARNING(f, ...) do {} while(0)
  #define MU_LOG_ERROR(f, ...) do {} while(0)
  #define MU_LOG_CRITICAL(f, ...) do {} while(0)
  #define MU_LOG_ALWAYS(f, ...) do {} while(0)
#endif

#ifdef MU_LOG_ENABLED  // remainder of file...

typedef enum {
  MU_LOG_ERR_NONE = 0,
  MU_LOG_ERR_SUBSCRIBERS_EXCEEDED,
  MU_LOG_ERR_NOT_SUBSCRIBED,
} mu_log_err_t;

// define the maximum number of concurrent subscribers
#define MU_LOG_MAX_SUBSCRIBERS 6

// maximum length of formatted log message
#define MU_LOG_MAX_MESSAGE_LENGTH 120

/**
 * @brief: prototype for uLog subscribers.
 */
typedef void (*mu_log_subscriber_fn)(mu_log_level_t severity, char *msg);

// =============================================================================
// declarations

void mu_log_init();
mu_log_err_t mu_log_subscribe(mu_log_subscriber_fn fn, mu_log_level_t threshold);
mu_log_err_t mu_log_unsubscribe(mu_log_subscriber_fn fn);
const char *mu_log_level_name(mu_log_level_t level);
void mu_log_message(mu_log_level_t severity, const char *fmt, ...);

#endif // #ifdef MU_LOG_ENABLED

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_MU_LOG_H_
