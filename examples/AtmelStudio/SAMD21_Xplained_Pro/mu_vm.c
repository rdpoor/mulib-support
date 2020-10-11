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
static bool tx_in_progress(void);
static void rtc_cb_trampoline(struct calendar_dev *const dev);
static void go_to_sleep(void);
static int quo_rounded(int x, int y);

// =============================================================================
// local storage

#ifdef MU_VM_FLOAT
static MU_VM_FLOAT s_rtc_period; // 1.0/RTC_FREQUENCY
#endif

static volatile bool s_tx_in_progress;

typedef struct {
  void (*button_cb)(void *arg);
  void *button_cb_arg;
  void (*tx_cb)(void);
  void *tx_cb_arg;
  void (*rx_cb)(uint8_t data);
  void *rx_cb_arg;
  void (*rtc_cb)(void *arg);
  void *rtc_cb_arg;
} vm_state_t;

static vm_state_t s_vm_state;

static volatile uint8_t s_tx_data;
static volatile bool s_tx_has_data;
static volatile uint8_t s_rx_data;
static volatile bool s_rx_has_data;

// =============================================================================
// public code

void mu_vm_init(void) {
#ifdef MU_VM_FLOAT
  s_rtc_period = 1.0 / (MU_VM_FLOAT)CONF_GCLK_RTC_FREQUENCY;
#endif
  memset(&s_vm_state, 0, sizeof(s_vm_state));

  ext_irq_register(PIN_PA15, button_cb_trampoline);

  // Commandeer the USART driver to do our bidding...
  customize_usart_driver();
  usart_async_enable(&USART_0);

  // Initialize the RTC.  Use CALENDAR_0 since that's the only published
  // interface for interacting with the underlying RTC.
  calendar_enable(&CALENDAR_0); // start RTC
  _calendar_register_callback(&CALENDAR_0.device, rtc_cb_trampoline);
}

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

// REAL TIME CLOCK

mu_vm_time_t mu_vm_rtc_now(void) {
  return hri_rtcmode0_read_COUNT_COUNT_bf(CALENDAR_0.device.hw);
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
  _calendar_set_comp(&CALENDAR_0.device, at);
  // Specific to the SAMD21, hri_rtcmode0_write_COMP_COMP_bf() clears the
  // READREQ_RCONT bit, which prevents subsequent RTC reads from updating.
  // Restore it here.
  hri_rtcmode0_set_READREQ_RCONT_bit(RTC);
  hri_rtcmode0_set_READREQ_RREQ_bit(RTC);
}

// LED

void mu_vm_led_set(bool on) { gpio_set_pin_level(USER_LED_AL, !on); }

bool mu_vm_led_get(void) { return !gpio_get_pin_level(USER_LED_AL); }

// BUTTON

bool mu_vm_button_is_pressed(void) {
  return !gpio_get_pin_level(USER_BUTTON_AL);
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

// TODO: is arg ever really used?
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
// private (local) code

static void button_cb_trampoline(void) {
  if (s_vm_state.button_cb) {
    s_vm_state.button_cb(s_vm_state.button_cb_arg);
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
  // enable the interrupts
  hri_sercomusart_write_INTEN_DRE_bit(USART_0.device.hw, true);
  hri_sercomusart_write_INTEN_TXC_bit(USART_0.device.hw, true);
  hri_sercomusart_write_INTEN_RXC_bit(USART_0.device.hw, true);
  hri_sercomusart_write_INTEN_ERROR_bit(USART_0.device.hw, true);
  // enable start-of-frame detection
  hri_sercomusart_set_CTRLB_SFDE_bit(USART_0.device.hw);
}

/**
 * Arrive here when the data buffer in the UART can accept another byte.
 * original code:
 *
 // static void usart_transmission_complete(struct _usart_async_device *device)
 // {
 // 	struct usart_async_descriptor *descr = CONTAINER_OF(device, struct usart_async_descriptor, device);
 //
 // 	descr->stat = 0;
 // 	if (descr->usart_cb.tx_done) {
 // 		descr->usart_cb.tx_done(descr);
 // 	}
 // }
 */
static void tx_byte_sent(struct _usart_async_device *device) {
  if (s_vm_state.tx_cb != NULL) {
    s_vm_state.tx_cb();
  }
}

/**
 * @brief Arrive here when a character has been fully received.
 */
static void rx_done_cb(struct _usart_async_device *device, uint8_t data) {
  // clear the start of frame flag so mu_vm_serial_read_in_progress() will
  // return false.
  hri_sercomusart_clear_INTFLAG_RXS_bit(USART_0.device.hw);

  // notify user callback if set
  if (s_vm_state.rx_cb) {
    s_vm_state.rx_cb(data);
  }
}

/**
 * Arrive here when the previous character has been fully transmitted.
 * original code:
 *
 // static void usart_transmission_complete(struct _usart_async_device *device)
 // {
 // 	struct usart_async_descriptor *descr = CONTAINER_OF(device, struct usart_async_descriptor, device);
 //
 // 	descr->stat = 0;
 // 	if (descr->usart_cb.tx_done) {
 // 		descr->usart_cb.tx_done(descr);
 // 	}
 // }
 */
static void tx_done_cb(struct _usart_async_device *device) {
  asm("nop");
}

/**
 * Arrive here on an error condition.
 * original code:
 *
 // static void usart_error(struct _usart_async_device *device)
 // {
 // 	struct usart_async_descriptor *descr = CONTAINER_OF(device, struct usart_async_descriptor, device);
 //
 // 	descr->stat = 0;
 // 	if (descr->usart_cb.error) {
 // 		descr->usart_cb.error(descr);
 // 	}
 // }
 */
static void error_cb(struct _usart_async_device *device) {
  asm("nop");
}

static bool tx_in_progress(void) {
  return !hri_sercomusart_get_INTEN_TXC_bit(USART_0.device.hw);
}

static void rtc_cb_trampoline(struct calendar_dev *const dev) {
  // Arrive here when the RTC count register matches the RTC compare register.
  // Even if the user hasn't registered a callback, this will wake the processor
  // from sleep...
  if (s_vm_state.rtc_cb) {
    s_vm_state.rtc_cb(s_vm_state.rtc_cb_arg);
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
