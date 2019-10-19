/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_enable
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip
 * Support</a>
 */

// =============================================================================
// includes

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "driver_init.h"
#include "mu_evt.h"
#include "mu_sched.h"
#include "mu_task.h"
#include "mu_time.h"
#include "port.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// =============================================================================
// types and definitions

// Define repetition interval for toggling LED and updating display
#define LED_UPDATE_INTERVAL 0.75

// =============================================================================
// forward declarations

/**
 * \brief mulib task that blinks LED
 */
static void led_task_fn(void *self, void *arg);

// =============================================================================
// local storage

static mu_sched_t s_sched;
static mu_evt_t s_led_event;

// =============================================================================
// public code

int main(void) {
  // Perform board-specific initialization
  atmel_start_init();

  // Perform port-specific initialization needed by mulib (RTC)
  port_init();

  // Initialize the scheduler.  In this demo, we don't need  to provide a pool
  // for ISR events so we pass NULL and 0.
  mu_sched_init(&s_sched, NULL, 0);

  mu_time_t now = mu_time_now();

  // Queue the LED event, triggering as soon as we start the scheduler
  mu_sched_add(
      &s_sched,
      mu_evt_init_at(&s_led_event, now, led_task_fn, NULL, "LED"));

  // Run the scheduler
  while (1) {
    mu_sched_step(&s_sched);
  }
  return 0; // Like Charlie on the MTA (no he never returned...)
}

// =============================================================================
// private code

static void led_task_fn(void *self, void *arg) {
  // Inhibit ompiler warning about unused variables
  (void)self;
  (void)arg;
  // Toggle the LED pin
  gpio_toggle_pin_level(LED0);
  // Reschedule the LED event to trigger again 500 mSec in the future
  mu_evt_offset_time(&s_led_event,
                     mu_time_seconds_to_duration(LED_UPDATE_INTERVAL));
  mu_sched_add(&s_sched, &s_led_event);
}
