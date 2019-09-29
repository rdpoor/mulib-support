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

#include "mu_log.h"

#ifdef MU_LOG_ENABLED  // whole file...

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// =============================================================================
// private types and definitions

typedef struct {
  mu_log_subscriber_fn fn;
  mu_log_level_t threshold;
} subscriber_t;

// =============================================================================
// private declarations

// =============================================================================
// local storage

// TODO: implement user-allocated subscribers and message buffer
static subscriber_t s_subscribers[MU_LOG_MAX_SUBSCRIBERS];
static char s_message[MU_LOG_MAX_MESSAGE_LENGTH];

// =============================================================================
// public code

void mu_log_init() {
  memset(s_subscribers, 0, sizeof(s_subscribers));
}

// search the s_subscribers table to install or update fn
mu_log_err_t mu_log_subscribe(mu_log_subscriber_fn fn, mu_log_level_t threshold) {
  int available_slot = -1;
  for (int i=0; i<MU_LOG_MAX_SUBSCRIBERS; i++) {
    if (s_subscribers[i].fn == fn) {
      // already subscribed: update threshold and return immediately.
      s_subscribers[i].threshold = threshold;
      return MU_LOG_ERR_NONE;

    } else if (s_subscribers[i].fn == NULL) {
      // found a free slot
      available_slot = i;
    }
  }
  // fn is not yet a subscriber.  assign if possible.
  if (available_slot == -1) {
    return MU_LOG_ERR_SUBSCRIBERS_EXCEEDED;
  }
  s_subscribers[available_slot].fn = fn;
  s_subscribers[available_slot].threshold = threshold;
  return MU_LOG_ERR_NONE;
}

// search the s_subscribers table to remove
mu_log_err_t mu_log_unsubscribe(mu_log_subscriber_fn fn) {
  for (int i=0; i<MU_LOG_MAX_SUBSCRIBERS; i++) {
    if (s_subscribers[i].fn == fn) {
      s_subscribers[i].fn = NULL;    // mark as empty
      return MU_LOG_ERR_NONE;
    }
  }
  return MU_LOG_ERR_NOT_SUBSCRIBED;
}

const char *mu_log_level_name(mu_log_level_t severity) {
  switch(severity) {
   case MU_LOG_TRACE_LEVEL: return "TRACE";
   case MU_LOG_DEBUG_LEVEL: return "DEBUG";
   case MU_LOG_INFO_LEVEL: return "INFO";
   case MU_LOG_WARNING_LEVEL: return "WARNING";
   case MU_LOG_ERROR_LEVEL: return "ERROR";
   case MU_LOG_CRITICAL_LEVEL: return "CRITICAL";
   case MU_LOG_ALWAYS_LEVEL: return "ALWAYS";
   default: return "UNKNOWN";
  }
}

void mu_log_message(mu_log_level_t severity, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(s_message, MU_LOG_MAX_MESSAGE_LENGTH, fmt, ap);
  va_end(ap);

  for (int i=0; i<MU_LOG_MAX_SUBSCRIBERS; i++) {
    if (s_subscribers[i].fn != NULL) {
      if (severity >= s_subscribers[i].threshold) {
        s_subscribers[i].fn(severity, s_message);
      }
    }
  }
}

// =============================================================================
// private code

#endif  // #ifdef MU_LOG_ENABLED
