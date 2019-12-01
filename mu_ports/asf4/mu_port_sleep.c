/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
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

#include "mu_port_sleep.h"
#include "mu_port_time.h"
#include "driver_init.h"
#include "hal_atomic.h"

// =============================================================================
// private types and definitions

#define RTC_SLEEP_MARGIN 50

// =============================================================================
// private declarations

static void rtc_callback(struct calendar_dev *const dev);
static void set_rtc_alarm_for(mu_port_time_t t);
static void enter_sleep_mode();

// =============================================================================
// local storage

// =============================================================================
// public code

void mu_port_sleep_init() {
	calendar_enable(&CALENDAR_0);
  // see hpl/rtc/hpl_rtc.c -- enables RTC interrupts
  //
  // Normally, the CALENDAR object will insert its own callback.  Since we only
  // use the calendar object for its underlying RTC, we insert our own callback.
  _calendar_register_callback(&CALENDAR_0.device, rtc_callback);
}

void mu_port_sleep_indefinitely() {
  enter_sleep_mode();
}

void mu_port_sleep_until(mu_port_time_t t) {
  // If the alarm time has passed by the time we enter sleep mote, the device
  // will sleep "forever" (until the RTC completes one full roll-over).  So we
  // assure that `t` is greater than `now` plus a little extra:

  mu_port_time_t t1;

  CRITICAL_SECTION_ENTER()
  t1 = mu_port_time_offset(mu_port_time_now(), RTC_SLEEP_MARGIN);
  if (mu_port_time_is_before(t, t1)) {
    t = t1;
  }
  set_rtc_alarm_for(t);
  CRITICAL_SECTION_LEAVE()
  enter_sleep_mode();
}

// =============================================================================
// private code

// Arrive here on RTC comparison interrupt
static void rtc_callback(struct calendar_dev *const dev) {
  // it's not clear we need to do anything here -- waking is the desired side
  // effect.
  asm("nop");
}

static void set_rtc_alarm_for(mu_port_time_t t) {
  // Set the RTC compare register.
  //
  // NOTE: t must be greater than the RTC COUNT register at the time we enter
  // sleep mode, else the device will sleep for a very very long time...
	hri_rtcmode0_write_COMP_COMP_bf(CALENDAR_0.device.hw, 0, t);
}

#define SLEEP_IDLE_MODE 2
#define SLEEP_STANDBY_MODE 3

static void enter_sleep_mode() {
  // see hpl/pm/hpl_pm.c for mapping of mode
  sleep(SLEEP_STANDBY_MODE);
}
