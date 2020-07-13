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

#ifndef _MU_LOG_H_
#define _MU_LOG_H_

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
  MU_LOG_CRITICAL_LEVEL
} mu_log_level_t;

// Unless MU_LOG_ENABLED is defined at compile time, all logging is disabled and
// no logging code is generated.  To enable logging, uncomment the next line or
// add -DMU_LOG_ENABLED to your compiler switches.

#define MU_LOG_ENABLED

#ifdef MU_LOG_ENABLED
  #define MU_LOG_INIT() mu_log_init()
  #define MU_LOG_SUBSCRIBE(a, b) mu_log_subscribe(a, b)
  #define MU_LOG_UNSUBSCRIBE(a) mu_log_unsubscribe(a)
  #define MU_LOG_LEVEL_NAME(a) mu_log_level_name(a)
  #define MU_LOG_TRACE(...) mu_log_message(MU_LOG_TRACE_LEVEL, __VA_ARGS__)
  #define MU_LOG_DEBUG(...) mu_log_message(MU_LOG_DEBUG_LEVEL, __VA_ARGS__)
  #define MU_LOG_INFO(...) mu_log_message(MU_LOG_INFO_LEVEL, __VA_ARGS__)
  #define MU_LOG_WARNING(...) mu_log_message(MU_LOG_WARNING_LEVEL, __VA_ARGS__)
  #define MU_LOG_ERROR(...) mu_log_message(MU_LOG_ERROR_LEVEL, __VA_ARGS__)
  #define MU_LOG_CRITICAL(...) mu_log_message(MU_LOG_CRITICAL_LEVEL, __VA_ARGS__)
#else
  // uLog vanishes when disabled at compile time...
  #define MU_LOG_INIT() do {} while(0)
  #define MU_LOG_SUBSCRIBE(a, b) do {} while(0)
  #define MU_LOG_UNSUBSCRIBE(a) do {} while(0)
  #define MU_LOG_LEVEL_NAME(a) do {} while(0)
  #define MU_LOG_TRACE(f, ...) do {} while(0)
  #define MU_LOG_DEBUG(f, ...) do {} while(0))
  #define MU_LOG_INFO(f, ...) do {} while(0)
  #define MU_LOG_WARNING(f, ...) do {} while(0)
  #define MU_LOG_ERROR(f, ...) do {} while(0)
  #define MU_LOG_CRITICAL(f, ...) do {} while(0)
#endif

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
typedef void (*mu_log_function_t)(mu_log_level_t severity, char *msg);

// =============================================================================
// declarations

/**
 * @brief Initialize the logging system.
 */
void mu_log_init();

/**
 * @brief Register a logging function.
 *
 * The given function is called for any logging messages equal to or above the
 * specified threshold.  If the function is already registered, this will change
 * the threshold.   Note that the most recently registered function will
 * be called last.
 *
 * @param fn The function to be called.  It receives two areguments: the
 * severity level and a string.
 * @param threshold Sets the minimum logging level: any logging message equal to
 * or above the threshold will trigger a call the the logging function.
 * @return MU_LOG_ERR_NONE on no error, MU_LOG_ERR_SUBSCRIBERS_EXCEEDED if there
 * are already MU_LOG_MAX_SUBSCRIBERS active subscribers.
 */
mu_log_err_t mu_log_subscribe(mu_log_function_t fn, mu_log_level_t threshold);

/**
 * @brief Un-register a logging function.
 *
 * The given function will no longer be called.
 *
 * @param fn The logging function.
 * @return MU_LOG_ERR_NONE on no error, MU_LOG_ERR_NOT_SUBSCRIBED if the
 * the logging function was not previously subscribed.
 */
mu_log_err_t mu_log_unsubscribe(mu_log_function_t fn);

/**
 * @brief Generate a log message.
 *
 * All subscribed logging functions whose threshold is at or below the given
 * severity will receive a string message as specified by the printf-like
 * format arguments.
 *
 * @param severity The severity level of the message.
 * @param fmt... printf-like arguments used to format the message.
 */
void mu_log_message(mu_log_level_t severity, const char *fmt, ...);

/**
 * @brief Get the log level as a string.
 *
 * This convenience function translates the given log level into a string.
 *
 * @param level The logging level
 * @eturn The logging level name as a string
 */
const char *mu_log_level_name(mu_log_level_t level);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_LOG_H_ */
