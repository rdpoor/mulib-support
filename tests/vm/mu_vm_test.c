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

static void test_setup(void);
static void test_rtc_cb(void *arg);
static void test_button_cb(void *arg);
static void test_serial_write_cb(void);
static void test_serial_read_cb(void);

// =============================================================================
// local storage

static uint32_t s_rtc_cb_call_count;
static uint32_t s_button_cb_call_count;
static void *s_expected_arg;

// =============================================================================
// public code

void mv_vm_test(void) {
	mu_vm_init();

	test_setup();
	// Assume that mu_vm_time_xxx() functions are covered by mu_time_xxx() tests.

  // ==========
	// Real Time Clock
	set_vm_time(10);
	ASSERT(mu_vm_rtc_now() == 10);

  // Callback function gets called on RTC match
	s_expected_arg = (void *)555;
	mu_vm_rtc_set_cb(test_rtc_cb, (void *)555);
	mu_vm_rtc_alarm_at(11);
	set_vm_time(11);
	ASSERT(s_rtc_cb_call_count == 1);

  // callback not called after setting cb function to NULL
	mu_vm_rtc_set_cb(NULL, NULL);
	mu_cm_rtc_alarm_at(12);
	set_vm_time(12);
	ASSERT(s_rtc_cb_call_count == 1);

	// ==========
	// LED

	// LED is initially off
	ASSERT(mu_vm_led_get() == false);

	mu_vm_led_set(true);
	ASSERT(mu_vm_led_get() == true);
	mu_vm_led_set(false);
	ASSERT(mu_vm_led_get() == false);

	// ==========
	// Button

	// Button is not initially pressed
	ASSERT(mu_vm_button_is_pressed() == false);
	set_vm_button(true);
	ASSERT(mu_vm_button_is_pressed() == true);
	set_vm_button(false);
	ASSERT(mu_vm_button_is_pressed() == false);

	s_expected_arg = (void *)444;
	mu_vm_button_set_cb(test_button_cb, (void *)444);
	set_vm_button(true);
	ASSERT(s_button_cb_call_count == 1);

	// ==========
	// SERIAL I/O

  // write
  // serial is initially ready to transmit
	ASSERT(mu_vm_serial_is_ready_to_write() == true);

	mu_vm_serial_set_write_cb(test_serial_write_cb);
	ASSERT(s_serial_write_cb_call_count == 0);
	set_vm_time(100);
	mu_vm_serial_write('a');
	// Our toy VM assumes 10 tics per serial byte
	ASSERT(mu_vm_serial_is_ready_to_write() == false);
  set_vm_time(110);
	ASSERT(s_serial_write_cb_call_count == 1);
	ASSERT(mu_vm_serial_is_ready_to_write() == true);

  // read
  // serial read initially has no data
	ASSERT(mu_vm_serial_is_ready_to_read() == false);

	set_vm_time(120);
  mu_vm_serial_set_read_cb(test_serial_read_cb);
	s_serial_written_char = 'b';
  write_vm_serial(s_serial_written_char);
	// Our toy VM assumes 10 tics per serial byte
	ASSERT(s_serial_write_cb_call_count == 0);
	ASSERT(mu_vm_serial_is_ready_to_read() == false);

	set_vm_time(130);
	ASSERT(s_serial_read_cb_call_count == 1);

	// ==========
	// SLEEP


}

// =============================================================================
// private (local) code

static void test_setup(void) {
	s_rtc_cb_call_count = 0;
	s_button_cb_call_count = 0;
  s_serial_write_cb_call_count = 0;
	s_serial_read_cb_call_count = 0;
}

static void test_rtc_cb(void *arg) {
	ASSERT(s_expected_arg == arg);
  s_rtc_cb_call_count++;
}

static void test_button_cb(void *arg) {
	ASSERT(s_expected_arg == arg);
  s_button_cb_call_count++;
}

static void test_serial_write_cb(void) {
	s_serial_write_cb_call_count++;
}

static void test_serial_read_cb(void) {
	ASSERT(mu_vm_serial_is_ready_to_read() == true);
	ASSERT(mu_vm_serial_read() == s_serial_written_char);
	ASSERT(mu_vm_serial_is_ready_to_read() == false);
	s_serial_read_cb_call_count++;
}

// =============================================================================
// virtual machine emulation

static void st_vm_time(mu_vm_time_t t) {

}

static void raise_vm_rtc_interrupt(void) {

}
