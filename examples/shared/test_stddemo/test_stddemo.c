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


// =============================================================================
// Local storage

volatile static bool s_button_was_pressed;
static bool s_led_state;

// =============================================================================
// Public code

void test_stddemo_x(int b) {
    //printf("test_stddemo_x");
    s_button_was_pressed = true;
}

void test_stddemo_init(void) {
  mu_time_t until;

  mu_platform_init();
  mu_stddemo_init(button_cb);

  printf("\n\rmulib test_stddemo v%s.\n", VERSION);
  s_button_was_pressed = false;
  s_led_state = false;

  set_led(true);
  printf("LED should be on for 5 seconds:\n");
  until = mu_time_offset(mu_time_now(), MU_TIME_MS_TO_DURATION(5000));
  while (mu_time_precedes(mu_time_now(), until)) {
	  asm(" nop");
	  // buzz...
  }

  set_led(false);
  printf("LED should be off for 5 seconds:\n");
  until = mu_time_offset(mu_time_now(), MU_TIME_MS_TO_DURATION(5000));
  while (mu_time_precedes(mu_time_now(), until)) {
      asm(" nop");
	  // buzz...
  }

  printf("Press button to toggle LED:\n");
}

void test_stddemo_step(void) {
  if (s_button_was_pressed == true) {
    s_button_was_pressed = false;
    bool led_state = !get_led();
    set_led(led_state);
    printf("LED is %s\n", led_state ? "on" : "off");
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

void set_led(int on) {
  s_led_state = on;
  mu_stddemo_led_set(on);
}
int get_led(void) { return s_led_state; }
