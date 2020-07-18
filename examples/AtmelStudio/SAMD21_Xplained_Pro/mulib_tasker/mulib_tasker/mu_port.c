/**
 * @file mu_port.c
 *
 */

 //DOM-IGNORE-BEGIN
 /******************************************************************************
 MIT License

 Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

// =============================================================================
// include files

#include "atmel_start.h"
#include "mulib.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// =============================================================================
// types and definitions

#define MAX_DURATION ((uint32_t)0x7fffffff)

// =============================================================================
// forward declarations to local functions

// =============================================================================
// local (static) storage

static mu_port_time_seconds_dt s_rtc_period;

// =============================================================================
// main code starts here

void mu_port_init() {
  // Initialize the RTC.  Use CALENDAR_0 since that's the only published
  // interface for interacting with the underlying RTC.
	calendar_enable(&CALENDAR_0);  // start RTC
  s_rtc_period = 1.0/(mu_port_time_seconds_dt)CONF_GCLK_RTC_FREQUENCY;
}

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

mu_port_time_dt mu_port_time_ms_to_duration(mu_port_time_ms_dt ms) {
    return mu_port_time_seconds_to_duration(ms / 1000.0); // could be better
}

mu_port_time_ms_dt mu_port_time_duration_to_ms(mu_port_time_dt dt) {
    return mu_port_time_duration_to_seconds(dt) * 1000;   // could be better
}

mu_port_time_dt mu_port_time_seconds_to_duration(mu_port_time_seconds_dt secs) {
  return secs / s_rtc_period;
}

mu_port_time_seconds_dt mu_port_time_duration_to_seconds(mu_port_time_dt dt) {
  return dt * s_rtc_period;
}

mu_port_time_t mu_port_rtc_now() {
	return hri_rtcmode0_read_COUNT_COUNT_bf(CALENDAR_0.device.hw);
}

// ================
// support for printf()

int _write(int file, char *ptr, int len) {
  // void file;
  int n = len;

  while (n-- > 0) {
    while (!USART_0_is_byte_sent())
  		;
    USART_0_write_byte(*ptr++);
  }
  return len;
}

int _read(int file, char *ptr, int len) {
  // void file;
  int n = len;

  while (n-- > 0) {
    while (!USART_0_is_byte_received())
			;
    *ptr++ = USART_0_read_byte();
  }
  return len;
}

// =============================================================================
// private code
