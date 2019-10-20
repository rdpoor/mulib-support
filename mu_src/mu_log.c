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
  mu_log_subscriber_t *subscriber_pool;
  int subscriber_pool_size;
  char *msg_buffer;
  int msg_buffer_size;
} mu_log_mgr_t;

// =============================================================================
// private declarations

// =============================================================================
// local storage

static mu_log_mgr_t s_log_mgr;

// =============================================================================
// public code

void mu_log_init(mu_log_subscriber_t *subscriber_pool,
                 int subscriber_pool_size,
                 char *msg_buffer,
                 int msg_buffer_size) {
  s_log_mgr.subscriber_pool = subscriber_pool;
  s_log_mgr.subscriber_pool_size = subscriber_pool_size;
  s_log_mgr.msg_buffer = msg_buffer;
  s_log_mgr.msg_buffer_size = msg_buffer_size;
  memset(subscriber_pool, 0, subscriber_pool_size * sizeof(mu_log_subscriber_t));
}

// search the s_log_mgr.subscriber_pool table to install or update fn
mu_log_err_t mu_log_subscribe(mu_log_subscriber_fn fn, mu_log_level_t threshold) {
  int available_slot = -1;
  for (int i=0; i<s_log_mgr.subscriber_pool_size; i++) {
    if (s_log_mgr.subscriber_pool[i].fn == fn) {
      // already subscribed: update threshold and return immediately.
      s_log_mgr.subscriber_pool[i].threshold = threshold;
      return MU_LOG_ERR_NONE;

    } else if (s_log_mgr.subscriber_pool[i].fn == NULL) {
      // found a free slot
      available_slot = i;
    }
  }
  // fn is not yet a subscriber.  assign if possible.
  if (available_slot == -1) {
    return MU_LOG_ERR_SUBSCRIBERS_EXCEEDED;
  }
  s_log_mgr.subscriber_pool[available_slot].fn = fn;
  s_log_mgr.subscriber_pool[available_slot].threshold = threshold;
  return MU_LOG_ERR_NONE;
}

// search the s_log_mgr.subscriber_pool table to remove
mu_log_err_t mu_log_unsubscribe(mu_log_subscriber_fn fn) {
  for (int i=0; i<s_log_mgr.subscriber_pool_size; i++) {
    if (s_log_mgr.subscriber_pool[i].fn == fn) {
      s_log_mgr.subscriber_pool[i].fn = NULL;    // mark as empty
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
  vsnprintf(s_log_mgr.msg_buffer, s_log_mgr.msg_buffer_size, fmt, ap);
  va_end(ap);

  for (int i=0; i<s_log_mgr.subscriber_pool_size; i++) {
    if (s_log_mgr.subscriber_pool[i].fn != NULL) {
      if (severity >= s_log_mgr.subscriber_pool[i].threshold) {
        s_log_mgr.subscriber_pool[i].fn(severity, s_log_mgr.msg_buffer);
      }
    }
  }
}

// =============================================================================
// private code

#endif  // #ifdef MU_LOG_ENABLED
