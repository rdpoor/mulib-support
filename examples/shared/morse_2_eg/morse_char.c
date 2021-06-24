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

// =============================================================================
// Includes

#include "morse_char.h"

#include <mulib.h>

// =============================================================================
// Local types and definitions

// Define Morse timing constants
#define MORSE_CHAR_QUANTUM MU_TIME_MS_TO_DURATION(100)
#define MORSE_SHORT_MARK (1 * MORSE_CHAR_QUANTUM)
#define MORSE_LONG_MARK (3 * MORSE_CHAR_QUANTUM)
#define MORSE_INTRA_CHAR_GAP (1 * MORSE_CHAR_QUANTUM)
#define MORSE_INTER_CHAR_GAP (5 * MORSE_CHAR_QUANTUM)
#define MORSE_INTER_WORD_GAP (11 * MORSE_CHAR_QUANTUM)

#define UNKNOWN_CH ". . - - . ." // aka '?'

// Define the context for a morse_char_task.
typedef struct {
  mu_task_t task;            // the task object
  const char *s;             // the character being printed
  mu_task_t *on_completion;  // a task to call upon completion
} ctx_t;

// =============================================================================
// Local (forward) declarations

static void task_fn(void *ctx, void *arg);

/**
 * @brief Convert a single ASCII char into a morse string of dots and dashes.
 *
 * @param ch The ASCII character to convert
 * @return A pointer to a string of '.', '-', and ' ' characters.
 */
static const char *get_morse_string(char ch);

/**
 * @brief Convert an ASCII character to upper case.
 */
static char upcase(char ch);

// =============================================================================
// Local storage

static ctx_t s_ctx;

// Translate ASCII characters into a string of dots and dashes.  Use UNKNOWN_CH
// when there is no Morse code equivalent.
static const char *s_ascii_to_itu_morse[] = {
    "       ",        // ' ' 32 (inter-word gap of 7 units)
    "- . - . - -",    // ! 33
    ". - . . - .",    // " 34
    UNKNOWN_CH,       // # 35
    UNKNOWN_CH,       // $ 36
    UNKNOWN_CH,       // % 37
    ". - . . .",      // & 38
    ". - - - - .",    // ' 39
    "- . - - .",      // ( 40
    "- . - - . -",    // ) 41
    UNKNOWN_CH,       // * 42
    ". - . - .",      // + 43
    "- - . . - -",    // , 44
    "- . . . . -",    // - 45
    ". - . - . -",    // . 46
    "- . . - .",      // / 47
    "- - - - -",      // 0 48
    ". - - - -",      // 1 49
    ". . - - -",      // 2 50
    ". . . - -",      // 3 51
    ". . . . -",      // 4 52
    ". . . . .",      // 5 53
    "- . . . .",      // 6 54
    "- - . . .",      // 7 55
    "- - - . .",      // 8 56
    "- - - - .",      // 9 57
    "- - - . . .",    // : 58
    UNKNOWN_CH,       // ; 59
    UNKNOWN_CH,       // < 60
    "- . . . -",      // = 61
    UNKNOWN_CH,       // > 62
    ". . - - . .",    // ? 63
    ". - - . - .",    // @ 64
    ". -",            // A 65
    "- . . .",        // B 66
    "- . - .",        // C 67
    "- . .",          // D 68
    ".",              // E 69
    ". . - .",        // F 70
    "- - .",          // G 71
    ". . . .",        // H 72
    ". .",            // I 73
    ". - - -",        // J 74
    "- . -",          // K 75
    ". - . .",        // L 76
    "- -",            // M 77
    "- .",            // N 78
    "- - -",          // O 79
    ". - - .",        // P 80
    "- - . -",        // Q 81
    ". - .",          // R 82
    ". . .",          // S 83
    "-",              // T 84
    ". . -",          // U 85
    ". . . -",        // V 86
    ". - -",          // W 87
    "- . . -",        // X 88
    "- . - -",        // Y 89
    "- - . .",        // Z 90
};

// =============================================================================
// Public code

mu_task_t *morse_char_init(char ascii, mu_task_t *on_completion) {
  // initialize the mu_task to associate blink_basic_fn with s_blink_basic_ctx
  mu_task_init(&s_ctx.task, task_fn, &s_ctx, "Morse Char");

  // Initialize s_ctx
  s_ctx.s = get_morse_string(ascii);
  s_ctx.on_completion = on_completion;

  // Make sure the LED is initially off
  mu_led_io_set(false);

  // Return the task object, ready to be passed to the scheduler
  return &s_ctx.task;
}

// =============================================================================
// Local (private) code

static void task_fn(void *ctx, void *arg) {
  // Recast the void * argument to a ctx_t * argument.
  ctx_t *self = (ctx_t *)ctx;
  (void)arg;  // unused

  switch(*self->s++) {
    case '.':  // dot: turn LED on for MORSE_SHORT_MARK
    mu_led_io_set(true);
    mu_sched_reschedule_in(MORSE_SHORT_MARK);
    break;

    case '-':  // dash: turn LED on for MORSE_LONG_MARK
    mu_led_io_set(true);
    mu_sched_reschedule_in(MORSE_LONG_MARK);
    break;

    case ' ':  // intra-character: turn LED off for MORSE_INTRA_CHAR_GAP
    mu_led_io_set(false);
    mu_sched_reschedule_in(MORSE_INTRA_CHAR_GAP);
    break;

    case '\0':
    // Arrive here when the character has been emitted: turn the LED off and
    // call the on_completion task after MORSE_INTER_CHAR_GAP
    mu_led_io_set(false);
    if (self->on_completion != NULL) {
      mu_sched_task_in(self->on_completion, MORSE_INTER_CHAR_GAP);
    }
    break;
  }
}

static const char *get_morse_string(char ch) {
  ch = upcase(ch);

  if ((ch < 32) || (ch > 90)) {
    return UNKNOWN_CH;
  } else {
    return s_ascii_to_itu_morse[ch - 32];
  }
}

static char upcase(char ch) {
  if ((ch >= 'a') && (ch <= 'z')) {
    return ch - 'a' + 'A';
  } else {
    return ch;
  }
}
