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

#ifndef MULIB_CHRON_H_
#define MULIB_CHRON_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// types and definitions

#ifndef CHRON_CLOCK_NBITS
#define CHRON_CLOCK_NBITS 32
#endif

#ifndef CHRON_FLOAT_TYPE
typedef double chron_float_t;
#endif

#if CHRON_CLOCK_NBITS == 32
typedef uint32_t chron_time_t;
typedef uint32_t chron_duration_t;
#define CHRON_CLOCK_MAX_DURATION ((chron_duration_t)0x80000000)
#endif


// =============================================================================
// declarations

/**
 * @brief Add a time and a duration.
 *
 * `chron_time_offset` adds a time and a duration to produce a new time object.
 *
 * @param t1 a time object
 * @param dt a duration object
 * @return t1 offset by dt
 */
chron_time_t chron_time_offset(chron_time_t t1, chron_duration_t dt);

/**
 * @brief Take the difference between two time objects
 *
 * `chron_time_difference` subtracts t2 from t1 to produce a duration object.
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return (t1-t2) as a duration object
 */
chron_duration_t chron_time_difference(chron_time_t t1, chron_time_t t2);

/**
 * @brief Return true if t1 is strictly before t2
 *
 * Note that if you want to know if t1 is before or equal to t2, you can use the
 * construct `!chron_time_is_after(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly before t2, false otherwise.
 */
bool chron_time_is_before(chron_time_t t1, chron_time_t t2);

/**
 * @brief Return true if t1 is equal to t2
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 equals t2, false otherwise.
 */
bool chron_time_is_equal(chron_time_t t1, chron_time_t t2);

/**
 * @brief Return true if t1 is strictly after t2
 *
 * Note that if you want to know if t1 is equal to or after t2, you can use the
 * construct `!chron_time_is_before(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly after t2, false otherwise.
 */
bool chron_time_is_after(chron_time_t t1, chron_time_t t2);

/**
 * @brief Convert a duration to the corresponding number of seconds as a chron_float_t.
 *
 * @param dt A duration object
 * @param clock_rate The clock rate of the counter in HZ
 * @return The duration in seconds
 */
chron_float_t chron_duration_to_seconds(chron_duration_t dt, chron_float_t clock_rate);

/**
 * @brief Convert a number of seconds to the corresponding duration
 *
 * @param s The duration in seconds
 * @param clock_rate The clock rate of the counter in HZ
 * @return A duration object
 */
chron_duration_t chron_seconds_to_duration(chron_float_t s, chron_float_t clock_rate);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MULIB_CHRON_H_
