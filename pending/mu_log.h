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
 *         MULOG_INIT();
 *
 *         // log messages to the console that are WARNING or more severe.  You
 *         // can dynamically re-subscribe a function to change the severity
 *         // level.
 *         MULOG_SUBSCRIBE(my_console_logger, MULOG_WARNING);
 *
 *         // log to a file messages that are DEBUG or more severe
 *         MULOG_SUBSCRIBE(my_file_logger, MULOG_DEBUG);
 *
 *         int arg = 42;
 *         MULOG_INFO("Arg is %d", arg);  // logs to file but not console
 *     }
 */

#ifndef MULIB_MULOG_H_
#define MULIB_MULOG_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

// =============================================================================
// types and definitions

typedef enum {
  MULOG_TRACE_LEVEL=100,
  MULOG_DEBUG_LEVEL,
  MULOG_INFO_LEVEL,
  MULOG_WARNING_LEVEL,
  MULOG_ERROR_LEVEL,
  MULOG_CRITICAL_LEVEL,
  MULOG_ALWAYS_LEVEL
} mulog_level_t;

// Unless MULOG_ENABLED is defined at compile time, all logging is disabled and
// no logging code is generated.  To enable logging, uncomment the next line or
// add -DMULOG_ENABLED to your compiler switches.

#ifdef MULOG_ENABLED
  #define MULOG_INIT() mulog_init()
  #define MULOG_SUBSCRIBE(a, b) mulog_subscribe(a, b)
  #define MULOG_UNSUBSCRIBE(a) mulog_unsubscribe(a)
  #define MULOG_LEVEL_NAME(a) mulog_level_name(a)
  #define MULOG(...) mulog_message(__VA_ARGS__)
  #define MULOG_TRACE(...) mulog_message(MULOG_TRACE_LEVEL, __VA_ARGS__)
  #define MULOG_DEBUG(...) mulog_message(MULOG_DEBUG_LEVEL, __VA_ARGS__)
  #define MULOG_INFO(...) mulog_message(MULOG_INFO_LEVEL, __VA_ARGS__)
  #define MULOG_WARNING(...) mulog_message(MULOG_WARNING_LEVEL, __VA_ARGS__)
  #define MULOG_ERROR(...) mulog_message(MULOG_ERROR_LEVEL, __VA_ARGS__)
  #define MULOG_CRITICAL(...) mulog_message(MULOG_CRITICAL_LEVEL, __VA_ARGS__)
  #define MULOG_ALWAYS(...) mulog_message(MULOG_ALWAYS_LEVEL, __VA_ARGS__)
#else
  // uLog vanishes when disabled at compile time...
  #define MULOG_INIT() do {} while(0)
  #define MULOG_SUBSCRIBE(a, b) do {} while(0)
  #define MULOG_UNSUBSCRIBE(a) do {} while(0)
  #define MULOG_LEVEL_NAME(A) do {} while(0)
  #define MULOG(s, f, ...) do {} while(0)
  #define MULOG_TRACE(f, ...) do {} while(0)
  #define MULOG_DEBUG(f, ...) do {} while(0))
  #define MULOG_INFO(f, ...) do {} while(0)
  #define MULOG_WARNING(f, ...) do {} while(0)
  #define MULOG_ERROR(f, ...) do {} while(0)
  #define MULOG_CRITICAL(f, ...) do {} while(0)
  #define MULOG_ALWAYS(f, ...) do {} while(0)
#endif

typedef enum {
  MULOG_ERR_NONE = 0,
  MULOG_ERR_SUBSCRIBERS_EXCEEDED,
  MULOG_ERR_NOT_SUBSCRIBED,
} mulog_err_t;

// define the maximum number of concurrent subscribers
#define MULOG_MAX_SUBSCRIBERS 6

// maximum length of formatted log message
#define MULOG_MAX_MESSAGE_LENGTH 120

/**
 * @brief: prototype for uLog subscribers.
 */
typedef void (*mulog_function_t)(mulog_level_t severity, char *msg);

// =============================================================================
// declarations

void mulog_init();
mulog_err_t mulog_subscribe(mulog_function_t fn, mulog_level_t threshold);
mulog_err_t mulog_unsubscribe(mulog_function_t fn);
const char *mulog_level_name(mulog_level_t level);
void mulog_message(mulog_level_t severity, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_MULOG_H_
