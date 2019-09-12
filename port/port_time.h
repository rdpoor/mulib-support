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

#ifndef PORT_TIME_H_
#define PORT_TIME_H_

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

typedef clock_t port_time_t;    // an absolute time
typedef clock_t port_time_dt;   // the interval between two times

// =============================================================================
// declarations

port_time_t port_time_offset(port_time_t t, port_time_dt dt);
port_time_dt port_time_difference(port_time_t t1, port_time_t t2);

bool port_time_is_before(port_time_t t1, port_time_t t2);
bool port_time_is_equal(port_time_t t1, port_time_t t2);
bool port_time_is_after(port_time_t t1, port_time_t t2);

port_time_dt port_time_s_to_dt(float seconds);
float port_time_t_to_s(port_time_dt dt);

port_time_t port_time_now();



#ifdef __cplusplus
}
#endif

#endif // #ifndef PORT_TIME_H_
