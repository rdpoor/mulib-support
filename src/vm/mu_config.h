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

#ifndef _MU_CONFIG_H_
#define _MU_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>

// =============================================================================
// types and definitions

// #define MU_LOG_ENABLED
// #define MU_THUNK_PROFILING
// #define MU_VM_CAN_SLEEP

/**
 * If your port supports floating point operations, choose one of the following
 * either by uncommenting one of the following lines, or by setting the symbol
 * in the compiler.
 */
// #define MU_VM_FLOAT float
#define MU_VM_FLOAT double

typedef uint32_t mu_vm_time_t;
typedef int32_t mu_vm_time_dt;
typedef int32_t mu_vm_time_ms_dt;

// =============================================================================
// Everything below this line is deduced from the settings above this line.

#ifdef MU_THUNK_PROFILING
#define MU_THUNK_PROFILING (1)
#else
#define MU_THUNK_PROFILING (0)
#endif

#ifdef MU_VM_CAN_SLEEP
#define MU_VM_CAN_SLEEP (1)
#else
#define MU_VM_CAN_SLEEP (0)
#endif

#ifdef MU_VM_FLOAT
  #define MU_VM_HAS_FLOAT (1)
#else
  #define MU_VM_HAS_FLOAT (0)
#endif

#if defined(MU_VM_FLOAT) && ((MU_VM_FLOAT == float) || (MU_VM_FLOAT == double))
  typedef MU_VM_FLOAT mu_vm_float_t;
#else
  #error MU_VM_FLOAT must be either float or double
#endif

#if (MU_VM_HAS_FLOAT == 1)
typedef mu_vm_float_t mu_vm_time_s_dt;
#endif

// =============================================================================
// declarations

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_CONFIG_H_ */
