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

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "driver_init.h"
#include "mu_evt.h"
#include "mu_port.h"
#include "mu_sched.h"
#include "mu_sleep.h"
#include "mu_task.h"
#include "mu_time.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// types and definitions

// # of interrupts notifications we can handle between calls to sched_step()
// must be a power of two.
#define ISR_QUEUE_POOL_SIZE 8

// character sequence to home cursor and clear to end of screen
#define CLEAR_SCREEN "\e[1;1H\e[2J"

// Define repetition interval for toggling LED and updating display
#define LED_UPDATE_INTERVAL 0.5
#define MONITOR_UPDATE_INTERVAL 5.0

// =============================================================================
// forward declarations

/**
 * \brief Write string to console
 */
static void str_write(const char *s);

/**
 * \brief Read character from console
 */
static char char_read(void);

/**
 * \brief Return true if io_read() will not block.
 */
static bool char_is_available(void);

/**
 * \brief cheap float to ascii conversion
 */
static const char *cheap_ftoa(float x);

/**
 * \brief Right justify s to width chars wide, padding on left with padchar.
 */
static const char *rjust(const char *s, int width, char padchar);

/**
 * \brief Dump statistics to console
 */
static void _os_show_statistics(void);

/**
 * \brief Generate single character representing event status
 *
 * A = active (current)
 * I = inactive (not in sched)
 * P = pending (not yet runnable)
 * R = runnable (ready to run)
 */
static char event_status(mu_evt_t *evt);

/**
 * \brief Start or stop a repeating event
 * \param[in] enable Set to \c true to enable, \c false to disable
 */
static void enable_event(mu_evt_t *evt, bool enable);

/**
 * \brief mulib function called when the scheduler has nothing to run.
 */
static void idle_task_fn(void *self, void *arg);

/**
 * \brief mulib task that blinks LED
 */
static void led_task_fn(void *self, void *arg);

/**
 *  \brief mulib task to display system statistics every 5 seconds
 */
static void monitor_task_fn(void *self, void *arg);

/**
 * \brief mulib task to handle user input
 */
static void console_task_fn(void *self, void *arg);

/**
 * \brief interrupt-level callback when serial data is received.
 */
static void serial_rx_cb(const struct usart_async_descriptor *const descr);

/**
 * \brief interrupt-level callback when serial data is transmitted.
 */
static void serial_tx_cb(const struct usart_async_descriptor *const descr);

/**
 * \brief interrupt-level callback when error detected on serial port.
 */
static void serial_err_cb(const struct usart_async_descriptor *const descr);

// =============================================================================
// local storage

static struct io_descriptor *s_usart_io;

static mu_sched_t s_sched;
static mu_ring_obj_t s_isr_queue_pool[ISR_QUEUE_POOL_SIZE];
static mu_task_t s_idle_task;
static mu_evt_t s_led_event;
static mu_evt_t s_monitor_event;
static mu_evt_t s_console_event;
static bool s_sleep_on_idle;

static volatile bool s_output_pending; // set in fg, cleared in tx callback

// The only reason for keeping an array of events is so we can get the status
// of any events that are NOT in the scheduler (i.e. idle status).
static mu_evt_t *s_events[] = {&s_led_event,
                               &s_monitor_event,
                               &s_console_event};

static const int N_EVENTS = sizeof(s_events) / sizeof(mu_evt_t *);

// =============================================================================
// public code

int main(void) {
  // Perform board-specific initialization
  atmel_start_init();

  // Perform port-specific initialization needed by mulib (RTC)
  mu_port_init();

  // Set up the serial I/O system
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, serial_tx_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, serial_rx_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, serial_err_cb);
  usart_async_get_io_descriptor(&EDBG_COM, &s_usart_io);
  usart_async_enable(&EDBG_COM);

  s_output_pending = false;

  s_sleep_on_idle = false;

  // Initialize the scheduler
  mu_sched_init(&s_sched, s_isr_queue_pool, ISR_QUEUE_POOL_SIZE);

  // Initialize and install the idle task in the scheduler.
  mu_task_init(&s_idle_task, idle_task_fn, NULL, "Idle");
  mu_sched_set_idle_task(&s_sched, &s_idle_task);

  mu_time_t now = mu_time_now();

  // Queue initial events
  mu_sched_queue(&s_sched,
                 mu_evt_init_at(&s_led_event, now, led_task_fn, NULL, "LED"));
  mu_sched_queue(
      &s_sched,
      mu_evt_init_at(&s_monitor_event, now, monitor_task_fn, NULL, "Monitor"));
  mu_sched_queue(
      &s_sched,
      mu_evt_init_immed(&s_console_event, console_task_fn, NULL, "Console"));

  // Run the scheduler
  while (1) {
    mu_sched_step(&s_sched);
  }
  return 0; // Like Charlie on the MTA (no he never returned...)
}

// =============================================================================
// private code

/**
 * \brief Write string to console
 */
static void str_write(const char *s) {
  s_output_pending = true;
  int len = strlen(s);
  io_write(s_usart_io, (const uint8_t *)s, len);
  // wait until previous output completes
  while (s_output_pending) {
    asm("nop");
  }
}

/**
 * \brief Read character from console
 */
static char char_read(void) {
  uint8_t tmp;
  while (ERR_TIMEOUT == io_read(&EDBG_COM.io, &tmp, 1)) {
    // We always call char_is_available() before calling char_read() so we
    // should never arrive here.
    asm("nop");
  }
  return (char)tmp;
}

/**
 * \brief Return true if io_read() will not block.
 */
static bool char_is_available(void) {
  return ringbuffer_num(&EDBG_COM.rx) != 0;
}

// print six digits to the right of the decimal point
#define FTOA_PRECISION 1000000
/**
 * \brief cheap float to ascii conversion
 *
 * Restrictions:
 * - output is written to a static buffer.  The caller must copy the contents
 *   to somewhere before the next call to cheap_ftoa()
 * - doesn't handle negative numbers, but it's easy to add if your application
 *   requires that.
 * - Results with anything that overflows an int are undefined.
 * - Results with Nan and Inf are undefined
 */
static const char *cheap_ftoa(float x) {
  static char buf[20];
  static char fbuf[7];                           // six digits plus null
  int c = (x * FTOA_PRECISION) + 0.5;            // handle rounding
  int d = c / FTOA_PRECISION;                    // the integer part
  int f = c - (d * FTOA_PRECISION);              // the fractional part
  sprintf(fbuf, "%u", f);                        // fractional part as ascii
  sprintf(buf, "%u.%s", d, rjust(fbuf, 6, '0')); // pad with leading '0'
  return buf;
}

/**
 * /brief Right justify s to width chars wide, padding on left with spaces.
 */
static const char *rjust(const char *s, int width, char padchar) {
  static char buf[32];
  int padding = width - strlen(s);
  if (padding <= 0)
    return s;

  memset(buf, padchar, padding);
  strcpy(&buf[padding], s);
  return buf;
}

/**
 * \brief Dump statistics to console
 */
static void _os_show_statistics(void) {
  mu_evt_t *evt;
  static char buf[128];

  str_write(CLEAR_SCREEN);
  str_write("\r\n>     Task: State    At     Calls  MaxLatency     Runtime");
  str_write("\r\n*********************************************************");

  for (int i = 0; i < N_EVENTS; i++) {
    evt = s_events[i];
    mu_task_t *task = &evt->task;
    sprintf(buf,
            "\r\n%s: %c",
            rjust(mu_task_name(task), 10, ' '),
            event_status(evt));
    str_write(buf);
    if (mu_evt_is_immediate(evt)) {
      str_write(rjust("immediate", 10, ' '));
    } else {
      sprintf(buf, "%u", (int)mu_evt_time(evt));
      str_write(rjust(buf, 10, ' '));
    }
    sprintf(buf, " %u", mu_task_call_count(task));
    str_write(rjust(buf, 10, ' '));
    sprintf(buf, " %s", cheap_ftoa(mu_task_max_latency(task)));
    str_write(rjust(buf, 12, ' '));
    sprintf(buf, " %s", cheap_ftoa(mu_task_runtime(task)));
    str_write(rjust(buf, 12, ' '));
  }

  if (s_sleep_on_idle) {
    str_write("\r\n--- Low-power idle mode.  Press any key to wake: ");
  } else {
    str_write("\r\n--- Full-power mode. Select one:"
              "\r\n b: activate blink task"
              "\r\n B: suspend blink task"
              "\r\n d: activate display task"
              "\r\n D: suspend display task"
              "\r\n p: enter low-power idle mode"
              "\r\n others: Echo back"
              "\r\n# ");
  }
}

/*
 * A = active (current)
 * I = inactive (not in sched)
 * P = pending (not yet runnable)
 * R = runnable (ready to run)
 */
static char event_status(mu_evt_t *evt) {
  if (mu_sched_current_event(&s_sched) == evt) {
    return 'A'; // this event is currently running
  } else if (!mu_sched_has_event(&s_sched, evt)) {
    return 'I'; // this event is not scheduled
  } else if (mu_evt_is_runnable(evt, mu_time_now())) {
    return 'R'; // this event is ready to run
  } else {
    return 'P'; // this event is in the schedule but its time hasn't arrived
  }
}

static void enable_event(mu_evt_t *evt, bool enable) {
  if (!enable) {
    // remove event from schedule (if it's in the schedule)
    mu_sched_remove(&s_sched, evt);
  } else if (!mu_sched_has_event(&s_sched, evt)) {
    // Event is not scheduled -- enable it now.
    mu_time_t now = mu_time_now();
    mu_sched_queue(&s_sched, mu_evt_set_time(evt, now));
  } else {
    // already in schedule
  }
}

static void led_task_fn(void *self, void *arg) {
  (void)self;
  (void)arg;
  gpio_toggle_pin_level(LED0);
  // run again in 500 mSec
  mu_evt_offset_time(&s_led_event,
                     mu_time_seconds_to_duration(LED_UPDATE_INTERVAL));
  mu_sched_queue(&s_sched, &s_led_event);
}

static void monitor_task_fn(void *self, void *arg) {
  (void)self;
  (void)arg;
  _os_show_statistics();
  // run again in 5 sec
  mu_evt_offset_time(&s_monitor_event,
                     mu_time_seconds_to_duration(MONITOR_UPDATE_INTERVAL));
  mu_sched_queue(&s_sched, &s_monitor_event);
}

/**
 * mulib task to monitor console inputs and do actions
 */
static void console_task_fn(void *self, void *arg) {
  (void)self;
  (void)arg;

  if (s_sleep_on_idle) {
    // We received a char whiile in s_sleep_on_idle mode.  Exit the mode and
    // slurp up any chars that arrived -- they're likely garbage chars.
    s_sleep_on_idle = false;
    while (char_is_available()) {
      char_read();
    }
    return;
  }

  while (char_is_available()) {
    char *str = "";
    char ch[2] = {0, 0};
    ch[0] = char_read();
    switch (ch[0]) {
    case 'b':
      enable_event(&s_led_event, true);
      str = "- Blink task is active.\r\n";
      break;
    case 'B':
      enable_event(&s_led_event, false);
      str = "- Blink task is suspended.\r\n";
      gpio_set_pin_level(LED0, true);
      break;
    case 'd':
      enable_event(&s_monitor_event, true);
      str = "- Display task is active.\r\n";
      break;
    case 'D':
      enable_event(&s_monitor_event, false);
      str = "- Display task is suspended.\r\n";
      break;
    case 'p':
      s_sleep_on_idle = true;
      str = "- Entering low-power idle mode.\r\n";
      break;
    default:
      str = ch;
      break;
    }

    str_write(str);
  }
}

// Arrive here when the scheduler has nothing runnable.  Three things to note:
// - all immediate tasks will have been processed (b/c they're always runnable)
// - if the event list is empty, there are no scheduled events -- sleep until
//   some external interrupt
// - if the event list is not empty, the first event is the soonest.  sleep
//   until that time arrives.
static void idle_task_fn(void *self, void *arg) {
  (void)self;
  // don't sleep if there's serial output pending.
  if (s_output_pending) {
    return;
  }
  // don't sleep on idle unless enabled
  if (!s_sleep_on_idle) {
    return;
  }

  // peek at the next event to find the desired wake time.
  mu_sched_t *sched = (mu_sched_t *)arg;
  mu_evt_t *next_event = mu_sched_get_events(sched);

  // TODO: bcast will sleep
  if (next_event == NULL) {
    mu_sleep_indefinitely();
  } else {
    mu_sleep_until(mu_evt_time(next_event));
  }
  // TODO: bcast did wake
  asm("nop");
}

static void serial_rx_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  // A character was received.  Trigger the console task.
  mu_sched_queue_from_isr(&s_sched, &s_console_event);
}

static void serial_tx_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  s_output_pending = false;
  asm("nop");
}

static void serial_err_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  asm("nop");
}

// Adopted from the FreeRTOS demo code: a smaller sprintf()
#define _SPRINTF_OVERRIDE 1
#if _SPRINTF_OVERRIDE
/* Override sprintf implement to optimize */

static const unsigned m_val[] = {1000000000u,
                                 100000000u,
                                 10000000u,
                                 1000000u,
                                 100000u,
                                 10000u,
                                 1000u,
                                 100u,
                                 10u,
                                 1u};
int sprintu(char *s, unsigned u) {
  char tmp_buf[12];
  int i, n = 0;
  int m;

  if (u == 0) {
    *s = '0';
    return 1;
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      if (u >= m_val[i]) {
        u -= m_val[i];
      } else {
        break;
      }
    }
    tmp_buf[i] = m + '0';
  }
  for (i = 0; i < 10; i++) {
    if (tmp_buf[i] != '0') {
      break;
    }
  }
  for (; i < 10; i++) {
    *s++ = tmp_buf[i];
    n++;
  }
  return n;
}

int sprintf(char *s, const char *fmt, ...) {
  int n = 0;
  va_list ap;
  va_start(ap, fmt);
  while (*fmt) {
    if (*fmt != '%') {
      *s = *fmt;
      s++;
      fmt++;
      n++;
    } else {
      fmt++;
      switch (*fmt) {
      case 'c': {
        char valch = va_arg(ap, int);
        *s = valch;
        s++;
        fmt++;
        n++;
        break;
      }
      case 'd': {
        int vali = va_arg(ap, int);
        int nc;

        if (vali < 0) {
          *s++ = '-';
          n++;
          nc = sprintu(s, -vali);
        } else {
          nc = sprintu(s, vali);
        }

        s += nc;
        n += nc;
        fmt++;
        break;
      }
      case 'u': {
        unsigned valu = va_arg(ap, unsigned);
        int nc = sprintu(s, valu);
        n += nc;
        s += nc;
        fmt++;
        break;
      }
      case 's': {
        char *vals = va_arg(ap, char *);
        while (*vals) {
          *s = *vals;
          s++;
          vals++;
          n++;
        }
        fmt++;
        break;
      }
      default:
        *s = *fmt;
        s++;
        fmt++;
        n++;
      }
    }
  }
  va_end(ap);
  *s = 0;
  return n;
}
#endif /* _SPRINTF_OVERRIDE */
