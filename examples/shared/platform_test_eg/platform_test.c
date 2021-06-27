/*
 * platform_test.c
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 */

// =============================================================================
// Includes

#include "platform_test.h"
#include "mu_platform.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);


// =============================================================================
// Local storage

volatile static bool s_button_was_pressed;

// =============================================================================
// Public code

void platform_test_init(void) {
  mu_time_t until;

  mu_platform_init();
  mu_button_io_set_callback(button_cb);

  printf("\n\rmulib platform_test v%s.\n", VERSION);
  s_button_was_pressed = false;

  mu_led_io_set(MU_LED_0, true);
  printf("LED should be on for 5 seconds:\n");
  until = mu_time_offset(mu_rtc_now(), MU_TIME_MS_TO_DURATION(5000));
  while (mu_time_precedes(mu_rtc_now(), until)) {
	  asm(" nop");
	  // buzz...
  }

  mu_led_io_set(MU_LED_0, false);
  printf("LED should be off for 5 seconds:\n");
  until = mu_time_offset(mu_rtc_now(), MU_TIME_MS_TO_DURATION(5000));
  while (mu_time_precedes(mu_rtc_now(), until)) {
      asm(" nop");
	  // buzz...
  }

  mu_led_io_set(MU_LED_0, true);
  printf("Press button to toggle LED:\n");
}

void platform_test_step(void) {
  if (s_button_was_pressed == true) {
    s_button_was_pressed = false;
    mu_led_io_set(MU_LED_0, !mu_led_io_get(MU_LED_0));
    printf("LED is %s\n", mu_led_io_get(MU_LED_0) ? "on" : "off");
  } else {
    asm(" nop");
  }
}

// =============================================================================
// Local (private) code

static void button_cb(uint8_t button_id, bool button_is_pressed) {
  (void)button_id;

  if (button_is_pressed) {
    s_button_was_pressed = true;
  }
}
