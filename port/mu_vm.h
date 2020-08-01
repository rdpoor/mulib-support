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
 * @file mu_vm.h
 *
 * Every target system for mulib is assumed to provide a few resources:
 *
 * * An LED that can be turned on and off
 * * A user button that can generate an asynchronous event
 * * A clock / counter that can be used as a time base for all time-dependent
 *   functions and generate an asynchronous event
 * * A serial port that supports byte at a time read and write operations, and
 *   generates an asynchronous callback when a byte becomes available for read.
 * * [Optional] The ability to sleep (i.e. run in a power-reduced mode) until
 *   an asynchronous event wakes it.
 *
 * This file, port.h, defines the interface to those resources.  To port mulib
 * to a new target system, you must implement the functions and types described
 * here.
 *
 * In addition, some compile-time macros control the behavior of port.h:
 *
 * * `MU_VM_FLOAT`: If your system supports floating point operations, define
 *   this to be `float` or `double` as appropriate.  Note that including
 *   floating point operations may increase the size of your image.
 * * `MU_VM_CAN_SLEEP`: If your system supports a low-power sleep mode,
 *   define this macro and include defintions for `mu_vm_sleep_until()` and
 *   `mu_vm_sleep()`
 */

#ifndef _MU_VM_H_
#define _MU_VM_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// types and definitions

#undef MU_VM_FLOAT
/**
 * If your port supports floating point operations, choose one of the following
 * and implement mu_vm_time_duration_to_s() and mu_vm_time_s_to_duration().
 */
// #define MU_VM_FLOAT float
// #define MU_VM_FLOAT double

/**
 * Define the data type that holds a time value and a duration value.  Using
 * 32 bit values is a good choice for many platforms, but you can change this
 * as needed.
 */
typedef uint32_t mu_vm_time_t;
typedef int32_t mu_vm_time_dt;
typedef int32_t mu_vm_time_ms_dt;
#ifdef MU_VM_FLOAT
typedef MU_VM_FLOAT mu_vm_time_seconds_dt;
#endif

/**
 * If your platform is able to sleep in order to conserver power, un-comment
 * MU_VM_CAN_SLEEP and define mu_vm_sleep_until() and mu_vm_sleep().
 */
#define MU_VM_CAN_SLEEP

typedef void (*mu_vm_callback_fn)(void *arg);

// =============================================================================
// declarations

void mu_vm_init(void);

// TIME

mu_vm_time_t mu_vm_time_offset(mu_vm_time_t t, mu_vm_time_dt dt);
mu_vm_time_dt mu_vm_time_difference(mu_vm_time_t a, mu_vm_time_t b);
bool mu_vm_time_precedes(mu_vm_time_t a, mu_vm_time_t b);
bool mu_vm_time_equals(mu_vm_time_t a, mu_vm_time_t b);
bool mu_vm_time_follows(mu_vm_time_t a, mu_vm_time_t b);
mu_vm_time_ms_dt mu_vm_time_duration_to_ms(mu_vm_time_dt dt);
mu_vm_time_dt mu_vm_time_ms_to_duration(mu_vm_time_ms_dt ms);

#ifdef MU_VM_FLOAT
mu_vm_time_seconds_dt mu_vm_time_duration_to_s(mu_vm_time_dt dt);
mu_vm_time_dt mu_vm_time_s_to_duration(mu_vm_time_seconds_dt seconds);
#endif

// ==========
// Real Time Clock

mu_vm_time_t mu_vm_rtc_now(void);
void mu_vm_rtc_set_cb(mu_vm_callback_fn fn, void *arg);
void mu_vm_rtc_alarm_at(mu_vm_time_t at);

// ==========
// LED

void mu_vm_led_set(bool on);
bool mu_vm_led_get(void);

// ==========
// BUTTON

bool mu_vm_button_is_pressed(void);
void mu_vm_button_set_cb(mu_vm_callback_fn fn, void *arg);

// ==========
// SERIAL I/O

/**
 * @brief Write a byte to the serial port.
 *
 * This function will block (busy wait) until the UART hardware can accept the
 * byte.  You can use the mu_vm_serial_can_write() function to find if this
 * function will block.
 *
 * Alternatively, you can set a callback using `mu_vm_serial_set_write_cb()`,
 * which will be called when the UART is ready to accept a character.
 */
void mu_vm_serial_write(uint8_t byte);

/**
 * @brief Return true if mu_vm_serial_write() will return immediately without
 * blocking.
 */
bool mu_vm_serial_can_write(void);

/**
 * Register a callback to be called after a write operation completes, which
 * indicates that mu_vm_serial_write() will not block.
 */
void mu_vm_serial_set_write_cb(mu_vm_callback_fn fn, void *arg);

/**
 * Return true if a serial byte is in the process of being transmitted, i.e. a
 * byte has been written to the UART but it is not yet fully transmitted. This
 * function is primarily intended to indicate whether it is permissable to
 * put the processor to sleep.
 */
bool mu_vm_serial_write_in_progress(void);

/**
 * @brief Read a byte from the serial port.
 *
 * If no data is available, this function will block until a character is
 * received.
 *
 * If a read callback has been set via mu_vm_serial_set_read_cb, serial read
 * interrtups are enabled after the read operation, otherwise read interrupts
 * are disabled.
 *
 * For blocking style reads (without interrupts):
 *   if (!mu_vm_serial_can_read()) {
 *     // return to the scheduler...
 *   } else {
 *     uint8_t ch = mu_vm_serial_read();
 *     // process character
 *   }
 *
 * For non-blocking style reads (using interrupts):
 *   mu_vm_serial_set_read_cb(serial_cb, NULL);
 *   // return to the scheduler...
 *   ...
 *   void serial_cb(void *arg) {
 *     // schedule a task to call mu_vm_serial_read()...
 *   }
 */
uint8_t mu_vm_serial_read(void);

/**
 * @brief Return true if mu_serial_read() will return immediately without
 * blocking.
 */
bool mu_vm_serial_can_read(void);

/**

 * Register a callback to be called when the serial port has a new byte
 * available and enable serial read interrupts.
 */
void mu_vm_serial_set_read_cb(mu_vm_callback_fn fn, void *arg);

/**
 * Return true if a serial byte is in the process of being received, i.e. a
 * start bit has been detected but the byte is not yet fully received.  This
 * function is primarily intended to indicate whether it is permissable to
 * put the processor to sleep.
 */
bool mu_vm_serial_read_in_progress(void);

// ==========
// SLEEP

#ifdef MU_VM_CAN_SLEEP
/**
 * @brief Put the processor into low-power mode until time t arrives, or an
 * external event wakes the processor.
 */
void mu_vm_sleep_until(mu_vm_time_t t);

/**
 * @brief Put the processor into low-power mode until an external event wakes
 * the processor.
 */
void mu_vm_sleep(void);

#endif


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_VM_H_ */
