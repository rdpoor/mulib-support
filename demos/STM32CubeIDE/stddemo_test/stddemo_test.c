/*
 * stddemo_test.c
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 */


// =============================================================================
// Includes

#include "stddemo_test.h"
#include "mu_platform.h"
#include <stdio.h>
#include <stdbool.h>

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

static bool s_button_was_pressed;
static bool s_led_state;

// =============================================================================
// Public code

void stddemo_test_init(void) {
	mu_stddemo_init(button_cb);

	printf("\n\rmulib stddemo test v%s.\n", VERSION);
	s_button_was_pressed = false;
	s_led_state = false;

	set_led(true);
	printf("LED should be on for 5 seconds:\n");
	mu_stddemo_sleep_until(mu_time_offset(mu_time_now(), MU_TIME_MS_TO_DURATION(5000)));

	set_led(false);
	printf("LED should be off for 5 seconds:\n");
	mu_stddemo_sleep_until(mu_time_offset(mu_time_now(), MU_TIME_MS_TO_DURATION(5000)));

	printf("Press button to toggle LED:\n");
}

void stddemo_test_step(void) {
	if (s_button_was_pressed == true) {
		s_button_was_pressed = false;
		bool led_state = !get_led();
		set_led(led_state);
		printf("LED is %s\n", led_state ? "on" : "off");
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

static bool get_led(void) {
	return s_led_state;
}
