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
 * This file, mu_vm.h.h, defines the interface to those resources. To port mulib
 * to a new target system, you must implement the functions and types described
 * here.
 *
 * In addition, some compile-time macros defined in mu_vm_config.h control the
 * behavior of mu_vm.h:
 *
 * * `MU_VM_HAS_FLOAT`: If your system supports floating point operations,
 *   define this to be true.  Note that including floating point operations may
 *   increase the size of your image.
 * * `MU_VM_HAS_DOUBLE`: If your system supports double operations, define this
 *   to be true.  Note that including double operations may increase the size of
 *   your image.
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

#include "mu_vm_config.h"    // must come first!

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// types and definitions

/**
 * @brief Signature for a generic callback function.
 *
 * @param arg A user-defined "pointer sized" argument.
 */
typedef void (*mu_vm_callback_fn)(void *arg);

/**
 * @brief Signature for serial write callbacks.
 *
 * This will be called at interrupt level whenever the serial subsystem is ready
 * to accept a new byte for writing.
 */
typedef void (*mu_vm_serial_write_cb)(void);

/**
 * @brief Signature for serial read callbacks.
 *
 * This will be called at interrupt level whenever the serial subsystem has
 * received a new byte.
 */
typedef void (*mu_vm_serial_read_cb)(uint8_t data);

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

#if (MU_VM_HAS_FLOAT)
mu_vm_time_s_dt mu_vm_time_duration_to_s(mu_vm_time_dt dt);
mu_vm_time_dt mu_vm_time_s_to_duration(mu_vm_time_s_dt seconds);
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
 * @brief Set the serial write callback function.
 *
 * @param cb The callback to be invoked when the serial subsystem is ready to
 *           accept a new byte for writing.  Passing NULL disables the callback.
 */
void mu_vm_serial_set_write_cb(mu_vm_serial_write_cb cb);

/**
 * @brief Is the system ready for a call to mu_vm_serial_write()?
 *
 * @return true if the serial subsystem is ready to accept a new byte for
 * writing.
 */
bool mu_vm_serial_is_ready_to_write(void);

/**
 * @brief Initiate a write a byte to the serial subsystem.
 *
 * This should only be called if mu_vm_serial_is_ready_to_write() returns true,
 * or from within a serial write callback.  Otherwise, previously written data
 * may be over-written, leading to garbled results.
 *
 * After sending the character is complete, the write callback will be called.
 *
 * @param data byte to be written.
 */
void mu_vm_serial_write(uint8_t data);

/**
 * @brief Is the serial subsystem busy writing?
 *
 * @return true if the serial subsystem has initiated but not yet completed a
 * write operation.
 */
bool mu_vm_serial_write_is_in_progress(void);

/**
 * @brief Set the serial read callback function.
 *
 * @param cb the callback function to be invoked when the serial subsystem has
 * received a new byte.  Passing NULL disables the callback.
 */
void mu_vm_serial_set_read_cb(mu_vm_serial_read_cb cb);

/**
* @brief Is the system ready for a call to mu_vm_serial_read()?
 *
 * @return true if a call to vm_serial_read() will return a valid value.
 */
bool mu_vm_serial_is_ready_to_read(void);

/**
 * @brief Read a byte from the serial subsystem.
 *
 * This should only be called if mu_vm_serial_is_ready_to_read() returns true.
 * Otherwise, it may return garbled data or repeat the previous byte.
 */
uint8_t mu_vm_serial_read(void);

/**
 * @brief Is the serial subsystem busy reading?
 *
 * @return true if the serial subsystem has initiated but not yet completed a
 * read operation.
 */
bool mu_vm_serial_read_is_in_progress(void);

// ==========
// SLEEP

#if (MU_VM_CAN_SLEEP)

/**
 * @brief Return true unless one or more of the vm resources are busy.
 */
bool mu_vm_is_ready_to_sleep(void);

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
