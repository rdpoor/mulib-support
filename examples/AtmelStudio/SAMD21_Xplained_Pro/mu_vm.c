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
 * @file mu_vm.c
 *
 * Reference implementation of mu_vm.c for the SAMD21 running under Atmel
 * Studio 7 / Atmel START / ASF4.
 *
 * Note: this implementation assumes the Atmel START project specifies the HAL
 * Async serial driver.
 */

 /**
Serial Notes:

We break a few levels of abstraction to implement the mu_vm serial interface.

After the system calls USART_0_Init(), but before we use USART_0, we modify
USART_0.device.usart_cb to point at our own interrupt callbacks.

  */

// =============================================================================
// includes

#include "mu_vm.h"
#include "atmel_start.h"
#include "peripheral_clk_config.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h> // memset

// =============================================================================
// private types and definitions

#define MAX_DURATION ((uint32_t)0x7fffffff)

#define MU_VM_TIME_MS_TO_DURATION(ms) (((ms) * CONF_GCLK_RTC_FREQUENCY) / 1000)

// don't sleep for less than 1ms (32 RTC tics)
#define MIN_SLEEP_DURATION MU_VM_TIME_MS_TO_DURATION(1)

// =============================================================================
// private declarations

static void button_cb_trampoline(void);
static void customize_usart_driver(void);
static void tx_byte_sent(struct _usart_async_device *device);
static void rx_done_cb(struct _usart_async_device *device, uint8_t data);
static void tx_done_cb(struct _usart_async_device *device);
static void error_cb(struct _usart_async_device *device);
static void rtc_cb_trampoline(struct calendar_dev *const dev);
static void go_to_sleep(void);
static int quo_rounded(int x, int y);

// =============================================================================
// local storage

#ifdef MU_VM_FLOAT
MU_VM_FLOAT s_rtc_period; // 1.0/RTC_FREQUENCY
#endif

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
} port_t;

static port_t s_port;

static volatile uint8_t s_rx_data;
static volatile bool s_rx_has_data;

// =============================================================================
// public code

void mu_vm_init(void) {
#ifdef MU_VM_FLOAT
  s_rtc_period = 1.0 / (MU_VM_FLOAT)CONF_GCLK_RTC_FREQUENCY;
#endif
  memset(&s_port, 0, sizeof(s_port));

  ext_irq_register(PIN_PA15, button_cb_trampoline);

  // Commandeer the USART driver to do our bidding...
  customize_usart_driver();
  usart_async_enable(&USART_0);

  // Initialize the RTC.  Use CALENDAR_0 since that's the only published
  // interface for setting up the RTC compare callback.
  calendar_enable(&CALENDAR_0); // start RTC
  _calendar_register_callback(&CALENDAR_0.device, rtc_cb_trampoline);
}

// ==========
// TIME

mu_vm_time_t mu_vm_time_offset(mu_vm_time_t t, mu_vm_time_dt dt) {
  return t + dt;
}

mu_vm_time_dt mu_vm_time_difference(mu_vm_time_t t1, mu_vm_time_t t2) {
  return t1 - t2;
}

bool mu_vm_time_precedes(mu_vm_time_t t1, mu_vm_time_t t2) {
  return mu_vm_time_difference(t1, t2) > MAX_DURATION;
}

bool mu_vm_time_equals(mu_vm_time_t t1, mu_vm_time_t t2) {
  return t1 == t2;
}

bool mu_vm_time_follows(mu_vm_time_t t1, mu_vm_time_t t2) {
  return mu_vm_time_difference(t2, t1) > MAX_DURATION;
}

mu_vm_time_dt mu_vm_time_ms_to_duration(mu_vm_time_ms_dt ms) {
  return quo_rounded(ms * CONF_GCLK_RTC_FREQUENCY, 1000);
}

mu_vm_time_ms_dt mu_vm_time_duration_to_ms(mu_vm_time_dt dt) {
  return quo_rounded(dt * 1000, CONF_GCLK_RTC_FREQUENCY);
}

#ifdef MU_VM_FLOAT
mu_vm_time_s_dt mu_vm_time_duration_to_s(mu_vm_time_dt dt) {
  return dt * s_rtc_period;
}

mu_vm_time_dt mu_vm_time_s_to_duration(mu_vm_time_s_dt seconds) {
  return seconds / s_rtc_period;
}
#endif

// ==========
// REAL TIME CLOCK

mu_vm_time_t mu_vm_rtc_now(void) {
  return hri_rtcmode0_read_COUNT_COUNT_bf(RTC);
}

/**
 * Register a callback to be called when rtc matches.
 */
void mu_vm_rtc_set_cb(mu_vm_callback_fn fn, void *arg) {
  if (fn) {
    s_port.rtc_cb = fn;
    s_port.rtc_cb_arg = arg;
  } else {
    s_port.rtc_cb = NULL;
    s_port.rtc_cb_arg = NULL;
  }
}

void mu_vm_rtc_alarm_at(mu_vm_time_t at) {
  hri_rtcmode0_write_COMP_COMP_bf(RTC, 0, at);
  // Particular to the SAMD21, hri_rtcmode0_write_COMP_COMP_bf() clears the
  // READREQ_RCONT bit, which prevents subsequent RTC reads from updating.
  // Restore it here.
  hri_rtcmode0_set_READREQ_RCONT_bit(RTC);
  hri_rtcmode0_set_READREQ_RREQ_bit(RTC);
}

// ==========
// LED

void mu_vm_led_set(bool on) { gpio_set_pin_level(USER_LED_AL, !on); }

bool mu_vm_led_get(void) { return !gpio_get_pin_level(USER_LED_AL); }

// ==========
// BUTTON

bool mu_vm_button_is_pressed(void) {
  return !gpio_get_pin_level(USER_BUTTON_AL);
}

void mu_vm_button_set_cb(mu_vm_callback_fn fn, void *arg) {
  if (fn) {
    s_port.button_cb = fn;
    s_port.button_cb_arg = arg;
  } else {
    s_port.button_cb = NULL;
    s_port.button_cb_arg = NULL;
  }
}

// ==========
// SERIAL

void mu_vm_serial_write(uint8_t byte) {
  // for either synchronous or asynchronous writes: make sure the UART DRE bit
  // is set (data ready) and write the byte:
  while (!mu_vm_serial_can_write()) {
    asm("nop");
  }
  _usart_async_write_byte(&USART_0.device, byte);

  if (s_port.tx_cb) {
    // If the callback function is set, the user is requesting asynchronous
    // operation (interrupt driven).  Enable DRE interrupts so that the user
    // callback will be triggered as soon as this write completes.
    _usart_async_set_irq_state(&USART_0.device, USART_ASYNC_BYTE_SENT, true);
  }
}

bool mu_vm_serial_can_write(void) {
  // returns DRE bit
  return hri_sercomusart_get_interrupt_DRE_bit(USART_0.device.hw);
}

void mu_vm_serial_set_write_cb(mu_vm_callback_fn fn, void *arg) {
  if (fn) {
    s_port.tx_cb = fn;
    s_port.tx_cb_arg = arg;
    // enable DRE interrupts
    _usart_async_set_irq_state(&USART_0.device, USART_ASYNC_BYTE_SENT, true);
  } else {
    // disable DRE interrupts
    _usart_async_set_irq_state(&USART_0.device, USART_ASYNC_BYTE_SENT, false);
    s_port.tx_cb = NULL;
    s_port.tx_cb_arg = NULL;
  }
}

bool mu_vm_serial_write_in_progress(void) {
  return !hri_sercomusart_get_INTEN_TXC_bit(USART_0.device.hw);
}

uint8_t mu_vm_serial_read(void) {
  // RXC interrupts must be enabled before mu_vm_serial_can_read() goes true.
  _usart_async_set_irq_state(&USART_0.device, USART_ASYNC_RX_DONE, true);
  while (!mu_vm_serial_can_read()) {
    asm("nop");
  }
  // rx_has_data is set in the rx_done_cb handler
  s_rx_has_data = false;       // clear the virtual RXD bit
  return s_rx_data;
}

bool mu_vm_serial_can_read(void) {
  // Returns the "virtual RXD" bit, set in the rx_done_cb handler
  return s_rx_has_data;
}

void mu_vm_serial_set_read_cb(mu_vm_callback_fn fn, void *arg) {
  if (fn) {
    s_port.rx_cb = fn;
    s_port.rx_cb_arg = arg;
    _usart_async_set_irq_state(&USART_0.device, USART_ASYNC_RX_DONE, true);
  } else {
    s_port.rx_cb = NULL;
    s_port.rx_cb_arg = NULL;
  }
}

bool mu_vm_serial_read_in_progress(void) {
  // Return true if the start of frame detecteted but receive not yet complete.
  // See also mu_vm_serial_read()
  //
  // NB: start-of-frame detection must be enabled(CTRLB.SFDE is '1')
  // for RXS to be active.
  return
    !hri_sercomusart_get_INTEN_RXC_bit(USART_0.device.hw) &&
    hri_sercomusart_get_INTEN_RXS_bit(USART_0.device.hw);
}

// ==========
// SLEEP

/**
 * @brief Put the processor into low-power mode until time t arrives, or an
 * external event wakes the processor.
 */
void mu_vm_sleep_until(mu_vm_time_t then) {
  mu_vm_time_t now = mu_vm_rtc_now();
  if (mu_vm_time_difference(then, now) > MIN_SLEEP_DURATION) {
    mu_vm_rtc_alarm_at(then);
    go_to_sleep();
  }
}

/**
 * @brief Put the processor into low-power mode until an external event wakes
 * the processor.
 */
void mu_vm_sleep(void) { go_to_sleep(); }

// =============================================================================
// private (local) code

static void button_cb_trampoline(void) {
  if (s_port.button_cb) {
    s_port.button_cb(s_port.button_cb_arg);
  }
}

/**
 * Replace the standard ASF4 USART callbacks with specialized versions.
 */
static void customize_usart_driver(void) {
  USART_0.device.usart_cb.tx_byte_sent = tx_byte_sent;
  USART_0.device.usart_cb.rx_done_cb = rx_done_cb;
  USART_0.device.usart_cb.tx_done_cb = tx_done_cb;
  USART_0.device.usart_cb.error_cb = error_cb;
  // enable start-of-frame detection
  hri_sercomusart_set_CTRLB_SFDE_bit(USART_0.device.hw);
}

/**
 * Arrive here when the USART is ready to accept a new character.
 */
static void tx_byte_sent(struct _usart_async_device *device) {
  // notify user callback if set
  if (s_port.tx_cb) {
    s_port.tx_cb(s_port.tx_cb_arg);
  }
}

/**
 * Arrive here when a character has been fully received.
 */
static void rx_done_cb(struct _usart_async_device *device, uint8_t data) {
  // The ASF4 serial interrupt handler reads the data register which clears the
  // RXD bit (as well as fetching the data).  The user needs to know if there is
  // data available, so we shadow the RXD bit and the data register with
  // s_rx_has_data and s_rx_data respectively.
  s_rx_data = data;
  s_rx_has_data = true;

  // clear the start of frame flag so mu_vm_serial_read_in_progress() will
  // return false.
  hri_sercomusart_clear_INTFLAG_RXS_bit(USART_0.device.hw);

  // notify user callback if set
  if (s_port.rx_cb) {
    s_port.rx_cb(s_port.rx_cb_arg);
  }
}

/**
 * Arrive here when the previous character has been fully transmitted.
 */
static void tx_done_cb(struct _usart_async_device *device) {
  // No action.  In the future, we might use this for an async version of
  // mu_vm_serial_write_in_progress().
  asm("nop");
}

/**
 * Arrive here on an error condition.
 */
static void error_cb(struct _usart_async_device *device) {
  asm("nop");
}

static void rtc_cb_trampoline(struct calendar_dev *const dev) {
  // Arrive here when the RTC count register matches the RTC compare register.
  // Even if the user hasn't registered a callback, this will wake the processor
  // from sleep...
  if (s_port.rtc_cb) {
    s_port.rtc_cb(s_port.rtc_cb_arg);
  }
}

static void go_to_sleep(void) {
  sleep(3); // in hal_sleep
  //Mystery bug in SAMD21: won't wake from sleep without a following delay.
  delay_us(200);
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
