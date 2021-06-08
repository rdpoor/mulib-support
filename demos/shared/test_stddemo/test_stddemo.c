/*
 * test_stddemo.c
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 */

// =============================================================================
// Includes

#include "test_stddemo.h"
#include "mu_platform.h"
#include <stdbool.h>
#include <stdio.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

// =============================================================================
// Local (forward) declarations

static void button_cb(bool button_is_pressed);

static void set_led(bool on);
static bool get_led(void);

// =============================================================================
// Local storage

volatile static bool s_button_was_pressed;
static bool s_led_state;

// =============================================================================
// Public code

void test_stddemo_init(void) {
  mu_time_t until;

  mu_platform_init();
  mu_stddemo_init(button_cb);

  mu_stddemo_printf("\n\rmulib test_stddemo v%s.\n", VERSION);
  s_button_was_pressed = false;
  s_led_state = false;

  set_led(true);
  mu_stddemo_printf("LED should be on for 5 seconds:\n");
  until = mu_time_offset(mu_time_now(), MU_TIME_MS_TO_DURATION(5000));
  while (mu_time_precedes(mu_time_now(), until)) {
	  asm(" nop");
	  // buzz...
  }

  set_led(false);
  mu_stddemo_printf("LED should be off for 5 seconds:\n");
  until = mu_time_offset(mu_time_now(), MU_TIME_MS_TO_DURATION(5000));
  while (mu_time_precedes(mu_time_now(), until)) {
      asm(" nop");
	  // buzz...
  }

  mu_stddemo_printf("Press button to toggle LED:\n");
}

void test_stddemo_step(void) {
  if (s_button_was_pressed == true) {
    s_button_was_pressed = false;
    bool led_state = !get_led();
    set_led(led_state);
    mu_stddemo_printf("LED is %s\n", led_state ? "on" : "off");
  } else {
    asm(" nop");
  }
}

// =============================================================================
// Local (private) code

static void button_cb(bool button_is_pressed) {
  (void)button_is_pressed;
  s_button_was_pressed = true;
}

static void set_led(bool on) {
  s_led_state = on;
  mu_stddemo_led_set(on);
}

static bool get_led(void) { return s_led_state; }
