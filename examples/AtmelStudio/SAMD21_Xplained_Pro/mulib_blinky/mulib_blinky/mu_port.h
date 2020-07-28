/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

/**
 * @file mu_port.h
 *
 * Every target system for mulib is assumed to provide a few resources:
 *
 * * An LED that can be turned on and off
 * * A user button that can generate an asynchronous event
 * * A clock / counter that can be used as a time base for all time-dependent
 *   functions and generate an asynchronous event
 * * A serial port that supports asynchronous read and write operations
 * * [Optional] The ability to sleep (i.e. run in a power-reduced mode) until
 *   an asynchronous event wakes it.
 *
 * This file, port.h, defines the interface to those resources.  To port mulib
 * to a new target system, you must implement the functions and types described
 * here.
 *
 * In addition, some compile-time macros control the behavior of port.h:
 *
 * * `MU_PORT_FLOAT`: If your system supports floating point operations, define
 *   this to be `float` or `double` as appropriate.  Note that including
 *   floating point operations may increase the size of your image.
 * * `MU_PORT_CAN_SLEEP`: If your system supports a low-power sleep mode,
 *   define this macro and include defintions for `mu_port_sleep_until()` and
 *   `mu_port_sleep()`
 */

#ifndef _MU_PORT_H_
#define _MU_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// types and definitions

#undef MU_PORT_FLOAT
/**
 * If your port supports floating point operations, choose one of the following
 * and implement mu_port_time_duration_to_s() and mu_port_time_s_to_duration().
 */
// #define MU_PORT_FLOAT float
// #define MU_PORT_FLOAT double

/**
 * Define the data type that holds a time value and a duration value.  Using
 * 32 bit values is a good choice for many platforms, but you can change this
 * as needed.
 */
typedef uint32_t mu_port_time_t;
typedef int32_t mu_port_time_dt;
typedef int32_t mu_port_time_ms_dt;
#ifdef MU_PORT_FLOAT
typedef MU_PORT_FLOAT mu_port_time_seconds_dt;
#endif

/**
 * If your platform is able to sleep in order to conserver power, un-comment
 * MU_PORT_CAN_SLEEP and define mu_port_sleep_until() and mu_port_sleep().
 */
#define MU_PORT_CAN_SLEEP

typedef void (*mu_port_callback_fn)(void *arg);

// =============================================================================
// declarations

void mu_port_init(void);

// TIME

mu_port_time_t mu_port_time_offset(mu_port_time_t t, mu_port_time_dt dt);
mu_port_time_dt mu_port_time_difference(mu_port_time_t a, mu_port_time_t b);
bool mu_port_time_precedes(mu_port_time_t a, mu_port_time_t b);
bool mu_port_time_equals(mu_port_time_t a, mu_port_time_t b);
bool mu_port_time_follows(mu_port_time_t a, mu_port_time_t b);
mu_port_time_ms_dt mu_port_time_duration_to_ms(mu_port_time_dt dt);
mu_port_time_dt mu_port_time_ms_to_duration(mu_port_time_ms_dt ms);

#ifdef MU_PORT_FLOAT
mu_port_time_seconds_dt mu_port_time_duration_to_s(mu_port_time_dt dt);
mu_port_time_dt mu_port_time_s_to_duration(mu_port_time_seconds_dt seconds);
#endif

// ==========
// Real Time Clock

mu_port_time_t mu_port_rtc_now(void);
void mu_port_rtc_set_cb(mu_port_callback_fn fn, void *arg);
void mu_port_rtc_alarm_at(mu_port_time_t at);

// ==========
// LED

void mu_port_led_set(bool on);
bool mu_port_led_get(void);

// ==========
// BUTTON

bool mu_port_button_is_pressed(void);
void mu_port_button_set_cb(mu_port_callback_fn fn, void *arg);

// ==========
// SERIAL I/O

/**
 * @brief Non-blocking write to the serial port.
 *
 * Initiate a write operation on the serial port.  The actual transfer happens
 * in the background, with a callback generated upon completion.  Until the
 * operation completes, mu_port_serial_can_write() will return false.
 *
 * Note: since buf may be used directly, do not modify its contents until
 * mu_port_serial_can_write() returns true or the write callback fires.
 */
bool mu_port_serial_write(const uint8_t *const buf, int n_bytes);

/**
 * @brief Return true if the previous call to mu_port_serial_write has completed.
 */
bool mu_port_serial_can_write(void);

/**
 * @brief Return the number of bytes that have been written since the most
 * recent call to mu_port_serial_write().
 *
 * Note: since writing happens asynchronously, this function may underestimate
 * the number of bytes written.
 */
int mu_port_serial_write_count(void);

/**
 * Register a callback to be called when a write operation completes.
 */
void mu_port_serial_set_write_cb(mu_port_callback_fn fn, void *arg);

/**
 * @brief Non-blocking read from the serial port.
 *
 * Initiate a read operation of up to n_bytes from the serial port.   The
 * data transfer happens in the background with a callback generated after
 * n_bytes have been read.  In the interim, mu_port_serial_read_count() will
 * indicate how many bytes have been read.
 */
bool mu_port_serial_read(uint8_t *const buf, int n_bytes);

/**
 * @brief Return true if a call to mu_port_serial_read() would return at least
 * one character.
 */
bool mu_port_serial_can_read(void);

/**
 * @brief Return the number of bytes that have been read since the most
 * recent call to mu_port_serial_read().
 *
 * Note: since reading happens asynchronously, this function may underestimate
 * the number of bytes read.
 */
int mu_port_serial_read_count(void);

/**
 * Register a callback to be called after a read operation has read n_bytes.
 */
void mu_port_serial_set_read_cb(mu_port_callback_fn fn, void *arg);

// ==========
// SLEEP

#ifdef MU_PORT_CAN_SLEEP
/**
 * @brief Put the processor into low-power mode until time t arrives, or an
 * external event wakes the processor.
 */
void mu_port_sleep_until(mu_port_time_t t);

/**
 * @brief Put the processor into low-power mode until an external event wakes
 * the processor.
 */
void mu_port_sleep(void);

#endif


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_PORT_H_ */
