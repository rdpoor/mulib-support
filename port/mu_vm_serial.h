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
 * @file mu_vm_serial.h
 *
 * Sketches for serial routines for virtual machine.
 */

#ifndef _MU_VM_SERIAL_H_
#define _MU_VM_SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// types and definitions

// =============================================================================
// declarations

// ==========
// SERIAL I/O

/**
 * @brief Write a byte to the serial port.
 *
 * If a callback is set, works in synchronous mode and returns immediately.
 *
 * If no callback is set, works in asynchronous mode: it blocks until all of the
 * data in cirq has been sent.
 *
 * @param cirq A ring buffer containing the data to be transmitted.
 */
void mu_vm_serial_write(mu_cirq_t *cirq);

/**
 * @brief Register a callback to be called when the most recent all to
 * mu_vm_serial_write() has no more bytes to transmit.
 */
void mu_vm_serial_register_write_cb(mu_vm_callback_fn fn, void *arg);

/**
 * @brief Read serial data into a ring buffer.
 *
 * Works in synchronous mode if a callback is set.  The callback will be
 * triggered when one or more characters are received.
 *
 * Works in asynchronous mode if no callback is set.  It blocks until one or
 * more characters are received and stored in the ring buffer.
 *
 * @param cirq A ring buffer to receive the data.
 */
void mu_vm_serial_read(mu_cirq_t *cirq);

/**
 * @brief Register a callback to be called when a serial byte is received.
 */
void mu_vm_serial_set_read_cb(mu_vm_callback_fn fn, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_VM_SERIAL_H_ */
