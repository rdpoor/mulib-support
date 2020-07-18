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

#ifndef _MU_PORT_H_
#define _MU_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_port.h"
#include <stdio.h>
#include <stdarg.h>

// =============================================================================
// private types and definitions

#define MAX_CHARS 50

// =============================================================================
// private declarations

static void wait_ms(int ms);
static void port_printf(const char *fmt, ...);
static void print_time(mu_time_t t);
static char test_getc(void);
static void button_press_cb(void *arg);
static void serial_read_cb(void *arg);

// =============================================================================
// local storage

static port_time_t s_epoch;
static bool s_button_pressed;
static bool s_char_available;

// =============================================================================
// public code

void mu_port_test(void) {
  port_init();

  // capture start up time...
  s_epoch = port_time_now();

  port_printf("\r\n================\r\nstarting mu_port_test\r\n");

  port_printf("looping for one second...");
  wait_ms(1000);
  port_printf("done\r\n");

  port_printf("flashing LED...\r\n");
  for (int i=0; i<10; i++) {
    port_led_set(!port_led_get());
    wait_ms(250);
  }

  port_printf("waiting for button press...");
  while (!port_button_is_pressed()) {
    // buzz...
  }
  port_printf("received button press.\r\n");

  s_button_was_pressed = false;
  port_button_set_cb(button_press_cb, NULL);
  port_printf("waiting for button press callback...");
  while (!s_button_pressed) {
    // buzz...
  }
  port_printf("received button callback.\r\n");

  port_printf("waiting for keyboard input...");
  while (!port_serial_can_read()) {
    // buzz...
  }
  char c = test_getc();
  port_printf("received'%c'\r\n", c);

  s_char_available = false;
  port_serial_set_read_cb(serial_read_cb, NULL);
  port_printf("waiting for serial read callback...");
  while (!s_char_available) {
    // buzz...
  }
  port_printf("received button callback.\r\n");


#ifdef PORT_CAN_SLEEP
  port_printf("sleeping for five seconds...");
  port_sleep_until(port_time_offset(port_time_now(), port_time_ms_to_duration(5000)));
  port_printf("done.\r\n");

  port_printf("sleeping until button press...");
  port_sleep();
  port_printf("done.\r\n");
#endif

  port_printf("\r\nend of mu_port_test\r\n================\r\n");
}

// =============================================================================
// private (local) code

static void wait_ms(int ms) {
  port_time_t now = port_time_now();
  port_time_t then = port_time_offset(now, port_time_ms_to_duration(ms));
  while (port_time_precedes(port_time_now(), then)) {
    // buzz...
  }
}

static void port_printf(const char *fmt, ...) {
  static char buf[MAX_CHARS];
  size_t n_remaining;

  va_list ap;
  va_start(ap, fmt);
  n_remaining = vsnprintf(buf, MAX_CHARS, fmt, ap);
  va_end(ap);

  char *p = buf;
  while (n_chars > 0) {
    int n_written = port_serial_write(p, n_chars);
    if (n_written < 0) break;
    n_remaining -= n_written;
    p += n_written;
  }
}

static void print_time(mu_time_t t) {
  port_time_dt uptime = mu_time_difference(t, s_epoch);
  int ms = port_time_duration_to_ms(uptime);
#ifdef PORT_FLOAT
  PORT_FLOAT s = port_time_duration_to_s(uptime);
  port_printf("time = %d ms (%f s)", ms, s);
#else
  port_printf("time = %d ms", ms);
#endif
}

static char test_getc(void) {
  char c;
  while (port_serial_read(&c, 1) == 0) {
    // buzz...
  }
  return c;
}

static void button_press_cb(void *arg) {
  s_button_pressed = true;
}

static void serial_read_cb(void *arg) {
  s_char_available = true;
}
