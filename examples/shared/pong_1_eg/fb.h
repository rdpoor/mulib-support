/**
 * MIT License
 *
 * Copyright (c) 2020 R. D. Poor <rdpoor@gmail.com>
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
 * @file fb.h
 *
 * Frame Buffer uses a double buffer to paint ASCII art onto a screen.
 */

#ifndef FB_H_
#define FB_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>

// =============================================================================
// types and definitions

// =============================================================================
// declarations

/**
 * @brief Initialize the frame buffer with backing store and display store.
 */
void fb_init(int width,
             int height,
             char *backing_store,
             char *display_store);

/**
 * @brief erase the screen and reset the backing store.
 */
void fb_erase(void);

/**
 * @brief Clear the frame buffer backing store.
 */
void fb_clear(void);


/**
 * @brief Clear from some internal point to the end of that line
 */
void fb_clear_to_end_of_line(char *pos);

/**
 * @brief Return a pointer to the first char of the given row #.
 */
char *fb_row_ref(int row_number);

/**
 * @brief Draw character into the frame buffer backing store at x, y
 */
void fb_draw(int x, int y, char ch);

/**
 * @brief Flush the frame buffer backing store to the screen
 *
 * In theory, this only paints characters that have changed, but that's an
 * implementation detail...
 */
void fb_flush(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef FB_H_ */
