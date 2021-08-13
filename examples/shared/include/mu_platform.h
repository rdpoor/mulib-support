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

#ifndef _MU_PLATFORM_H_
#define _MU_PLATFORM_H_

// About mu_platform.h:
//
// mu_platform contains the platform specific code required by the mulib system.
//
// Like mulib.h, mu_platform.h offers build strategies, referred to as the
// "prix fixe" approach and the "ala carte" approach.
//
// The _prix fixe_ approach is the easiest: if you include "mulib.h" and call
// "mu_init()" when your code starts up, everything required from mu_platform
// is provided for you.
//
// But if you want the smallest possible code impage, you'll want to use the
// _ala carte_ approach.  In this scheme, you `#include` only the mu_platform
// modules that your application needs.  In addition, for each module named
// `xxx`, you may need to call `xxx_init()` if that module requires it.

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_button_io.h"
#include "mu_config.h"
#include "mu_kbd_io.h"
#include "mu_led_io.h"
#include "mu_rtc.h"
#include "mu_time.h"

// =============================================================================
// types and definitions

void mu_platform_init(void);

// =============================================================================
// declarations

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_PLATFORM_H_ */
