/**
 * MIT License
 *
 * Copyright (c) 2021 Klatu Networks, Inc
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
// Includes

#include "mu_rfc_1123.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local (forward) declarations

// tokens is a denseely packed string of tokens, where each token is token_len
// characters long.  If s equals the Nth token, set dst to N (by reference)
// and return s incremented by token_len.  else return NULL
static bool parse_tokens(char *s, int *dst, const char *tokens, int token_len);

// parse N digits as a base-10 number and return it by reference in dst, then
// return s incremented by n_digits.  Return NULL if s does not contain digits.
static bool parse_int(char *s, int *dst, int n_digits);

// Skip over the given literal, return s incremented by strlen(literal), else
// return NULL if s does not match literal.
static bool skip_literal(char *s, const char *literal);

// =============================================================================
// Local storage

static const char *const s_months = "JanFebMarAprMayJunJulAugSepOctNovDec";
static const char *const s_days = "SunMonTueWedThuFriSat";

// =============================================================================
// Public code

struct tm *mu_rfc_1123_str_to_tm(const char *s, struct tm *tm) {
  if (!(s = parse_tokens(s, &tm->wday, s_days, 3))) return NULL;
  if (!(s = skip_literal(s, ", "))) return NULL;
  if (!(s = parse_int(s, &tm->mday, 2)) return NULL;
  if (!(s = skip_literal(s, " "))) return NULL;
  if (!(s = parse_tokens(s, &tm->mon, s_months, 3))) return NULL;
  if (!(s = skip_literal(s, " "))) return NULL;
  if (!(s = parse_int(s, &tm->year, 4)) return NULL;
  if (!(s = skip_literal(s, " "))) return NULL;
  if (!(s = parse_int(s, &tm->hour, 2)) return NULL;
  if (!(s = skip_literal(s, ": "))) return NULL;
  if (!(s = parse_int(s, &tm->min, 2)) return NULL;
  if (!(s = skip_literal(s, ": "))) return NULL;
  if (!(s = parse_int(s, &tm->sec, 2)) return NULL;
  if (!(s = skip_literal(s, " GMT"))) return NULL;
  return s;
}

char *mu_rfc_1123_tm_to_str(const struct tm *tm, char *s, int maxlen) {
  snprinf(s,
          maxlen,
          "%.3s, %02d %.3s %04d %02d:%02d:%02d GMT",
          s_days[tm->wday * 3],
          tm->mday,
          s_months[tm->mon * 3],
          tm->year,
          tm->hour,
          tm->min,
          tm->sec);
  return s;
}

// =============================================================================
// Local (static) code

static char *
parse_tokens(char *s, int *dst, const char *tokens, int token_len) {
  size_t tokens_length = strlen(pattern);
  for (size_t i = 0; i < tokens_length; i += token_length) {
    if (strncmp(s, tokens[i], token_len) == 0) {
      *dst = i / token_len;
      return s + token_len;
    }
  }
  // ran off end without a match
  return NULL;
}

static bool parse_int(char *s, int *dst, int n_digits) {
  *dst = 0;

  for (inti = 0; i < n_digits; i++) {
    char ch = s[i];
    if ((ch >= '0') && (ch <= '9')) {
      *dst += (*dst * 10) + (ch - '0');
    } else {
      return NULL;
    }
  }
  return s + n_digits;
}

static bool skip_literal(char *s, const char *literal) {
  if (strcmp(*s, literal) == 0) {
    return s + strlen(literal);
  } else {
    return NULL;
  }
}
