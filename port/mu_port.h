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

// =============================================================================
// types and definitions

#undef PORT_FLOAT
/**
 * If your port supports floating point operations, choose one of the following
 * and implement port_time_duration_to_s() and port_time_s_to_duration().
 */
// #define PORT_FLOAT float
// #define PORT_FLOAT double

/**
 * Define the data type that holds a time value and a duration value.
 */
typedef uint32_t mu_time_t;
typedef int32_t mu_time_dt;

#undef PORT_CAN_SLEEP
/**
 * If your port supports a low-power sleep mode, uncomment the following and
 * implement port_sleep_until() and port_sleep().
 */
// #define PORT_CAN_SLEEP

typedef void (*port_callback_fn)(void *arg);

// =============================================================================
// declarations

void port_init(void);

// TIME

port_time_t port_time_now(void);
port_time_t port_time_offset(port_time_t t, port_time_dt dt);
port_time_dt port_time_difference(port_time_t a, port_time_t b);
bool port_time_precedes(port_time_t a, port_time_t b);
bool port_time_follows(port_time_t a, port_time_t b);
bool port_time_equals(port_time_t a, port_time_t b);
int port_time_duration_to_ms(port_time_dt dt);
port_time_dt port_time_ms_to_duration(int ms);

#ifdef PORT_FLOAT
PORT_FLOAT port_time_duration_to_s(port_time_dt dt);
port_time_dt port_time_s_to_duration(PORT_FLOAT seconds);
#endif

// LED

void port_led_set(bool on);
bool port_led_get(void);

// BUTTON

bool port_button_is_pushed(void);
void port_button_set_cb(port_callback_fn fn, void *arg);

// SERIAL

/**
 * @brief Return true if a call to port_serial_write() would accept at least one
 * character.
 */
bool port_serial_can_write(void);

/**
 * @brief Non-blocking write to the serial port.
 *
 * Write up to n_bytes on the serial port.  Non-blocking, returns the number of
 * bytes written (which may be zero) or a negative number indicating an error.
 */
int port_serial_write(const char * buf, int n_bytes);

/**
 * Register a callback to be called when characters may be written to the
 * serial port.
 */
void port_serial_set_write_cb(port_callback_fn fn, void *arg);

/**
 * @brief Return true if a call to port_serial_read() would return at least one
 * character.
 */
bool port_serial_can_read(void);

/**
 * @brief Non-blocking read from the serial port.
 *
 * Read up to n_bytes from the serial port.  Returns the number of bytes read,
 * which may be zero if no bytes available or negative on an error.
 */
int port_serial_read(char *buf, int n_bytes);

/**
 * Register a callback to be called when characters become available for read.
 */
void port_serial_set_read_cb(port_callback_fn fn, void *arg);

// SLEEP

#ifdef PORT_CAN_SLEEP
/**
 * @brief Put the processor into low-power mode until time t arrives, or an
 * external event wakes the processor.
 */
void port_sleep_until(port_time_t t);

/**
 * @brief Put the processor into low-power mode until an external event wakes
 * the processor.
 */
void port_sleep(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_PORT_H_ */
