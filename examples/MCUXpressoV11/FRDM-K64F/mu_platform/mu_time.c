/**
 * MIT License
 *
 * Copyright (c) 2021 R. Dunbar Poor <rdpoor@gmail.com>
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
// Includes

#include "mu_config.h"
#include "mu_time.h"

// =============================================================================
// Private types and definitions

// =============================================================================
// Private (forward) declarations

// =============================================================================
// Local storage

volatile uint32_t s_systick_count;

// =============================================================================
// Public code

/**
 * @brief Initialize the time system.  Must be called before any other time
 * functions are called.
 */
void mu_time_init(void) {
  /* Set systick reload value to generate 1ms interrupt */
  if (SysTick_Config(SystemCoreClock / 1000U)) {
    while (1) {
			// SysTick_Config failed
    }
	}
}

/**
 * @brief Get the current system time.
 *
 * @return A value representing the current time.
 */
mu_time_t mu_rtc_now() { return s_systick_count; }

/**
 * @brief Add a time and a duration.
 *
 * `mu_time_offset` adds a time and a duration to produce a new time object.
 *
 * @param t1 a time object
 * @param dt a duration object
 * @return t1 offset by dt
 */
mu_time_t mu_time_offset(mu_time_t t1, mu_duration_t dt) { return t1 + dt; }

/**
 * @brief Take the difference between two time objects
 *
 * `mu_time_difference` subtracts t2 from t1 to produce a duration object.
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return (t1-t2) as a duration object
 */
mu_duration_t mu_time_difference(mu_time_t t1, mu_time_t t2) { return t1 - t2; }

/**
 * @brief Return true if t1 is strictly before t2
 *
 * Note that if you want to know if t1 is before or equal to t2, you can use the
 * construct `!mu_time_follows(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly before t2, false otherwise.
 */
bool mu_time_precedes(mu_time_t t1, mu_time_t t2) { return t1 < t2; }

/**
 * @brief Return true if t1 is equal to t2
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 equals t2, false otherwise.
 */
bool mu_time_equals(mu_time_t t1, mu_time_t t2) { return t1 == t2; }

/**
 * @brief Return true if t1 is strictly after t2
 *
 * Note that if you want to know if t1 is equal to or after t2, you can use the
 * construct `!mu_time_precedes(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly after t2, false otherwise.
 */
bool mu_time_follows(mu_time_t t1, mu_time_t t2) { return t1 > t2; }

/**
 * @brief Convert a duration to milliseconds.
 *
 * @param dt A duration object
 * @return The duration in seconds
 */
mu_duration_ms_t mu_time_duration_to_ms(mu_duration_t dt) {
  return (dt * 1000) / SYSTICK_FREQUENCY;
}

/**
 * @brief Convert milliseconds to a duration
 *
 * @param ms The duration in milliseconds
 * @return A duration object
 */
mu_duration_t mu_time_ms_to_duration(mu_duration_ms_t ms) {
  return (ms * SYSTICK_FREQUENCY) / 1000;
}

#ifdef MU_FLOAT
/**
 * @brief Convert a duration to seconds.
 *
 * @param dt A duration object
 * @return The duration in seconds
 */
MU_FLOAT mu_time_duration_to_s(mu_duration_t dt) {
  return dt / (MU_FLOAT)(SYSTICK_FREQUENCY);
}

/**
 * @brief Convert seconds to a duration.
 *
 * @param s The duration in seconds
 * @return A duration object
 */
mu_duration_t mu_time_s_to_duration(MU_FLOAT s) { return s * SYSTICK_FREQUENCY; }

#endif

// =============================================================================
// Local code


void SysTick_Handler(void) {
	s_systick_count += 1;
}
