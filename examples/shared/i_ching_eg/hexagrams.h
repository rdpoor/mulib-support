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

#ifndef HEXAGRAMS_H_
#define HEXAGRAMS_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <mulib.h>
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// types and definitions

typedef struct {
  char *pd;
  char *cm;
} i_ching_line;

typedef struct {
  unsigned char number; // 1 - 64, corresponding to the KWS chart
  uint8_t sk; // holds the bit encoding of the lines
  char *name;
  char *cm;
  char *jd;
  char *j_cm;
  char *im;
  char *i_cm;
  i_ching_line lines[7]; // 7th line seems to speak to the special case when all 6 lines are changing (old)
} i_ching_hexagram;

// =============================================================================
// declarations
int hexagram_number_from_user_lines(char *user_lines);
char *change_user_lines(char *user_lines);
void draw_multiple_user_lines(char *user_lines[], int how_many, int width, int height);
void print_analaysis_of_changing_lines(char *user_lines);
const i_ching_hexagram *get_hexagram(int number);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef HEXAGRAMS_H_ */


