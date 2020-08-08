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

#include "mu_vm_test.h"
#include "mu_vm.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// =============================================================================
// private types and definitions

#define MAX_CHARS 50

// =============================================================================
// private declarations

static void test_wait_ms(int ms);
static void test_sync_printf(const char *fmt, ...);
static void test_async_printf(const char *fmt, ...);
static void test_print_time(mu_vm_time_t t);
static void button_press_cb(void *arg);
static void serial_write_cb(void *arg);
static void serial_read_cb(void *arg);
static void rtc_cb(void *arg);

// =============================================================================
// local storage

static mu_vm_time_t s_epoch;
static volatile bool s_rtc_matched;
static volatile bool s_button_pressed;
static volatile bool s_rx_char_available;

static uint8_t s_tx_buf[MAX_CHARS];
static volatile int s_tx_count = 0;
static volatile int s_tx_index = 0;

// =============================================================================
// public code

void mv_vm_test_init(void) {
	mu_vm_init();
}

void mv_vm_test_step(void) {
  uint8_t c;

  // capture start up time...
  s_epoch = mu_vm_rtc_now();

  test_sync_printf("\r\n================\r\nstarting mu_vm_test\r\n");
  test_sync_printf("sync serial write\r\n");
  test_async_printf("async serial write\r\n");

  test_sync_printf("time before is ");
  test_print_time(mu_vm_rtc_now());
  test_sync_printf("\r\n");
  test_sync_printf("looping for 2.5 seconds...");
  test_wait_ms(2500);
  test_sync_printf("done.  time after is ");
  test_print_time(mu_vm_rtc_now());
  test_sync_printf("\r\n");

  s_rtc_matched = false;
  mu_vm_rtc_set_cb(rtc_cb, NULL);
  mu_vm_rtc_alarm_at(mu_vm_time_offset(mu_vm_rtc_now(),
                                           mu_vm_time_ms_to_duration(2500)));
  test_sync_printf("waiting 2.5 seconds for RTC match...");
  while (!s_rtc_matched) {
	  asm("nop");
    // buzz...
  }
  test_sync_printf("done.  time is now ");
  test_print_time(mu_vm_rtc_now());
  test_sync_printf("\r\n");

  test_sync_printf("flashing LED for 2.5 seconds...");
  for (int i = 0; i < 10; i++) {
    mu_vm_led_set(!mu_vm_led_get());
    test_wait_ms(250);
  }
  mu_vm_led_set(false);
  test_sync_printf("done.  time is now ");
  test_print_time(mu_vm_rtc_now());
  test_sync_printf("\r\n");

  test_sync_printf("waiting for button press (synchronous)...");
  while (!mu_vm_button_is_pressed()) {
    // buzz...
  }
  test_sync_printf("received button press.\r\n");

  // make sure button has stopped bouncing before moving to next test.
  do {
    test_wait_ms(10);
  } while (mu_vm_button_is_pressed());

  s_button_pressed = false;
  mu_vm_button_set_cb(button_press_cb, NULL);
  test_sync_printf("waiting for button press (asynchronous)...");
  while (!s_button_pressed) {
    // buzz...
  }
  test_sync_printf("received button press.\r\n");

  test_sync_printf("waiting for keyboard input (synchronous)...");
  c = mu_vm_serial_read();
  test_sync_printf("received '%c'\r\n", c);


  test_sync_printf("waiting for keyboard input (asynchronous)...");
  // set callback
  s_rx_char_available = false;
  mu_vm_serial_set_read_cb(serial_read_cb, NULL);
  while (!s_rx_char_available) {
	  asm("nop");
  }
  c = mu_vm_serial_read();
  test_sync_printf("received '%c'\r\n", c);

#ifdef MU_VM_CAN_SLEEP
  test_sync_printf("sleeping for 2.5 seconds...");
  // Assure line gets printed before sleeping
  while (!mu_vm_serial_can_write()) {
	  asm("nop");
	  // buzz...
  }
  mu_vm_sleep_until(mu_vm_time_offset(mu_vm_rtc_now(),
                                          mu_vm_time_ms_to_duration(2500)));
  test_sync_printf("done.  time is now ");
  test_print_time(mu_vm_rtc_now());
  test_sync_printf("\r\n");

  test_sync_printf("sleeping until button press...");
  // Assure line gets printed before sleeping
  while (!mu_vm_serial_can_write()) {
	  asm("nop");
	  // buzz...
  }
  mu_vm_sleep();
  test_sync_printf("done.  time is now ");
  test_print_time(mu_vm_rtc_now());
  test_sync_printf("\r\n");
#endif

  test_sync_printf("end of mu_vm_test\r\n================\r\n");
}

// =============================================================================
// private (local) code

static void test_wait_ms(int ms) {
  mu_vm_time_t then =
      mu_vm_time_offset(mu_vm_rtc_now(), mu_vm_time_ms_to_duration(ms));
  while (mu_vm_time_precedes(mu_vm_rtc_now(), then)) {
	  asm("nop");
    // buzz...
  }
}

static void test_sync_printf(const char *fmt, ...) {
  static uint8_t buf[MAX_CHARS];
  size_t n_chars;

  va_list ap;
  va_start(ap, fmt);
  n_chars = vsnprintf((char *)buf, MAX_CHARS, fmt, ap);
  va_end(ap);

  uint8_t *p = buf;

  while (n_chars > 0) {
    while (!mu_vm_serial_can_write()) {
      asm("nop");
      // buzz...
    }
    mu_vm_serial_write(*p++);
    n_chars--;
  }

}

static void test_async_printf(const char *fmt, ...) {

  va_list ap;
  va_start(ap, fmt);
  s_tx_count = vsnprintf((char *)s_tx_buf, MAX_CHARS, fmt, ap);
  va_end(ap);

  s_tx_index = 0;
  if (s_tx_count > 0) {
    // install callback to handle subsequent writes
    mu_vm_serial_set_write_cb(serial_write_cb, NULL);
    // write the first character to start things
    s_tx_index += 1;
    s_tx_count -= 1;
    mu_vm_serial_write(s_tx_buf[0]);
  }

  // buzz until all chars written
  while (s_tx_count > 0) {
    asm("nop");
  }

  // de-install callbacks.
  mu_vm_serial_set_write_cb(NULL, NULL);
}

static void test_print_time(mu_vm_time_t t) {
  mu_vm_time_dt uptime = mu_vm_time_difference(t, s_epoch);
  int ms = mu_vm_time_duration_to_ms(uptime);
#ifdef PORT_FLOAT
  PORT_FLOAT s = mu_vm_time_duration_to_s(uptime);
  test_sync_printf("time = %d ms (%f s)", ms, s);
#else
  test_sync_printf("time = %d ms", ms);
#endif
}

static void button_press_cb(void *arg) {
	s_button_pressed = true;
}

static void serial_write_cb(void *arg) {
  // arrive here when serial can send another char
  if (s_tx_count > 0) {
    mu_vm_serial_write(s_tx_buf[s_tx_index++]);
    s_tx_count -= 1;
  }
  // When the write operation completes, this callback will be called again.
}

static void serial_read_cb(void *arg) {
  s_rx_char_available = true;
}

static void rtc_cb(void *arg) {
	s_rtc_matched = true;
}
