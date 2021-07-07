/**
 * ansi_nico_font.c
 *
 *
 */

// =============================================================================
// Includes
#include <stdio.h>
#include <string.h>
#include "fb.h"

// =============================================================================
// Local types and definitions


// =============================================================================
// Local storage
#if 0
static int nico_font_line_count = 5;
const char *nico_font[] = {

"||||||||||||"
"|||      |||"
"|||      |||"
"|||      |||"
"||||||||||||",

"  |||||     "
"    |||     "
"    |||     "
"    |||     "
" |||||||||  ",

"||||||||||||"
"         |||"
"||||||||||||"
"|||         "
"||||||||||||",

"||||||||||||"
"         |||"
"||||||||||||"
"         |||"
"||||||||||||",

"|||      |||"
"|||      |||"
"||||||||||||"
"         |||"
"         |||",

"||||||||||||"
"|||         "
"||||||||||||"
"         |||"
"||||||||||||",

"||||||||||||"
"|||         "
"||||||||||||"
"|||      |||"
"||||||||||||",

"||||||||||||"
"        ||| "
"      |||   "
"    |||     "
"  |||       ",

"||||||||||||"
"|||      |||"
"||||||||||||"
"|||      |||"
"||||||||||||",

"||||||||||||"
"|||      |||"
"||||||||||||"
"         |||"
"||||||||||||",

"       "
"  |||  "
"       "
"  |||  "
"       ",
};

#endif

static int nico_font_line_count = 5;
const char *nico_font[] = {

" |||||||| "
"|||    |||"
"|||    |||"
"|||    |||"
" |||||||| ",

"  |||||   "
"    |||   "
"    |||   "
"    |||   "
" |||||||||",

"||||||||| "
"       |||"
" |||||||| "
"|||       "
"||||||||||",

"||||||||| "
"       |||"
"||||||||| "
"       |||"
"||||||||| ",

"|||    |||"
"|||    |||"
"||||||||||"
"       |||"
"       |||",

"||||||||||"
"|||       "
"||||||||| "
"       |||"
"||||||||| ",

" |||||||| "
"|||       "
"||||||||| "
"|||    |||"
" |||||||| ",

"||||||||||"
"       |||"
"     |||  "
"   |||    "
" |||      ",

" |||||||| "
"|||    |||"
" |||||||| "
"|||    |||"
" |||||||| ",

" |||||||| "
"|||    |||"
" |||||||||"
"       |||"
" |||||||| ",

"   "
"|||"
"   "
"|||"
"   ",
};

static int nico_font_index_for_char(char c) {
  if(c == ':') return 10;
  if(c < '0' || c > '9') return 10; // to avoid a crash
  return c - '0';
}

void print_string_using_nico_font(char *wut) {
  for(int i = 0; i < nico_font_line_count; i++) {
    char *fb_line_start = fb_row_ref(i);
    if(!fb_line_start) {
      printf("print_string_using_nico_font() needs fb_init(width, height) to have been called.\n");
      return;
    }
    for(int ci = 0; ci < strlen(wut); ci++) {
      char c = wut[ci];
      const char *s = nico_font[nico_font_index_for_char(c)];
      int line_len = strlen(s) / nico_font_line_count;
      sprintf(fb_line_start, "%.*s ", line_len, s + (line_len * i));
      fb_line_start += line_len + 1;
    }
    fb_clear_to_end_of_line(fb_line_start);
  }
  fb_flush();
}
