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

#ifndef MU_PORT_H_
#define MU_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

// =============================================================================
// includes
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// types and definitions

typedef clock_t mu_port_time_t;    // an absolute time
typedef clock_t mu_port_time_dt;   // the interval between two times
typedef double mu_port_time_seconds_t;

// =============================================================================
// declarations

// called only when MU_ASSERT() is enabled
void port_assert(const bool condition,
                 const char *const expr,
                 const char *const file,
                 const int line);

mu_port_time_t mu_port_time_offset(mu_port_time_t t, mu_port_time_dt dt);

mu_port_time_dt mu_port_time_difference(mu_port_time_t t1, mu_port_time_t t2);

bool mu_port_time_is_before(mu_port_time_t t1, mu_port_time_t t2);

bool mu_port_time_is_equal(mu_port_time_t t1, mu_port_time_t t2);

bool mu_port_time_is_after(mu_port_time_t t1, mu_port_time_t t2);

mu_port_time_seconds_t mu_port_time_seconds_to_duration(mu_port_time_seconds_t seconds);

mu_port_time_dt mu_port_time_duration_to_seconds(mu_port_time_dt dt);

mu_port_time_t mu_port_time_now();

void mu_port_sleep_indefinitely();

void mu_port_sleep_until(mu_port_time_t t);

#ifdef __cplusplus
}
#endif

#endif // #ifndef PORT_H_
