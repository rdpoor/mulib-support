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

// =============================================================================
// includes

#include "mu_test_utils.h"
#include "extras/mu_random.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// =============================================================================
// private types and definitions
#define GIBBERISH_LENGTH 78

#define EXPECTED_EMISSION "M<-F::M?LN?#'IY7*T78@,3K=H9?HWK8J0+5GQ19W4?ZB;-4V=$/AR@,5*ACHM&8T;=N@;8;+8E6NV"

// =============================================================================
// private declarations

int display_gibberish();

// =============================================================================
// local storage


// =============================================================================
// public code

void mu_random_test() {
  ASSERT(display_gibberish() == 1);
}

int display_gibberish() {
  printf("\n");
  for(int i = 0; i < GIBBERISH_LENGTH; i++) {
    char a = (char)mu_random_range(35, 91);
    //fputc(a,stdout);
    if(a != EXPECTED_EMISSION[i]) {
      printf("\nmu_random() failed to match the expected random sequence for the default seed.\n");
      return 0;
    }
  }
  printf("\n");
  return 1;
}

// =============================================================================
// private code

