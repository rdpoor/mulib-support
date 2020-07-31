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
#include <definitions.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// =============================================================================
// private types and definitions

#define MAX_DURATION ((uint32_t)0x7fffffff)

#define MU_TIME_MS_TO_DURATION(ms) (((ms) * CONF_GCLK_RTC_FREQUENCY) / 1000)

// don't sleep for less than 1ms (32 RTC tics)
#define MIN_SLEEP_DURATION_MS 1

#define USER_LED_AL PORT_PIN_PC18
#define USER_BUTTON_AL PORT_PIN_PB31

// =============================================================================
// private declarations

static void button_cb_trampoline(uintptr_t context);
static void tx_cb_trampoline(uintptr_t context);
static void rx_cb_trampoline(uintptr_t context);
static void rtc_cb_trampoline(RTC_TIMER32_INT_MASK intCause, uintptr_t context);
static void go_to_sleep(void);
static int quo_rounded(int x, int y);

// =============================================================================
// local storage

// TODO: package up in a struct to memset can clear all at initialization

#ifdef PORT_FLOAT
static PORT_FLOAT s_rtc_period; // 1.0/RTC_FREQUENCY
#endif

static void (*s_button_cb)(void *arg);
static void *s_button_cb_arg;

static void (*s_tx_cb)(void *arg);
static void *s_tx_cb_arg;

static void (*s_rx_cb)(void *arg);
static void *s_rx_cb_arg;

static void (*s_rtc_cb)(void *arg);
static void *s_rtc_cb_arg;

static uint32_t s_rtc_frequency;
static mu_port_time_dt s_min_sleep_duration;

// =============================================================================
// public code

void mu_port_init(void) {
  RTC_Initialize();
  s_rtc_frequency = RTC_Timer32FrequencyGet();
  s_min_sleep_duration = mu_port_time_ms_to_duration(MIN_SLEEP_DURATION_MS);
#ifdef PORT_FLOAT
  s_rtc_period = 1.0/(mu_port_time_seconds_dt)s_rtc_frequency;
#endif
  RTC_Timer32Start();        // start counting

  PORT_PinOutputEnable(USER_LED_AL);
  mu_port_led_set(false);

  // Register the button_cb function to be called upon an EIC interrupt on
  // the user button.
  s_button_cb = NULL;
  s_button_cb_arg = NULL;
  EIC_CallbackRegister(EIC_PIN_15 , button_cb_trampoline, (uintptr_t)NULL);
  EIC_InterruptEnable(EIC_PIN_15);

  s_tx_cb = NULL;
  s_tx_cb_arg = NULL;
  SERCOM2_USART_WriteCallbackRegister(tx_cb_trampoline, (uintptr_t)NULL);

  s_rx_cb = NULL;
  s_rx_cb_arg = NULL;
  SERCOM2_USART_ReadCallbackRegister(rx_cb_trampoline, (uintptr_t)NULL);

  // Initialize the RTC.
  s_rtc_cb = NULL;
  s_rtc_cb_arg = NULL;
  // register RTC callback and enable interrupts
  RTC_Timer32CallbackRegister(rtc_cb_trampoline, (uintptr_t)NULL);
  RTC_Timer32InterruptEnable(RTC_TIMER32_INT_MASK_CMP0);
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
  return quo_rounded(ms * s_rtc_frequency, 1000);
}

int mu_port_time_duration_to_ms(mu_port_time_dt dt) {
  return quo_rounded(dt * 1000, s_rtc_frequency);
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
  return RTC_Timer32CounterGet();
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
  RTC_Timer32Compare0Set(at);
}

// LED

void mu_port_led_set(bool on) {
  PORT_PinWrite(USER_LED_AL, !on);
}

bool mu_port_led_get(void) {
  // Note difference between PORT_PinRead() and PORT_PinLatchRead()
  return !PORT_PinLatchRead(USER_LED_AL);
}

// BUTTON

bool mu_port_button_is_pressed(void) {
  return !PORT_PinRead(USER_BUTTON_AL);
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

bool mu_port_serial_write(const uint8_t *const buf, int n_bytes) {
  SERCOM2_USART_Write((void *)buf, n_bytes);
  return true;
}

bool mu_port_serial_can_write(void) {
  return !SERCOM2_USART_WriteIsBusy();
}

int mu_port_serial_write_count(void) {
  // TODO: tbd
  return 0;
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

bool mu_port_serial_read(uint8_t *const buf, int n_bytes) {
  if (!SERCOM2_USART_ReadIsBusy()) {
    // read not yet started
    SERCOM2_USART_Read(buf, n_bytes);
  }
  return true;
}

bool mu_port_serial_can_read(void) {
  // peek directly at the Receive Complete register in the USART
  return (SERCOM2_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) == SERCOM_USART_INT_INTFLAG_RXC_Msk;
}

bool mu_port_serial_read_count(void) {
  // TODO: tbd
  return 0;
}

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
  if (mu_port_time_difference(then, now) > s_min_sleep_duration) {
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

void button_cb_trampoline(uintptr_t context) {
  if (s_button_cb) {
    s_button_cb(s_button_cb_arg);
  }
}

void tx_cb_trampoline(uintptr_t context) {
  // arrive here when the previous call to serial_write() completes.
  if (s_tx_cb) {
    s_tx_cb(s_tx_cb_arg);
  }
}

void rx_cb_trampoline(uintptr_t context) {
  if (s_rx_cb) {
    s_rx_cb(s_rx_cb_arg);
  }
}

static void rtc_cb_trampoline(RTC_TIMER32_INT_MASK intCause, uintptr_t context) {
  // Arrive here when the RTC count register matches the RTC compare register.
  // Even if the user hasn't registered a callback, this will wake the processor
  // from sleep...
  if (s_rtc_cb) {
    s_rtc_cb(s_rtc_cb_arg);
  }
}

static void go_to_sleep(void) {
  // There appears to be a bug in the E54 silicon that causes the DFLL to lose
  // the state of the .FINE bits across STANDBY / wake events.  As a workaround,
  // we save the DFLL state prior to STANDBY and restore it upon waking.
  uint32_t saved_dfllval = OSCCTRL_REGS->OSCCTRL_DFLLVAL;
  PM_StandbyModeEnter();
  OSCCTRL_REGS->OSCCTRL_DFLLVAL = saved_dfllval;
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