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

/**
 * @file mu_port.c
 *
 * Reference implementation of mu_port.c for the SAMD21 running under Atmel
 * Studio 7 / Atmel START / ASF4.
 *
 * Note: this implementation assumes the Atmel START project specifies the HAL
 * Async serial driver.
 */

 /**
Serial Notes:

We break a few levels of abstraction to implement the mu_port serial interface.

Note the distinction between descr.usart_cb which is a
`struct usart_async_callbacks` for the user level callbacks, and
descr.device.usart_cb, which is a `struct _usart_async_callbacks`
and is used for driver level callbacks.  It's this one that we
need to modify.

USART_0 is a `struct usart_async_descriptor`:

struct usart_async_descriptor {
	struct io_descriptor         io;           // _read, _write fns: not used yet
	struct _usart_async_device   device;       // used -- see below
	struct usart_async_callbacks usart_cb;     // user-level callbacks
	uint32_t                     stat;         // not used

	struct ringbuffer rx;                      // not used
	uint16_t          tx_por;                  // not used
	uint8_t *         tx_buffer;               // not used
	uint16_t          tx_buffer_length;        // not used
};

struct _usart_async_device {
	struct _usart_async_callbacks usart_cb;    // used -- see below
	struct _irq_descriptor        irq;         // not clear
	void *                        hw;          // SERCOM3 (0x42001400)
};

USART_0.device.usart_cb is a struct of four function pointers:

struct _usart_async_callbacks {
	void (*tx_byte_sent)(struct _usart_async_device *device);
	void (*rx_done_cb)(struct _usart_async_device *device, uint8_t data);
	void (*tx_done_cb)(struct _usart_async_device *device);
	void (*error_cb)(struct _usart_async_device *device);
};

After the system calls USART_0_Init(), but before we use USART_0, we modify
USART_0.device.usart_cb to point at our own interrupt callbacks.

NEED TO UNDERSTAND:

INTEN_TXC ("tx_done") means the byte has been completely transmitted.
INTEN_DRE ("byte_sent") means the tx register is ready to accept a char.
INTEN_RXC

USART_0_Init() called from system_init():

  USART_0_CLOCK_init();
  usart_async_init(&USART_0, SERCOM3, USART_0_buffer, USART_0_BUFFER_SIZE, (void *)NULL);
  USART_0_PORT_init();

usart_async_init called from USART_0_init():

  int32_t init_status;
  ASSERT(descr && hw && rx_buffer && rx_buffer_length);

  if (ERR_NONE != ringbuffer_init(&descr->rx, rx_buffer, rx_buffer_length)) {
    return ERR_INVALID_ARG;
  }
  init_status = _usart_async_init(&descr->device, hw);
  if (init_status) {
    return init_status;
  }

  descr->io.read  = usart_async_read;
  descr->io.write = usart_async_write;

  descr->device.usart_cb.tx_byte_sent = usart_process_byte_sent;
  descr->device.usart_cb.rx_done_cb   = usart_fill_rx_buffer;
  descr->device.usart_cb.tx_done_cb   = usart_transmission_complete;
  descr->device.usart_cb.error_cb     = usart_error;

  return ERR_NONE;

_usart_async_init called from usart_async_init:

int32_t _usart_async_init(struct _usart_async_device *const device, void *const hw) {
	int32_t init_status;

	ASSERT(device);

	init_status = _usart_init(hw);
	if (init_status) {
		return init_status;
	}
	device->hw = hw;
	_sercom_init_irq_param(hw, (void *)device);
	NVIC_DisableIRQ((IRQn_Type)_sercom_get_irq_num(hw));
	NVIC_ClearPendingIRQ((IRQn_Type)_sercom_get_irq_num(hw));
	NVIC_EnableIRQ((IRQn_Type)_sercom_get_irq_num(hw));

	return ERR_NONE;
}

_usart_init called from _usart_async_init:

_sercom_init_irq_param called from _usart_async_init:

static void _sercom_init_irq_param(const void *const hw, void *dev) {
	if (hw == SERCOM3) {
		_sercom3_dev = (struct _usart_async_device *)dev;
	}
}


_usart_async_enable called from usart_async_enable:

void _usart_async_enable(struct _usart_async_device *const device) {
	hri_sercomusart_set_CTRLA_ENABLE_bit(device->hw);
}


  */

// =============================================================================
// includes

#include "mu_port.h"
#include "atmel_start.h"
#include "peripheral_clk_config.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h> // memset

// =============================================================================
// private types and definitions

#define MAX_DURATION ((uint32_t)0x7fffffff)

#define MU_PORT_TIME_MS_TO_DURATION(ms) (((ms) * CONF_GCLK_RTC_FREQUENCY) / 1000)

// don't sleep for less than 1ms (32 RTC tics)
#define MIN_SLEEP_DURATION MU_PORT_TIME_MS_TO_DURATION(1)

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

#ifdef MU_PORT_FLOAT
MU_PORT_FLOAT s_rtc_period; // 1.0/RTC_FREQUENCY
#endif

static struct io_descriptor *s_usart_descriptor;

static volatile bool s_tx_in_progress;

typedef struct {
  void (*button_cb)(void *arg);
  void *button_cb_arg;
  void (*tx_cb)(void *arg);
  void *tx_cb_arg;
  void (*rx_cb)(void *arg);
  void *rx_cb_arg;
  void (*rtc_cb)(void *arg);
  void *rtc_cb_arg;
  uint8_t *rx_buf;       // destination for incoming serial data
  size_t rx_buf_size;    // size of rx_buf
  size_t rx_buf_count;   // # of bytes in rx_buf
} port_t;

static port_t s_port;

// =============================================================================
// public code

void mu_port_init(void) {
#ifdef MU_PORT_FLOAT
  s_rtc_period = 1.0 / (MU_PORT_FLOAT)CONF_GCLK_RTC_FREQUENCY;
#endif
  memset(&s_port, 0, sizeof(s_port));

  ext_irq_register(PIN_PA15, button_cb_trampoline);

  s_tx_in_progress = false;
  usart_async_get_io_descriptor(&USART_0, &s_usart_descriptor);
  usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, tx_cb_trampoline);
  usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb_trampoline);
  // usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, err_cb);
  usart_async_enable(&USART_0);

  // Initialize the RTC.  Use CALENDAR_0 since that's the only published
  // interface for interacting with the underlying RTC.
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

bool mu_port_time_equals(mu_port_time_t t1, mu_port_time_t t2) {
  return t1 == t2;
}

bool mu_port_time_follows(mu_port_time_t t1, mu_port_time_t t2) {
  return mu_port_time_difference(t2, t1) > MAX_DURATION;
}

mu_port_time_dt mu_port_time_ms_to_duration(mu_port_time_ms_dt ms) {
  return quo_rounded(ms * CONF_GCLK_RTC_FREQUENCY, 1000);
}

mu_port_time_ms_dt mu_port_time_duration_to_ms(mu_port_time_dt dt) {
  return quo_rounded(dt * 1000, CONF_GCLK_RTC_FREQUENCY);
}

#ifdef MU_PORT_FLOAT
mu_port_time_seconds_dt mu_port_time_duration_to_seconds(mu_port_time_dt dt) {
  return dt * s_rtc_period;
}

mu_port_time_dt mu_port_time_seconds_to_duration(mu_port_time_seconds_dt seconds) {
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
    s_port.rtc_cb = fn;
    s_port.rtc_cb_arg = arg;
  } else {
    s_port.rtc_cb = NULL;
    s_port.rtc_cb_arg = NULL;
  }
}

void mu_port_rtc_alarm_at(mu_port_time_t at) {
  _calendar_set_comp(&CALENDAR_0.device, at);
  // Particular to the SAMD21, hri_rtcmode0_write_COMP_COMP_bf() clears the
  // READREQ_RCONT bit, which prevents subsequent RTC reads from updating.
  // Restore it here.
  hri_rtcmode0_set_READREQ_RCONT_bit(RTC);
  hri_rtcmode0_set_READREQ_RREQ_bit(RTC);
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
    s_port.button_cb = fn;
    s_port.button_cb_arg = arg;
  } else {
    s_port.button_cb = NULL;
    s_port.button_cb_arg = NULL;
  }
}

// SERIAL

bool mu_port_serial_write(const uint8_t *const buf, int n_bytes) {
  s_tx_in_progress = true;
  if (io_write(s_usart_descriptor, buf, n_bytes) < 0) {
    // previous operation not yet completed
    return false;
  } else {
    return true;
  }
}

int mu_port_serial_write_count(void) {
  struct usart_async_status status;
  usart_async_get_status(&USART_0, &status);
  return status.txcnt;
}

bool mu_port_serial_can_write(void) {
  struct usart_async_status status;
  usart_async_get_status(&USART_0, &status);
  return status.flags == 0;
}

void mu_port_serial_set_write_cb(mu_port_callback_fn fn, void *arg) {
  if (fn) {
    s_port.tx_cb = fn;
    s_port.tx_cb_arg = arg;
  } else {
    s_port.tx_cb = NULL;
    s_port.tx_cb_arg = NULL;
  }
}

/**
 * Implementation note: Because ASF4 provides byte-at-a-time callback, we do
 * the following:
 * - initiate a single byte read request
 * - upon notification of a byte, call io_read to fetch the byte and copy it
 *   into the user-supplied buffer.
 * - if n_bytes have been copied, call the user-supplied callback.
 */
bool mu_port_serial_read(uint8_t *const buf, int n_bytes) {
  s_port.rx_buf = buf;
  s_port.rx_buf_size = n_bytes;
  s_port.rx_buf_count = 0;
  io_read(s_usart_descriptor, buf, 1);  // initiate one-byte read
  return true;
}

bool mu_port_serial_can_read(void) {
  return usart_async_is_rx_not_empty(&USART_0);
}

int mu_port_serial_read_count(void) {
  return s_port.rx_buf_count;
}

void mu_port_serial_set_read_cb(mu_port_callback_fn fn, void *arg) {
  if (fn) {
    s_port.rx_cb = fn;
    s_port.rx_cb_arg = arg;
  } else {
    s_port.rx_cb = NULL;
    s_port.rx_cb_arg = NULL;
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
  if (s_port.button_cb) {
    s_port.button_cb(s_port.button_cb_arg);
  }
}

void tx_cb_trampoline(const struct usart_async_descriptor *const io_descr) {
  // arrive here when the previous call to mu_port_serial_write() completes.
  s_tx_in_progress = false;
  if (s_port.tx_cb) {
    s_port.tx_cb(s_port.tx_cb_arg);
  }
}

void rx_cb_trampoline(const struct usart_async_descriptor *const io_descr) {
  // arrive here whenever a character is received on the serial port.
  if (s_port.rx_buf_count == s_port.rx_buf_size) {
    // rx buffer is full -- discard char
    return;
  }
  // copy new char into rx_buff
  io_read(s_usart_descriptor, &s_port.rx_buf[s_port.rx_buf_count++], 1);
  if (s_port.rx_buf_count == s_port.rx_buf_size) {
    // n_chars have been read.  invoke user callback
    if (s_port.rx_cb) {
      s_port.rx_cb(s_port.rx_cb_arg);
    }
  }
}

void rtc_cb_trampoline(struct calendar_dev *const dev) {
  // Arrive here when the RTC count register matches the RTC compare register.
  // Even if the user hasn't registered a callback, this will wake the processor
  // from sleep...
  if (s_port.rtc_cb) {
    s_port.rtc_cb(s_port.rtc_cb_arg);
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
