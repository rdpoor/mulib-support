/*
 * platform_test.c
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 */

// =============================================================================
// Includes

#include "platform_test.h"
#include <mulib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mu_platform.h"

// =============================================================================
// Local types and definitions

#define VERSION "1.0"

// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);
void set_led(int on);
int get_led(void);

// =============================================================================
// Local storage

volatile static bool s_button_was_pressed;

// =============================================================================
// Public code

void platform_test_init(void) {
  mu_time_t until;

  mulib_init();
  //mu_platform_init();
  mu_button_io_set_callback(button_cb);

  mu_ansi_term_clear_screen();
  mu_ansi_term_set_foreground_color(MU_ANSI_TERM_YELLOW);
  mu_ansi_term_set_cursor_visible(false);

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
  printf("Press button to toggle LED:           \n");

  mu_begin_polling_for_keypress();
  atexit(mu_ansi_term_exit_noncanonical_mode); // restores terminal attributes
}

void platform_test_step(void) {
  mu_sched_step();
  unsigned char kp = mu_term_get_current_keypress();
  if(kp == 'q') exit(0);
  if(kp || s_button_was_pressed == true) {
    s_button_was_pressed = false;
    mu_led_io_set(MU_LED_0, !mu_led_io_get(MU_LED_0));
    printf("LED is %s                               \n", mu_led_io_get(MU_LED_0) ? "on" : "off");
  } else {
    asm(" nop");
  }
}

// =============================================================================
// Local (private) code


void set_led(int on) {
  mu_led_io_set(MU_LED_0,on);
}

int get_led(void) { 
  return mu_led_io_get(MU_LED_0); 
}

static void button_cb(uint8_t button_id, bool button_is_pressed) {
  (void)button_id;

  if (button_is_pressed) {
    s_button_was_pressed = true;
  }
}
