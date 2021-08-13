# A mildly opinionated style guide to MuLib

MuLib strives to be a framework that lets you do what you want without getting
in your way.  And while MuLib doesn't tell you how to structure your code, there
are a few MuLib-specific programming patterns that have proven to be durable and
useful.  This document presents a few of these patterns in the form of spelled-
out examples.

Every embedded system starts with the canonical "blink an LED" program, and this
style guide is no different.  But we use that as a starting point for more
complex and different kinds of tasks.

All of the following examples assume you have a "board support package"  (bsp)
that hides the details of controlling LEDs, and assume that you have provided
the function:

```
bsp_set_led(led_id_t led_id, bool on);
```

that takes two arguments: the `led_id` specifies which LED, the `on` argument
turns the LED on if true and turns it off if false.

# Pattern 1: A singleton task that reschedules itself

`blinky_one` is an example of a singleton task that periodically reschedules
itself.  It turns an LED on for 10 milliseconds and off for 490 milliseconds,
repeating ad infinitum.

## file: main.c

```
#include "mulib.h"       // the MuLib library
#include "bsp.h"         // your Board Support Package
#include "blinky_one.h"  // declarations for the blinky_one task

#define ON_TIME_MS 10   // the LED on time (in milliseconds)
#define OFF_TIME_MS 490 // the LED off time

void main(void) {
  bsp_init();       // whatever your system need to initialize from a cold start
  mu_time_init();   // initialize the timer functions
  mu_sched_init();  // set up the scheduler

  // initialize and queue the blinky_one task
  mu_sched_task_now(blinky_one_init(ON_TIME_MS, OFF_TIME_MS));

  // Run the scheduler forever
  while (1) {
    mu_sched_step();
  }  
}
```

## file: blinky_one.h
```
#ifndef _BLINKY_ONE_H_
#define _BLINKY_ONE_H_

// Just in case someone wants to integrate your code into a C++ program, it's
// handy to include this preamble and corresponding postamble.  In a pure C
// program, it has no effect
#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"
#include <stdint.h>  // defines uint16_t

// =============================================================================
// types and definitions

// =============================================================================
// declarations

/**
 * @brief Return a blinky-one task, that when run, blinks an LED.
 *
 * This returns a mu_task_t object that, when run in the scheduler, periodically
 * turns an LED on for `on_time_ms` milliseconds and then off for `off_tiem_ms`.
 *
 * @param on_time_ms The length of time the LED should be on, in milliseconds.
 * @param off_time_ms The length of time the LED should be off.
 * @return A mu_task_t object to be scheduled.
 */
mu_task_t *blinky_one_init(uint16_t on_time_ms, uint16_t off_time_ms);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _BLINKY_ONE_H_ */
```

## file: blinky_one.c
```
// =============================================================================
// includes

#include "blinky_one.h"
#include "mulib.h"
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// local types and definitions

// Define the context for a blinky_one task.  When the blinky_one_fn is called,
// this context is passed in as an argument, and gives blinky_one_fn all of the
// information it needs to run the task.
typedef struct {
  mu_task_t task;
  mu_duration_t on_time;
  mu_duration_t off_time;
  bool led_is_on;
} blinky_one_ctx_t;

// =============================================================================
// local storage

static blinky_one_ctx_t s_blinky_one_ctx;

// =============================================================================
// local (forward) declarations

// Provide a forward declaration to the blinky_one_fn()
static void *blinky_one_fn(void *ctx, void *arg);

// =============================================================================
// public code

mu_task_t *blinky_one_init(uint16_t on_time_ms, off_time_ms) {

  // initialize the mu_task to bind the blinky_one_fn and s_blinky_one_ctx  
  mu_task_init(&s_blinky_one_ctx.task, blinky_one_fn, &s_blinky_one_ctx, "Blinky One");

  // Set up other state in s_blinky_one_ctx
  s_blinky_one.on_time = mu_time_ms_to_dt(on_time_ms);
  s_blinky_one.off_time = mu_time_ms_to_dt(off_time_ms);
  s_blinky_one_ctx.led_is_on = false;

  // Make sure the LED is initially off
  bsp_set_led(LED_A, false);

  // Return a pointer to the task so the caller can schedule it.
  return &s_blinky_one_ctx.task;
}

// =============================================================================
// local (static) code

static void blinky_one_fn(void *ctx) {
  blinky_one_ctx_t *self = (blinky_one_ctx_t *)ctx;
  if (self->led_is_on) {
    // turn off the LED and re-run the task after off_time has elapsed
    self->led_is_on = false
    bsp_set_led(LED_A, false);
    mu_reschedule_in(self->off_time);
  } else {
    // turn on the LED and re-run the task after on_time has elapsed
    self->led_is_on = true;
    bsp_set_led(LED_A, true);
    mu_reschedule_in(self->on_time);
  }
}
```

If you prefer dense code, this is an equivalent version of `blinky_one_fn()`:

```
static void blinky_one_fn(void *ctx) {
  // Recast the void * argument to a blinky_one_ctx_t * argument.
  blinky_one_ctx_t *self = (blinky_one_ctx_t *)ctx;

  // Toggle the internal state and make the LED match the internal state
  bsp_set_led(LED_A, (self->led_is_on = !self->led_is_on));

  // Reschedule the currently running task in the specified amount of time.
  mu_reschedule_in(self->led_is_on ? self->on_time : self->off_time);
}
```

# Pattern 2: Multiple tasks sharing the same code

What if you wanted something like blinky_one, but for three LEDs that each have
their own blink pattern?  The solution is nearly identical to blinky_one, but
rather than hiding the context structure inside blinky_one.c, we expose it in
the blinky_two.h header file so the user can allocate a context for each LED.  

Like this...

## file: main.c

```
#include "mulib.h"       // the MuLib library
#include "bsp.h"         // your Board Support Package
#include "blinky_two.h"  // declarations for the blinky_two task

static blinky_two_ctx_t led_a;
static blinky_two_ctx_t led_b;
static blinky_two_ctx_t led_c;

void main(void) {
  bsp_init();
  mu_time_init();

  mu_sched_task_now(blinky_two_init(&led_a, 10, 490, LED_A, "LED A Task"));
  mu_sched_task_now(blinky_two_init(&led_b, 10, 590, LED_B, "LED B Task"));
  mu_sched_task_now(blinky_two_init(&led_c, 10, 690, LED_C, "LED C Task"));

  // Run the scheduler forever
  while (1) {
    mu_sched_step();
  }  
}
```

## file: blinky_two.h
```
#ifndef _BLINKY_TWO_H_
#define _BLINKY_TWO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"
#include "bsp.h"     // defines led_id_t
#include <stdint.h>  // defines uint16_t

// =============================================================================
// types and definitions

// State required for the blinky_two_fn.  Note that in blinky_one, the context
// was private to the blinky_one.c file and not exposed to the caller.  In the
// non-singleton case, the caller must pass in a pointer to the context so we
// can have a context for each LED.  So we define the context in the .h file
// rather than in the .c file.

typedef struct {
  mu_task_t task;
  mu_duration_t on_time;
  mu_duration_t off_time;
  bool led_is_on;
  led_id_t led_id;      // field added to indicate which LED.
} blinky_two_ctx_t;

// =============================================================================
// declarations

/**
 * @brief Return a blinky-two task, that when run, blinks an LED.
 *
 * This returns a mu_task_t object that, when run in the scheduler, periodically
 * turns an LED on for `on_time_ms` milliseconds and then off for `off_time_ms`.
 *
 * @param ctx A pointer to blinky_two_ctx_t structure which will be filled in.
 * @param on_time_ms The length of time the LED should be on, in milliseconds.
 * @param off_time_ms The length of time the LED should be off.
 * @param led_id Defines which LED to be blinked.
 * @param task_name For debugging only, the name of the task.
 * @return A mu_task_t object to be scheduled.
 */
mu_task_t *blinky_two_init(&blinky_two_ctx_t *ctx,
                           uint16_t on_time_ms,
                           uint16_t off_time_ms
                           led_id_t led_id,
                           const char const *task_name);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _BLINKY_TWO_H_ */
```

## file: blinky_two.c
```
// =============================================================================
// includes

#include "blinky_two.h"
#include "mulib.h"
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local storage

// =============================================================================
// local (forward) declarations

static void *blinky_two_fn(void *ctx, void *arg);

// =============================================================================
// public code

mu_task_t *blinky_two_init(&blinky_two_ctx_t *ctx,
                           uint16_t on_time_ms,
                           uint16_t off_time_ms
                           led_id_t led_id,
                           const char const *task_name) {
  // initialize the mu_task to bind the blinky_two_fn and ctx  
  mu_task_init(&ctx->task, blinky_two_fn, ctx, task_name);

  // Set up other state in ctx
  ctx->on_time = mu_time_ms_to_dt(on_time_ms);
  ctx->off_time = mu_time_ms_to_dt(off_time_ms);
  ctx->led_id = led_id;
  ctx->led_is_on = false;

  // Make sure the LED is initially off
  bsp_set_led(led_id, false);

  // return the task
  return ctx->task;
}

// =============================================================================
// local (static) code

static void blinky_two_fn(void *ctx) {
  // Recast the void * argument to a blinky_two_ctx_t * argument.
  blinky_two_ctx_t *self = (blinky_two_ctx_t *)ctx;

  // Toggle the internal state and make the LED match the internal state
  self->led_is_on = !self->led_is_on;
  bsp_set_led(self->led_id, self->led_is_on);

  // Reschedule the blinky_two task itself.
  mu_reschedule_in(self->led_is_on ? self->on_time : self->off_time);
}
```

# Pattern 3: Managing a sequence of events

We have an LED, but simply blinking it on and off is a bit boring.  What
interesting stuff can we do with an LED?  Morse, of course.

This example takes an ASCII character and (if recognized) blinks that character
in Morse code.  The `morse_char_task` introduces two new concepts:

* Using `switch/case` statements to implement a state machine
* Calling another task when this task completes.

## file: main.c

```
#include "mulib.h"            // the MuLib library
#include "bsp.h"              // your Board Support Package
#include "morse_char_task.h"  // declarations for the morse_char_task
#include <stddef.h>           // defines NULL

static morse_char_task_ctx_t s_morse_char_task_ctx;

void main(void) {
  bsp_init();       // whatever your system need to initialize from a cold start
  mu_time_init();   // initialize the timer functions
  mu_sched_init();  // set up the scheduler

  // initialize and queue the morse_char_task to blink the letter "A"
  mu_sched_task_now(morse_char_task_init(&s_morse_char_task_ctx,
                                         'A',
                                         LED_A,
                                         NULL,
                                         "Morse Char"));

  // Run the scheduler forever
  while (1) {
    mu_sched_step();
  }  
}
```

## file: morse_char_task.h
```
#ifndef _MORSE_CHAR_TASK_H_
#define _MORSE_CHAR_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"

// =============================================================================
// types and definitions

// Define Morse timing constants
#define MORSE_CHAR_QUANTUM MU_TIME_MS_TO_DT(100)
#define MORSE_SHORT_MARK (1 * MORSE_CHAR_QUANTUM)
#define MORSE_LONG_MARK (3 * MORSE_CHAR_QUANTUM)
#define MORSE_INTRA_CHAR_GAP (1 * MORSE_CHAR_QUANTUM)
#define MORSE_INTER_CHAR_GAP (3 * MORSE_CHAR_QUANTUM)
#define MORSE_INTER_WORD_GAP (7 * MORSE_CHAR_QUANTUM)

// Define the context for a morse_char_task.
typedef struct {
  mu_task_t task;            // the task object
  char *s;                   // the character being printed
  led_id_t led_id,           // the ID of the LED doing the blinking
  mu_task_t *on_completion;  // a task to call upon completion
} morse_char_task_ctx_t;

// =============================================================================
// declarations

/**
 * @brief Return a morse_char_task, that when run, converts an ASCII character
 * into a series of Morse code blinks.
 *
 * @param ctx Pointer to a morse_char_task context.
 * @param ch The ASCII character to be converted into Morse.
 * @param led_id Which LED to blink.
 * @param on_completion If non-NULL, a task to be invoked upon completion.
 * @param task_name name assigned to this task (for debugging only).
 * @return A mu_task_t object to be scheduled.
 */
mu_task_t *morse_char_task_init(morse_char_task_ctx_t *ctx,
                                char ch,
                                led_id_t led_id,
                                mu_task_t on_completion,
                                const char const *task_name);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MORSE_CHAR_TASK_H_ */
```

## file: morse_char_task.c
```
// =============================================================================
// includes

#include "morse_char_task.h"
#include "mulib.h"
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// local types and definitions

#define UNKNOWN_CH ". . - - . ." // aka '?'

// =============================================================================
// local storage

// Translate ASCII characters into a string of dots and dashes.  Use UNKNOWN_CH
// when there is no Morse code equivalent.
static const char const *s_ascii_to_itu_morse[] = {
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
// local (forward) declarations

// Provide a forward declaration to the morse_char_task_fn()
static void *morse_char_task_fn(void *ctx, void *arg);

/**
 * @brief translate an ASCII character into a string of dot and dashes.
 */
static const char const *get_morse_string(char ch);

/**
 * @brief Convert a-z to A-Z, leave other values untouched.
 */
static char upcase(char ch);

// =============================================================================
// public code

mu_task_t *morse_char_task_init(morse_char_task_ctx_t *ctx,
                                char ch,
                                led_id_t led_id,
                                mu_task_t on_completion,
                                const char const *task_name) {

  // bind the morse_char_task_fn and context  
  mu_task_init(&ctx->task, morse_char_task_fn, ctx, task_name);

  // Set up other state in s_morse_char_task_ctx
  ctx->s = get_morse_string(ch);
  ctx->led_id = led_id;
  ctx->on_completion = on_completion;

  // Make sure the LED is initially off
  bsp_set_led(self->led_id, false);

  // return the task
  return &ctx->task;
}

// =============================================================================
// local (static) code

static void morse_char_task_fn(void *ctx) {
  // Recast the void * argument to a morse_char_task_ctx_t * argument.
  morse_char_task_ctx_t *self = (morse_char_task_ctx_t *)ctx;

  switch(*self->s++) {
    case '.':  // dot: turn LED on for MORSE_SHORT_MARK
    bsp_set_led(self->led_id, true);
    mu_sched_reschedule_in(MORSE_SHORT_MARK);
    break;

    case '-':  // dash: turn LED on for MORSE_LONG_MARK
    bsp_set_led(self->led_id, true);
    mu_sched_reschedule_in(MORSE_LONG_MARK);
    break;

    case ' ':  // intra-character: turn LED off for MORSE_INTRA_CHAR_GAP
    bsp_set_led(self->led_id, false);
    mu_sched_reschedule_in(MORSE_INTRA_CHAR_GAP);
    break;

    case '\0':
    // Arrive here when the character has been emitted: turn the LED off and
    // call the on_completion task after MORSE_INTER_CHAR_GAP
    bsp_set_led(self->led_id, false);
    if (self->on_completion != NULL) {
      mu_sched_task_in(self->on_completion, MORSE_INTER_CHAR_GAP);
    }
    break;
  }
}

static const char const *get_morse_string(char ch) {
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

```

# Pattern 4: Coroutines

As shown above, `morse_char_task` blinks one Morse character at a time, but what
if you want to write a whole string, like "Hello, World!"?

This example shows how a higher-level task (`morse_task`) can call a sub-task
(`morse_char_task`).  In this case, `morse_task` is initialized with a string to
be rendered in Morse code, and it invokes `morse_char_task` to blink each char
in turn.  As `morse_char_task` completes blinking each char, it invokes its
`on_callback` argument to ask `morse_task` for the next character.

This example assumes you already have morse_char_task.c and morse_char_task.h.

## file: main.c

```
#include "mulib.h"            // the MuLib library
#include "bsp.h"              // your Board Support Package
#include "morse_task.h"       // declarations for the morse_task
#include <stddef.h>           // defines NULL

static morse_task_ctx_t s_morse_task_ctx;

void main(void) {
  bsp_init();       // whatever your system need to initialize from a cold start
  mu_time_init();   // initialize the timer functions
  mu_sched_init();  // set up the scheduler

  // initialize and queue the morse_task to blink the phrase "Hello, World!"
  mu_sched_task_now(morse_task_init(&s_morse_task_ctx,
                                    "Hello, World!",
                                    LED_A,
                                    NULL,
                                    "Morse"));

  // Run the scheduler forever
  while (1) {
    mu_sched_step();
  }  
}
```

## file: morse_task.h
```
#ifndef _MORSE_TASK_H_
#define _MORSE_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"

// =============================================================================
// types and definitions

// Define the context for a morse_task.
typedef struct {
  mu_task_t task;
  morse_char_task_ctx_t morse_char_ctx; // morse_task has a morse_char_task
  char *s;                              // The string to render as morse.
  mu_task_t *on_completion;             // The task to invoke on completion.
} morse_task_ctx_t;

// =============================================================================
// declarations

/**
 * @brief Return a morse_task, that when run, converts an ASCII string
 * into a series of Morse code blinks.
 *
 * @param ctx Pointer to a morse_task context.
 * @param s A null-terminated ASCII string to be converted into Morse.
 * @param led_id Which LED to blink
 * @param on_completion If non-NULL, a task to be invoked upon completion.
 * @param task_name name assigned to this task (for debugging only).
 * @return A mu_task_t object to be scheduled.
 */
mu_task_t *morse_task_init(morse_task_ctx_t *ctx,
                           const char const *s,
                           led_id_t led_id,
                           mu_task_t *on_completion,
                           const char const *task_name);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MORSE_TASK_H_ */
```

## file: morse_task.c
```
// =============================================================================
// includes

#include "morse_task.h"
#include "mulib.h"
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local storage

// =============================================================================
// local (forward) declarations

// Provide a forward declaration to the morse_task_fn()
static void *morse_task_fn(void *ctx, void *arg);

// =============================================================================
// public code

mu_task_t *morse_task_init(morse_task_ctx_t *ctx,
                           const char const *s,
                           led_id_t led_id,
                           mu_task_t *on_completion,
                           const char const *task_name) {

  // initialize the mu_task to bind the morse_task_fn and ctx  
  mu_task_init(&ctx->task, morse_task_fn, ctx, "Morse");

  // Set up other state in s_morse_char_task_ctx
  ctx->s = s;
  ctx->on_completion = on_completion;

  // return the task
  return &s_morse_char_task_ctx.task;
}

// =============================================================================
// local (static) code

static void morse_task_fn(void *ctx) {
  // Recast the void * argument to a morse_task_ctx_t * argument.
  morse_task_ctx_t *self = (morse_task_ctx_t *)ctx;

  char ch = *self->s++;   // the character to be generated in Morse

  if (ch != NULL) {
    // Start the morse_char_task to emit one char.  Since we pass &self->task as
    // the on_completion argument, mu_char_task will invoke this task when it
    // finishes emitting the char.
    mu_schedule_task_now(morse_char_task_init(&self->morse_char_ctx,
                                              ch,
                                              &self->task, // on_completion
                                              "Morse Char" // task name
                                              ));
  } else {
    // Arrive here when the entire string has been emitted.  Normally, we would
    // immediately invoke the on_completion task (if provided), but in Morse
    // code, we always want a seven-unit pause at the end of each word.  So we
    // schedule the on_completion task after MORSE_INTER_WORD_GAP.
    if (self->on_completion != NULL) {
      mu_sched_task_in(self->on_completion, MORSE_INTER_WORD_GAP);
    }
  }
}
```

# Pattern 5: Joins

In a multi-tasking environment, you sometimes need to spawn two or more
sub-tasks, then perform some other action only after all of the sub-tasks have
completed.

In this example, we create a `morse_join` task that spawns several
`morse_task`s, each generating a different phrase in Morse code.  When the last
`morse_task` completes, `morse_join` invokes its own `on_completion` task.

## file: main.c

```
#include "mulib.h"            // the MuLib library
#include "bsp.h"              // your Board Support Package
#include "morse_task.h"       // declarations for the morse_task
#include "morse_join.h"       // declarations for the morse_task
#include <stddef.h>           // defines NULL

#define ISR_QUEUE_SIZE 8
static mu_spsc_item_t s_isr_queue_items[ISR_QUEUE_SIZE];

// Three tasks to blink out "Hello, World" in different languages
static morse_task_ctx_t s_english_task_ctx;
static morse_task_ctx_t s_francais_task_ctx;
static morse_task_ctx_t s_espanol_task_ctx;

// ... and one task to join them all
static joiner_ctx_t s_joiner_ctx;

void main(void) {
  bsp_init();       // whatever your system need to initialize from a cold start
  mu_time_init();   // initialize the timer functions
  mu_sched_init(s_isr_queue_items, ISR_QUEUE_SIZE);  // set up the scheduler

  joiner_init(&s_joiner_ctx, NULL, NULL);
  joiner_add_task(&s_joiner_ctx, morse_task_init(&s_english_task_ctx,
                                                 "Hello, World!",
                                                 LED_A,
                                                 joiner_task(&s_joiner_ctx),
                                                 "English"));
  joiner_add_task(&s_joiner_ctx, morse_task_init(&s_francais_task_ctx,
                                                 "Bonjour, Monde!",
                                                 LED_B,
                                                 joiner_task(&s_joiner_ctx),
                                                 "Francais"));
  joiner_add_task(&s_joiner_ctx, morse_task_init(&s_espanol_task_ctx,
                                                 "Hola, Mundo!",
                                                 LED_C,
                                                 joiner_task(&s_joiner_ctx),
                                                 "Espanol"));

  mu_sched_task_now(joiner_task(&s_joiner_ctx));

  // Run the scheduler forever
  while (1) {
    mu_sched_step();
  }  
}
```

## file: joiner.h
```
#ifndef _JOINER_H_
#define _JOINER_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"
#include <stdint.h>

// =============================================================================
// types and definitions

// Define the context for a joiner.
typedef struct {
  mu_task_t task;
  uint16_t pending_count;    // # of pending tasks
  mu_task_t *on_completion;  // task to call when all pending tasks complete
} joiner_ctx_t;

// =============================================================================
// declarations

/**
 * @brief Initialize the joiner
 */
void joiner_init(joiner_ctx_t *ctx, mu_task_t *on_completion);

mu_task_t *joiner_add_task(joiner_ctx_t *ctx, mu_task_t *task);

mu_task_t *joiner_task(joiner_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _JOINER_H_ */
```

## file: joiner.c
```
// =============================================================================
// includes

#include "joiner.h"
#include "mulib.h"
#include <stdint.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local storage

// =============================================================================
// local (forward) declarations

// Provide a forward declaration to the morse_task_fn()
static void *joiner_fn(void *ctx, void *arg);

// =============================================================================
// public code

void joiner_init(joiner_ctx_t *ctx, mu_task_t *on_completion) {
  // bind the joiner_fn and ctx  
  mu_task_init(&ctx->task, joiner_fn, ctx, "Joiner");

  ctx->pending_count = 0;
  ctx->on_completion = on_completion;
}

mu_task_t *joiner_add_task(joiner_ctx_t *ctx, mu_task_t *task) {
  ctx->pending_count += 1;
  return task;
}

mu_task_t *joiner_task(joiner_ctx_t *ctx) {
  return ctx->task;
}

// =============================================================================
// local (static) code

// Arrive here when any one of the joined tasks complete.
static void joiner_fn(void *ctx) {
  // Recast the void * argument to a morse_task_ctx_t * argument.
  joiner_t *self = (joiner_t *)ctx;

  self->pending_count -= 1;

  if (self->pending_count == 0) {
    // all joined tasks have completed -- invoke the on_completion task
    if (self->on_completion != NULL) {
      mu_sched_task_now(self->on_completion);
    }
  }
}
```
