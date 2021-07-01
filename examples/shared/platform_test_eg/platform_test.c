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

#define VERSION "1.1"
#define DEMO_INTERVAL_SECS (5.0)

// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);
static void kbd_cb(unsigned char ch);
void set_led(int on);
int get_led(void);

// =============================================================================
// Local storage

volatile static bool s_button_was_pressed;
static bool user_hit_key = false;
static char _most_recent_character = 'x';

// =============================================================================
// Public code

void platform_test_init(void) {

  mu_time_t until;

  mulib_init();
  //mu_platform_init();
  mu_button_io_set_callback(button_cb);
  mu_kbd_io_set_callback(kbd_cb);

  mu_ansi_term_clear_screen();
  mu_ansi_term_set_cursor_visible(false);

  printf("\n\rmulib platform_test v%s.\n", VERSION);
  s_button_was_pressed = false;

  mu_led_io_set(MU_LED_0, true);
  printf("LED should be on for %2.2f seconds.\n", DEMO_INTERVAL_SECS);
  until = mu_time_offset(mu_rtc_now(), MU_TIME_MS_TO_DURATION(DEMO_INTERVAL_SECS * 1000));
  while (mu_time_precedes(mu_rtc_now(), until)) {
	  asm(" nop");
	  // buzz...
  }

  mu_led_io_set(MU_LED_0, false);
  printf("LED should be off for %2.2f seconds.\n", DEMO_INTERVAL_SECS);
  until = mu_time_offset(mu_rtc_now(), MU_TIME_MS_TO_DURATION(DEMO_INTERVAL_SECS * 1000));
  while (mu_time_precedes(mu_rtc_now(), until)) {
      asm(" nop");
	  // buzz...
  }
  mu_ansi_term_clear_screen();
  mu_led_io_set(MU_LED_0, true);
  printf("LED is %s\n", mu_led_io_get(MU_LED_0) ? "on" : "off");
  printf("Press button or key to toggle LED.");
  printf("\n'q' or CTRL-C to quit.");
}

void platform_test_step(void) {
  mu_sched_step();

  if(user_hit_key || s_button_was_pressed == true) {
    if(_most_recent_character == 'q') exit(0);
    s_button_was_pressed = user_hit_key = false;
    mu_led_io_set(MU_LED_0, !mu_led_io_get(MU_LED_0));
    printf("LED is %s", mu_led_io_get(MU_LED_0) ? "on" : "off");
    mu_ansi_term_clear_to_end_of_line();
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

static void kbd_cb(unsigned char ch) {
  // TODO -- calling this from POSIX thread violates the single thread thing -- really should just call sched_isr_task_now BUT we cant store ch without violating this...
  _most_recent_character = ch; 
  user_hit_key = true;
}

