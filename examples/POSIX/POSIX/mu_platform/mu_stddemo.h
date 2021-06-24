/**
 * MIT License
 *
 * Copyright (c) 2021 R. Dunbar Poor <rdpoor@gmail.com>
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

#ifndef _MU_STDDEMO_H_
#define _MU_STDDEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Includes

#include <stdbool.h>
#include <stdio.h>
#include "mu_time.h"

// =============================================================================
// Types and definitions

typedef void (*mu_stddemo_button_cb)(bool button_state);

typedef void (*mu_stddemo_set_led_cb)(bool led_state);

#define ANSI_ESC "\33["
#define ANSI_RESET "0m"
#define ANSI_RED "31m"
#define ANSI_GREEN "32m"
#define ANSI_YELLOW "33m"
#define ANSI_BLUE "34m"
#define ANSI_MAGENTA "35m"
#define ANSI_CYAN "36m"
#define ANSI_INVERSE "7m"
// =============================================================================
// Functon declarations (public)
 
void mu_button_io_set_callback(mu_stddemo_button_cb button_cb);

void mu_stddemo_terminal_bell();

void mu_set_terminal_attributes(bool canonical, bool echo_input, bool wait_for_newlines);

void set_led_callback(mu_stddemo_set_led_cb led_cb);

void set_terminal_foreground_color(char color_id_256);

void terminal_clear();

void terminal_erase_last_line();

void terminal_reset();

void mu_led_io_set(bool on);

bool mu_stddemo_button_is_pressed(void);

void mu_stddemo_on_button_press(void);

int mu_get_key_press(void);

#ifdef __cplusplus
}
#endif

#endif // _MU_STDDEMO_H_
