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
 * Reference implementation of mu_vm.c for POSIX
 */

// =============================================================================
// includes

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// private types and definitions

// =============================================================================
// private declarations

// =============================================================================
// local storage

static bool s_led_state;
static bool s_button_state;

// =============================================================================
// public code

void mu_vm_init(void) {
  s_led_state = false;
  s_button_state = false;
}

// TIME

mu_vm_time_t mu_vm_time_offset(mu_vm_time_t t, mu_vm_time_dt dt) {
  return t + dt;
}

mu_vm_time_dt mu_vm_time_difference(mu_vm_time_t t1, mu_vm_time_t t2) {
  return t1 - t2;
}

bool mu_vm_time_precedes(mu_vm_time_t t1, mu_vm_time_t t2) {
  return is_msb_set(t1 - t2);
}

bool mu_vm_time_equals(mu_vm_time_t t1, mu_vm_time_t t2) {
  return t1 == t2;
}

bool mu_vm_time_follows(mu_vm_time_t t1, mu_vm_time_t t2) {
  return is_msb_set(t2 - t1);
}

mu_vm_time_dt mu_vm_time_ms_to_duration(mu_vm_time_ms_dt ms) {
  return quo_rounded(ms * CLOCKS_PER_SEC, 1000);
}

mu_vm_time_ms_dt mu_vm_time_duration_to_ms(mu_vm_time_dt dt) {
  return quo_rounded(dt * 1000, CLOCKS_PER_SEC);
}

mu_vm_time_s_dt mu_vm_time_duration_to_s(mu_vm_time_dt dt) {
  return dt / CLOCKS_PER_SEC;
}

mu_vm_time_dt mu_vm_time_s_to_duration(mu_vm_time_s_dt seconds) {
  return seconds * CLOCKS_PER_SEC;
}

// REAL TIME CLOCK

mu_vm_time_t mu_vm_rtc_now(void) {
  return clock();
}

/**
 * Register a callback to be called when rtc matches.
 */
void mu_vm_rtc_set_cb(mu_vm_callback_fn fn, void *arg) {
  if (fn) {
    s_vm_state.rtc_cb = fn;
    s_vm_state.rtc_cb_arg = arg;
  } else {
    s_vm_state.rtc_cb = NULL;
    s_vm_state.rtc_cb_arg = NULL;
  }
}

void mu_vm_rtc_alarm_at(mu_vm_time_t at) {
}

// LED

void mu_vm_led_set(bool on) {
  s_led_state = on;
}

bool mu_vm_led_get(void) {
  return s_led_state;
}

// BUTTON

bool mu_vm_button_is_pressed(void) {
  return s_button_state;
}

void mu_vm_button_set_cb(mu_vm_callback_fn fn, void *arg) {
  if (fn) {
    s_vm_state.button_cb = fn;
    s_vm_state.button_cb_arg = arg;
  } else {
    s_vm_state.button_cb = NULL;
    s_vm_state.button_cb_arg = NULL;
  }
}

// SERIAL

void mu_vm_serial_set_write_cb(mu_vm_serial_write_cb cb) {
  s_vm_state.tx_cb = cb;
}

bool mu_vm_serial_is_ready_to_write(void) {
  return hri_sercomusart_get_interrupt_DRE_bit(USART_0.device.hw);
}

void mu_vm_serial_write(uint8_t data) {
  // If callback is set, enable transmit complete interrupt
  if (s_vm_state.tx_cb != NULL) {
    hri_sercomusart_set_INTEN_DRE_bit(USART_0.device.hw);
  }
  // write the byte
  hri_sercomusart_write_DATA_reg(USART_0.device.hw, data);
}

void mu_vm_serial_set_read_cb(mu_vm_serial_read_cb cb) {
  s_vm_state.rx_cb = cb;
}

bool mu_vm_serial_is_ready_to_read(void) {
	return hri_sercomusart_get_interrupt_RXC_bit(USART_0.device.hw);
}

void mu_vm_serial_read(void) {
  // If callback is set, enable rx complete interrupt
  if (s_vm_state.rx_cb != NULL) {
    hri_sercomusart_write_INTEN_RXC_bit(USART_0.device.hw, true);
  }
}

bool mu_vm_serial_read_in_progress(void) {
  // Return true if the start of frame deteceted but receive not yet complete.
  // See also mu_vm_serial_read()
  //
  // NB: start-of-frame detection must be enabled(CTRLB.SFDE is '1')
  // for RXS to be active.
  return
    !hri_sercomusart_get_INTEN_RXC_bit(USART_0.device.hw) &&
    hri_sercomusart_get_INTEN_RXS_bit(USART_0.device.hw);
}

// SLEEP

bool mu_vm_is_ready_to_sleep(void) {
  return !mu_vm_tx_in_progress();
}

/*
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
// functions that modify and query the virtual machine state.

void vm_set_time(mu_vm_time_t time) {
  s_time = time;
  s_time_changed = true;
}

mu_vm_time_t vm_get_time(void) {
  return s_time;
}

bool vm_get_led_changed(void) {
  return s_led_changed;
}

bool vm_get_led_state(void) {
  return s_led_state;
}

void vm_set_button_state(bool pressed) {
  s_button_state = pressed;
  s_button_changed = true;
}

bool vm_get_button_state(void) {
  return s_button_state;
}

// emulate receiving a UART byte
void vm_write_uart_rx_buffer(uint8_t byte) {
  s_uart_rx_buffer = byte;
  s_uart_received = true;
}

bool vm_get_uart_tx_available(void) {
  return s_uart_tx_available;
}

// return the most recently transmitted UART byte
uint8_t vm_read_uart_tx_buffer(void) {
  return s_uart_tx_buffer;
}
// =============================================================================
// private (local) code

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

static bool is_msb_set(mu_port_time_t t) {
  return TIME_IS_SIGNED ? (t < 0) : ((t << 1) >> 1) ^ t;
}
