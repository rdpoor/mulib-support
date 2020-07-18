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
 * Define the data type that holds a time value and a duration value.
 */
typedef uint32_t mu_port_time_t;
typedef int32_t mu_port_time_dt;

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
int mu_port_time_duration_to_ms(mu_port_time_dt dt);
mu_port_time_dt mu_port_time_ms_to_duration(int ms);

#ifdef PORT_FLOAT
PORT_FLOAT mu_port_time_duration_to_s(mu_port_time_dt dt);
mu_port_time_dt mu_port_time_s_to_duration(PORT_FLOAT seconds);
#endif

// Real Time Clock

mu_port_time_t mu_port_rtc_now(void);
void mu_port_rtc_set_cb(mu_port_callback_fn fn, void *arg);
void mu_port_rtc_alarm_at(mu_port_time_t at);

// LED

void mu_port_led_set(bool on);
bool mu_port_led_get(void);

// BUTTON

bool mu_port_button_is_pressed(void);
void mu_port_button_set_cb(mu_port_callback_fn fn, void *arg);

// SERIAL

/**
 * @brief Return true if the previous call to mu_port_serial_write has completed.
 */
bool mu_port_serial_can_write(void);

/**
 * @brief Non-blocking write to the serial port.
 *
 * Write up to n_bytes on the serial port.  Non-blocking, returns the number of
 * bytes written (which may be zero) or a negative number indicating an error.
 */
int mu_port_serial_write(const uint8_t *const buf, int n_bytes);

/**
 * Register a callback to be called when characters may be written to the
 * serial port.
 */
void mu_port_serial_set_write_cb(mu_port_callback_fn fn, void *arg);

/**
 * @brief Return true if a call to mu_port_serial_read() would return at least one
 * character.
 */
bool mu_port_serial_can_read(void);

/**
 * @brief Non-blocking read from the serial port.
 *
 * Read up to n_bytes from the serial port.  Returns the number of bytes read,
 * which may be zero if no bytes available or negative on an error.
 */
int mu_port_serial_read(uint8_t *const buf, int n_bytes);

/**
 * Register a callback to be called when characters become available for read.
 */
void mu_port_serial_set_read_cb(mu_port_callback_fn fn, void *arg);

// SLEEP

#ifdef PORT_CAN_SLEEP
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
