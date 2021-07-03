/*
 * platform_test.h
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 *
 * This program tests the platform-specific functions used by mulib. Note that
 * this program does not depend upon mulib itself -- it only verifies that the
 * low-level functions are working properly:
 *
 * - mu_button_io signals when a button is pressed or released
 * - mu_kbd_io signals when a key is pressed or released
 * - mu_led_io turns an LED on or off
 * - mu_rtc manages a real-time clock
 * - mu_time performs basic operations on time and durations.
 *
 * If this program runs properly for your platform, there's a high probabilty
 * that all mulib functions will work properly on your platform.
 */

#ifndef _PLATFORM_TEST_H_
#define _PLATFORM_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Includes

// =============================================================================
// Types and definitions

// =============================================================================
// Declarations

/**
 * @brief Called once at startup.
 */
void platform_test_init(void);

/**
 * @brief Called repeatedly from main().
 */
void platform_test_step(void);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _PLATFORM_TEST_H_
