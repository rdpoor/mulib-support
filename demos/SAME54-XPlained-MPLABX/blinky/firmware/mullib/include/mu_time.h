/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
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

#ifndef _MU_TIME_H_
#define _MU_TIME_H_

#ifdef __cplusplus
extern "C";
#endif

// =============================================================================
// includes

#include <stdint.h>
#include <stdbool.h>
#include "port_time.h"

// =============================================================================
// types and definitions

typedef port_time_t mu_time_t;
typedef port_time_dt mu_time_dt;
typedef port_time_ms_dt mu_time_ms_dt;
typedef port_time_seconds_dt mu_time_seconds_dt;

// =============================================================================
// declarations

void mu_time_init(void);

/**
 * @brief Add a time and a duration.
 *
 * `mu_time_offset` adds a time and a duration to produce a new time object.
 *
 * @param t1 a time object
 * @param dt a duration object
 * @return t1 offset by dt
 */
mu_time_t mu_time_offset(mu_time_t t1, mu_time_dt dt);

/**
 * @brief Take the difference between two time objects
 *
 * `mu_time_difference` subtracts t2 from t1 to produce a duration object.
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return (t1-t2) as a duration object
 */
mu_time_dt mu_time_difference(mu_time_t t1, mu_time_t t2);

/**
 * @brief Return true if t1 is strictly before t2
 *
 * Note that if you want to know if t1 is before or equal to t2, you can use the
 * construct `!mu_time_is_after(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly before t2, false otherwise.
 */
bool mu_time_is_before(mu_time_t t1, mu_time_t t2);

/**
 * @brief Return true if t1 is equal to t2
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 equals t2, false otherwise.
 */
bool mu_time_is_equal(mu_time_t t1, mu_time_t t2);

/**
 * @brief Return true if t1 is strictly after t2
 *
 * Note that if you want to know if t1 is equal to or after t2, you can use the
 * construct `!mu_time_is_before(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly after t2, false otherwise.
 */
bool mu_time_is_after(mu_time_t t1, mu_time_t t2);

/**
 * @brief Convert a duration to milliseconds.
 *
 * @param dt A duration object
 * @return The duration in seconds
 */
mu_time_ms_dt mu_time_duration_to_ms(mu_time_dt dt);

/**
 * @brief Convert milliseconds to a duration
 *
 * @param ms The duration in milliseconds
 * @return A duration object
 */
mu_time_dt mu_time_ms_to_duration(mu_time_ms_dt ms);

/**
 * @brief Convert a duration to seconds.
 *
 * @param dt A duration object
 * @return The duration in seconds
 */
mu_time_seconds_dt mu_time_duration_to_seconds(mu_time_dt dt);

/**
 * @brief Convert seconds to a duration.
 *
 * @param s The duration in seconds
 * @return A duration object
 */
mu_time_dt mu_time_seconds_to_duration(mu_time_seconds_dt s);

/**
 * @brief Get the current system time.
 *
 * @return A value representing the current time.
 */
mu_time_t mu_time_now();

#ifdef __cplusplus
}
#endif

#endif // #ifndef _MU_TIME_H_
