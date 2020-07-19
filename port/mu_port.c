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

#include "mu_port.h"
#include "atmel_start.h"
#include "peripheral_clk_config.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// =============================================================================
// private types and definitions

#define MAX_DURATION ((uint32_t)0x7fffffff)

#define MU_TIME_MS_TO_DURATION(ms) (((ms) * CONF_GCLK_RTC_FREQUENCY) / 1000)

// don't sleep for less than 1ms (32 RTC tics)
#define MIN_SLEEP_DURATION MU_TIME_MS_TO_DURATION(1)

// =============================================================================
// private declarations

static void button_cb_trampoline(void);
static void tx_cb_trampoline(const struct usart_async_descriptor *const io_descr);
static void rx_cb_trampoline(const struct usart_async_descriptor *const io_descr);
static void rtc_cb_trampoline(struct calendar_dev *const dev);
static void go_to_sleep(void);
static int quo_rounded(int x, int y);

// =============================================================================
// local storage

// TODO: package up in a struct to memset can clear all at initialization

#ifdef PORT_FLOAT
PORT_FLOAT s_rtc_period; // 1.0/RTC_FREQUENCY
#endif

static void (*s_button_cb)(void *arg);
static void *s_button_cb_arg;

static struct io_descriptor *s_usart_descriptor;

static void (*s_tx_cb)(void *arg);
static void *s_tx_cb_arg;

static void (*s_rx_cb)(void *arg);
static void *s_rx_cb_arg;

static void (*s_rtc_cb)(void *arg);
static void *s_rtc_cb_arg;

static volatile bool s_tx_in_progress;

// =============================================================================
// public code

void mu_port_init(void) {
#ifdef PORT_FLOAT
  s_rtc_period = 1.0 / (PORT_FLOAT)CONF_GCLK_RTC_FREQUENCY;
#endif
  s_button_cb = NULL;
  s_button_cb_arg = NULL;
  ext_irq_register(PIN_PA15, button_cb_trampoline);

  usart_async_get_io_descriptor(&USART_0, &s_usart_descriptor);
  s_tx_cb = NULL;
  s_tx_cb_arg = NULL;
  s_tx_in_progress = false;
  usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, tx_cb_trampoline);
  s_rx_cb = NULL;
  s_rx_cb_arg = NULL;
  usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb_trampoline);
  // usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, err_cb);
  usart_async_enable(&USART_0);

  // Initialize the RTC.  Use CALENDAR_0 since that's the only published
  // interface for interacting with the underlying RTC.
  s_rtc_cb = NULL;
  s_rtc_cb_arg = NULL;
  calendar_enable(&CALENDAR_0); // start RTC
  _calendar_register_callback(&CALENDAR_0.device, rtc_cb_trampoline);
}

// TIME

mu_port_time_t mu_port_time_offset(mu_port_time_t t, mu_port_time_dt dt) {
  return t + dt;
}

mu_port_time_dt mu_port_time_difference(mu_port_time_t t1, mu_port_time_t t2) {
  return t1 - t2;
}

bool mu_port_time_precedes(mu_port_time_t t1, mu_port_time_t t2) {
  return mu_port_time_difference(t1, t2) > MAX_DURATION;
}

bool mu_port_time_is_equal(mu_port_time_t t1, mu_port_time_t t2) {
  return t1 == t2;
}

bool mu_port_time_follows(mu_port_time_t t1, mu_port_time_t t2) {
  return mu_port_time_difference(t2, t1) > MAX_DURATION;
}

mu_port_time_dt mu_port_time_ms_to_duration(int ms) {
  return quo_rounded(ms * CONF_GCLK_RTC_FREQUENCY, 1000);
}

int mu_port_time_duration_to_ms(mu_port_time_dt dt) {
  return quo_rounded(dt * 1000, CONF_GCLK_RTC_FREQUENCY);
}

#ifdef PORT_FLOAT
PORT_FLOAT mu_port_time_duration_to_seconds(mu_port_time_dt dt) {
  return dt * s_rtc_period;
}

mu_port_time_dt mu_port_time_seconds_to_duration(PORT_FLOAT seconds) {
  return seconds / s_rtc_period;
}
#endif

// REAL TIME CLOCK

mu_port_time_t mu_port_rtc_now(void) {
  return hri_rtcmode0_read_COUNT_COUNT_bf(CALENDAR_0.device.hw);
}

/**
 * Register a callback to be called when rtc matches.
 */
void mu_port_rtc_set_cb(mu_port_callback_fn fn, void *arg) {
  if (fn) {
    s_rtc_cb = fn;
    s_rtc_cb_arg = arg;
  } else {
    s_rtc_cb = NULL;
    s_rtc_cb_arg = NULL;
  }
}

void mu_port_rtc_alarm_at(mu_port_time_t at) {
  _calendar_set_comp(&CALENDAR_0.device, at);
  // For reasons unknown, hri_rtcmode0_write_COMP_COMP_bf() clears READREQ_RCONT
  // which prevents subsequent RTC reads from updating.  Fix it here.
  // See
  // https://community.atmel.com/forum/samd21-setting-rtc-comp-clears-readreqrcont
  hri_rtcmode0_set_READREQ_RCONT_bit(RTC);
}

// LED

void mu_port_led_set(bool on) { gpio_set_pin_level(USER_LED_AL, !on); }

bool mu_port_led_get(void) { return !gpio_get_pin_level(USER_LED_AL); }

// BUTTON

bool mu_port_button_is_pressed(void) {
  return !gpio_get_pin_level(USER_BUTTON_AL);
}

void mu_port_button_set_cb(mu_port_callback_fn fn, void *arg) {
  if (fn) {
    s_button_cb = fn;
    s_button_cb_arg = arg;
  } else {
    s_button_cb = NULL;
    s_button_cb_arg = NULL;
  }
}

// SERIAL

/**
 * @brief Return true if a call to mu_port_serial_write() would accept at least
 * one character.
 */
bool mu_port_serial_can_write(void) {
  return !s_tx_in_progress;
}

/**
 * @brief Non-blocking write to the serial port.
 *
 * Write up to n_bytes on the serial port.  Non-blocking, returns the number of
 * bytes written (which may be zero) or a negative number indicating an error.
 */
int mu_port_serial_write(const uint8_t *const buf, int n_bytes) {
  s_tx_in_progress = true;
  return io_write(s_usart_descriptor, buf, n_bytes);
}

/**
 * Register a callback to be called when characters may be written to the
 * serial port.
 */
void mu_port_serial_set_write_cb(mu_port_callback_fn fn, void *arg) {
  if (fn) {
    s_tx_cb = fn;
    s_tx_cb_arg = arg;
  } else {
    s_tx_cb = NULL;
    s_tx_cb_arg = NULL;
  }
}

/**
 * @brief Return true if a call to mu_port_serial_read() would return at least
 * one character.
 */
bool mu_port_serial_can_read(void) {
  return usart_async_is_rx_not_empty(&USART_0);
}

/**
 * @brief Non-blocking read from the serial port.
 *
 * Read up to n_bytes from the serial port.  Returns the number of bytes read,
 * which may be zero if no bytes available or negative on an error.
 */
int mu_port_serial_read(uint8_t *const buf, int n_bytes) {
  return io_read(s_usart_descriptor, buf, n_bytes);
}

/**
 * Register a callback to be called when characters become available for read.
 */
void mu_port_serial_set_read_cb(mu_port_callback_fn fn, void *arg) {
  if (fn) {
    s_rx_cb = fn;
    s_rx_cb_arg = arg;
  } else {
    s_rx_cb = NULL;
    s_rx_cb_arg = NULL;
  }
}

// SLEEP

/**
 * @brief Put the processor into low-power mode until time t arrives, or an
 * external event wakes the processor.
 */
void mu_port_sleep_until(mu_port_time_t then) {
  mu_port_time_t now = mu_port_rtc_now();
  if (mu_port_time_difference(then, now) > MIN_SLEEP_DURATION) {
    mu_port_rtc_alarm_at(then);
    go_to_sleep();
  }
}

/**
 * @brief Put the processor into low-power mode until an external event wakes
 * the processor.
 */
void mu_port_sleep(void) { go_to_sleep(); }

// =============================================================================
// private (local) code

void button_cb_trampoline(void) {
  if (s_button_cb) {
    s_button_cb(s_button_cb_arg);
  }
}

void tx_cb_trampoline(const struct usart_async_descriptor *const io_descr) {
  // arrive here when the previous call to serial_write() completes.
  s_tx_in_progress = false;
  if (s_tx_cb) {
    s_tx_cb(s_tx_cb_arg);
  }
}

void rx_cb_trampoline(const struct usart_async_descriptor *const io_descr) {
  if (s_rx_cb) {
    s_rx_cb(s_rx_cb_arg);
  }
}

void rtc_cb_trampoline(struct calendar_dev *const dev) {
  // Arrive here when the RTC count register matches the RTC compare register.
  // Even if the user hasn't registered a callback, this will wake the processor
  // from sleep...
  if (s_rtc_cb) {
    s_rtc_cb(s_rtc_cb_arg);
  }
}

static void go_to_sleep(void) {
  sleep(3); // in hal_sleep
}

// See https://stackoverflow.com/a/18067292/558639
//
static int quo_rounded(int x, int y) {
  // What does it all mean?
  //   (x < 0) is false (zero) if x is non-negative
  //   (y < 0) is false (zero) if x is non-negative
  //   (x < 0) ^ (y < 0) is true if x and y have opposite signs
  //   x/y would be the quotient, but it is truncated towards zero.  To round:
  //   (x + y/2)/y is the rounded quotient when x and y have the same sign
  //   (x - y/2)/y is the rounded quotient when x and y have opposite signs
  return ((x < 0) ^ (y < 0)) ? ((x - y/2)/y) : ((x + y/2)/y);
}
