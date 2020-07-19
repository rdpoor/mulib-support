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
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// =============================================================================
// private types and definitions

#define MAX_CHARS 50

// =============================================================================
// private declarations

static void wait_ms(int ms);
static void port_printf(const char *fmt, ...);
// static void print_time(mu_port_time_t t);
static uint8_t test_getc(void);
static void test_flush_rx(void);
static void button_press_cb(void *arg);
static void serial_read_cb(void *arg);
static void rtc_cb(void *arg);

// =============================================================================
// local storage

static mu_port_time_t s_epoch;
static volatile bool s_rtc_matched;
static volatile bool s_button_pressed;
static volatile bool s_char_available;

// =============================================================================
// public code

void mulib_port_test_init(void) {
	mu_port_init();
}

void mulib_port_test_step(void) {
  uint8_t c;

  // capture start up time...
  s_epoch = mu_port_rtc_now();

  port_printf("\r\n================\r\nstarting mu_port_test\r\n");

  port_printf("looping for 2.5 seconds...");
  wait_ms(2500);
  port_printf("done\r\n");

  s_rtc_matched = false;
  mu_port_rtc_set_cb(rtc_cb, NULL);
  mu_port_rtc_alarm_at(mu_port_time_offset(mu_port_rtc_now(),
                                           mu_port_time_ms_to_duration(2500)));
  port_printf("waiting 2.5 seconds for RTC match...");
  while (!s_rtc_matched) {
	  asm("nop");
    // buzz...
  }
  port_printf("done\r\n");

  port_printf("flashing LED for 2.5 seconds...");
  for (int i = 0; i < 10; i++) {
    mu_port_led_set(!mu_port_led_get());
    wait_ms(250);
    // mu_port_led_set(true);
    // wait_ms(250);
    // mu_port_led_set(false);
    // wait_ms(250);
  }
  mu_port_led_set(false);
  port_printf("done\r\n");

  port_printf("waiting for button press (synchronous)...");
  while (!mu_port_button_is_pressed()) {
    // buzz...
  }
  port_printf("received button press.\r\n");

  // make sure button has stopped bouncing before moving to next test.
  do {
    wait_ms(10);
  } while (mu_port_button_is_pressed());

  s_button_pressed = false;
  mu_port_button_set_cb(button_press_cb, NULL);
  port_printf("waiting for button press (asynchronous)...");
  while (!s_button_pressed) {
    // buzz...
  }
  port_printf("received button press.\r\n");

  port_printf("waiting for keyboard input (synchronous)...");
  while (!mu_port_serial_can_read()) {
    // buzz...
  }
  port_printf("received '%c'\r\n", test_getc());

  // slurp any characters before next test...
  test_flush_rx();

  s_char_available = false;
  mu_port_serial_set_read_cb(serial_read_cb, NULL);
  // DESIGN QUESTION: should mu_port_init() start an async read process, or
  // should we wait for an explicit call to mu_port_serial_read()?  Note that
  // ASF4 / SERCOM ASYNC chooses the former.
  mu_port_serial_read(&c, 1);
  port_printf("waiting for keyboard input (asynchronous)...");
  while (!s_char_available) {
    // buzz...
  }
  port_printf("received '%c'\r\n", test_getc());

  // slurp any characters before next test...
  test_flush_rx();

#ifdef MU_PORT_CAN_SLEEP
  port_printf("sleeping for 2.5 seconds...");
  // Assure line gets printed before sleeping
  while (!mu_port_serial_can_write()) {
	  asm("nop");
	  // buzz...
  }
  mu_port_sleep_until(mu_port_time_offset(mu_port_rtc_now(),
                                          mu_port_time_ms_to_duration(2500)));
  port_printf("done.\r\n");

  port_printf("sleeping until button press...");
  // Assure line gets printed before sleeping
  while (!mu_port_serial_can_write()) {
	  asm("nop");
	  // buzz...
  }
  mu_port_sleep();
  port_printf("done.\r\n");
#endif

  port_printf("end of mu_port_test\r\n================\r\n");
}

// =============================================================================
// private (local) code

static void wait_ms(int ms) {
  mu_port_time_t then =
      mu_port_time_offset(mu_port_rtc_now(), mu_port_time_ms_to_duration(ms));
  while (mu_port_time_precedes(mu_port_rtc_now(), then)) {
	  asm("nop");
    // buzz...
  }
}

static void port_printf(const char *fmt, ...) {
  static uint8_t buf[MAX_CHARS];
  size_t n_remaining;

  while (!mu_port_serial_can_write()) {
    asm("nop");
    // buzz...
  }

  va_list ap;
  va_start(ap, fmt);
  n_remaining = vsnprintf((char *)buf, MAX_CHARS, fmt, ap);
  va_end(ap);

  uint8_t *p = buf;
  while (n_remaining > 0) {
    int n_written = mu_port_serial_write(p, n_remaining);
    if (n_written < 0)
      break;
    n_remaining -= n_written;
    p += n_written;
  }
}

// static void print_time(mu_port_time_t t) {
//   mu_port_time_dt uptime = mu_port_time_difference(t, s_epoch);
//   int ms = mu_port_time_duration_to_ms(uptime);
// #ifdef PORT_FLOAT
//   PORT_FLOAT s = mu_port_time_duration_to_s(uptime);
//   port_printf("time = %d ms (%f s)", ms, s);
// #else
//   port_printf("time = %d ms", ms);
// #endif
// }

static uint8_t test_getc(void) {
  uint8_t c;
  while (mu_port_serial_read(&c, 1) == 0) {
    // buzz...
  }
  return c;
}

static void test_flush_rx(void) {
  uint8_t c;
  // read characters until there aren't any more to read
  while (mu_port_serial_read(&c, 1) != 0) {
    // repeat...
  }
}

static void button_press_cb(void *arg) {
	s_button_pressed = true;
}

static void serial_read_cb(void *arg) {
	s_char_available = true;
}

static void rtc_cb(void *arg) {
	s_rtc_matched = true;
}
