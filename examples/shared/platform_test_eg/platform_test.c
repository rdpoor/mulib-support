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
#include <stdlib.h>

// =============================================================================
// Local types and definitions

#define VERSION "1.1"
#define DEMO_INTERVAL_SECS (5)

// =============================================================================
// Local (forward) declarations

/**
 * @brief Called from interrupt level when a button changes state.
 */
static void button_cb(uint8_t button_id, bool button_is_pressed);

/**
 * @brief Called from interrupt level when a key is received.
 */
static void kbd_cb(unsigned char ch);

/**
<<<<<<< HEAD
=======
 * @brief Called from interrupt level on RTC count match.
 */
static void rtc_cb(void);

/**
>>>>>>> platform-update
 * @brief Invert the state of the user LED.
 */
static void toggle_led(void);

// =============================================================================
// Local storage

// Linkage between interrupt level and foreground -- see button_cb(), kbd_cb()
// and platform_test_step().  These variables are declared volatile so the
// compiler won't optimize them away.
volatile static bool s_button_changed_state;
volatile static bool s_button_state;

volatile static bool s_key_pressed;
volatile static char s_last_char;

volatile static bool s_rtc_count_matched;

// =============================================================================
// Public code

void platform_test_init(void) {

  mu_platform_init();
  mu_button_io_set_callback(button_cb);
  mu_kbd_io_set_callback(kbd_cb);

  printf("\n\rmu_platform test v%s.\n", VERSION);
  s_button_changed_state = false;
  s_key_pressed = false;
  s_rtc_count_matched = false;

  // Turn LED on and then off to verify the RTC and time functions are working
  // properly...
  mu_led_io_set(MU_LED_0, true);

  printf("LED should be on for %d seconds (busy wait).\n", DEMO_INTERVAL_SECS);
  mu_rtc_busy_wait(MU_TIME_MS_TO_DURATION(DEMO_INTERVAL_SECS * 1000));

  mu_led_io_set(MU_LED_0, false);

  printf("LED should be off for %d seconds (match callback).\n", DEMO_INTERVAL_SECS);
  mu_rtc_set_match_count(
    mu_time_offset(mu_rtc_now(),
                   MU_TIME_MS_TO_DURATION(DEMO_INTERVAL_SECS * 1000)));
  mu_rtc_set_match_cb(rtc_cb);
  while (!s_rtc_count_matched) {
	  asm("nop");    // buzz...
  }
  s_rtc_count_matched = false;

  mu_led_io_set(MU_LED_0, true);

  printf("Press button or any key...\n");
}

void platform_test_step(void) {
  // Called continually from the main loop.  Here we test mu_button_io and
  // mu_kbd_io to make sure they are working properly.
  if (s_button_changed_state) {
    s_button_changed_state = false;
    printf("button %s\n", s_button_state ? "down" : "up");
    toggle_led();
  }

  if (s_key_pressed) {
    s_key_pressed = false;
    printf("key 0x%02x pressed\n", s_last_char);
    toggle_led();
  }
}

// =============================================================================
// Local (private) code

static void button_cb(uint8_t button_id, bool button_is_pressed) {
  // Called at interrupt level when a button changes state.  Note that we do
  // not want to call printf() or any time-consuming operations from within
  // interrupt level.  Instead, we just set some variables in order to notify
  // the main loop after the interrupt returns -- see platform_test_step().
  (void)button_id;   // unused.
  s_button_state = button_is_pressed;
  s_button_changed_state = true;
}

static void kbd_cb(unsigned char ch) {
  // Called at interrupt level when a key changes state.  Note that we do
  // not want to call printf() or any time-consuming operations from within
  // interrupt level.  Instead, we just set some variables in order to notify
  // the main loop after the interrupt returns -- see platform_test_step().
  s_last_char = ch;
  s_key_pressed = true;
}

static void rtc_cb(void) {
  s_rtc_count_matched = true;
}

static void toggle_led(void) {
  mu_led_io_set(MU_LED_0, !mu_led_io_get(MU_LED_0));
}
