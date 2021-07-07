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

#define MU_DISABLE_INTERRUPTS() do {} while(0)
#define MU_ENABLE_INTERRUPTS() do {} while(0)

/**
 * Uncomment if you want logging enabled.
 */
#define MU_LOG_ENABLED 1

/**
 * Define the number of tasks that can be scheduled at interrupt level between
 * calls to sched_step().  This must be define as a power of two.
 */
#define MU_IRQ_TASK_QUEUE_SIZE 8

/**
 * Uncomment if you want task profiling enabled.
 */
// #define MU_TASK_PROFILING

/**
 * If your port supports floating point operations, choose one of the following
 * either by uncommenting one of the following lines, or by setting the symbol
 * in the compiler.
 */
// #define MU_FLOAT float
// #define MU_FLOAT double

// =============================================================================
// Everything below this line is deduced from the settings above this line.

#define MU_WITH_INTERRUPTS_DISABLED(_body)                                     \
  MU_DISABLE_INTERRUPTS();                                                     \
  _body                                                                        \
  MU_ENABLE_INTERRUPTS();

// already defined in utils/utils_assert.h
// #ifndef ASSERT
// #define ASSERT(expr) mu_test_assert((expr), #expr, __FILE__, __LINE__)
// #endif

#ifdef MU_TASK_PROFILING
#define MU_TASK_PROFILING (1)
#else
#define MU_TASK_PROFILING (0)
#endif

#ifdef MU_FLOAT
  #define MU_HAS_FLOAT (1)
#else
  #define MU_HAS_FLOAT (0)
#endif

#if defined(MU_FLOAT)
  typedef MU_FLOAT mu_float_t;
#endif

// =============================================================================
// declarations

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_CONFIG_H_ */
