/**
 * @file mu_rtc.c
 *
 * The Kinetis K64 family of processors have a low-power 16 bit timer which is
 * driven from a 32768 Hz clock source with a prescaler that divides it down to
 * 1024 Hz.  This means that it rolls over onces every 64 seconds.
 *
 * And although the K64 LPTMR provides an interrupt on match, it does not have
 * an interrupt on overflow.  As a consequence, we have to emulate the overflow
 * in softare.  To do this, we capture the last 16 bit counter value read.  At
 * the next read, if the count has decreased, we know that it has rolloed over,
 * and the high order 16 bit counter extension must be incremented.
 *
 * An implication of this is that the user code must call mu_rtc_now() at least
 * once every 64 seconds to detect the rollover. (Actually, once every 63.999
 * seconds to make sure it doesn't equal the previous call.)
 *
 * In addition, we extend the 16 bit counter in software to behave like a 32 bit
 * counter: whenever the low over 16 bits overflow, we increment the high-order
 * 16 bit word.  This gives us an overflow once every 4,194,304 seconds (48.5
 * days).
 */

 // =============================================================================
 // includes

#include "mu_rtc.h"
#include "mu_time.h"
#include <rtc.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

static mu_rtc_match_cb_t s_rtc_match_cb;
volatile static uint16_t s_rtc_hi;
static uint16_t s_match_count_hi;

// =============================================================================
// public code

/**
 * @brief Initialize the Real Time Clock.  Must be called before any other rtc
 * functions are called.
 */
void mu_rtc_init(void) {
  	// RTC.PER = 65535; /* Period Register: 65535 */

  	while (RTC.STATUS > 0) {
      /* Wait for register to synchronize */
  	}
  	// RTC.CNT = 0; /* 0 */

    // Enable RTC, running at the given RTC clock rate
    RTC.CTRL = RTC_PRESCALER_DIV1_gc;

    mu_rtc_set_match_cb(NULL);
    s_rtc_hi = 0;
}

mu_time_t mu_rtc_now(void) {
  uint16_t rtc_hi = s_rtc_hi;
  uint16_t rtc_lo = RTC.CNT;
  while (rtc_hi != s_rtc_hi) {
    rtc_hi = s_rtc_hi;
    rtc_lo = RTC.CNT;
  }
  return (uint32_t)rtc_hi << 16 | rtc_lo;
}

void mu_rtc_busy_wait(mu_duration_t ticks) {
  mu_time_t until  = mu_time_offset(mu_rtc_now(), ticks);
  while (mu_time_precedes(mu_rtc_now(), until)) {
    asm(" nop");
    // buzz...
  }
}

/**
 * @brief Set the time at which the RTC should trigger a callback.
 */
void mu_rtc_set_match_count(mu_time_t count) {
  RTC.INTFLAGS |= (1 << 1);   // clear COMPARE Interrupt flag
  s_match_count_hi = count >> 16;
  RTC.COMP = count & 0xFFFF;
}

/**
 * @brief Get the time at which the RTC should trigger a callback.
 */
mu_time_t mu_rtc_get_match_count(void) {
  return (uint32_t)s_match_count_hi << 16 | RTC.COMP;
}

/**
 * @brief Set the function to be called when the RTC count matches.
 *
 * Pass NULL for the CB to disable RTC compare callbacks.
 */
void mu_rtc_set_match_cb(mu_rtc_match_cb_t cb) {
  s_rtc_match_cb = cb;

  if (cb == NULL) {
    // disable compare interrupts, enable overflow interrupts.
    RTC.INTCTRL = RTC_COMPINTLVL_OFF_gc | RTC_OVFINTLVL_LO_gc;
  } else {
    // enable compare interrupts, enable overflow interrupts.
    RTC.INTCTRL = RTC_COMPINTLVL_LO_gc | RTC_OVFINTLVL_LO_gc;
  }
  while (RTC.STATUS > 0) {
    /* Wait for registers to synchronize before returning from the API */
  }
}

/**
 * @brief Called from interrupt level on compare interrupt.
 */
void mu_rtc_on_compare_interrupt(void) {
  if (s_match_count_hi == s_rtc_hi) {
    if (s_rtc_match_cb != NULL) {
      s_rtc_match_cb();
    }
  }
}

/**
 * @brief Called from interrupt level on overflow interrupt.
 */
void mu_rtc_on_overflow_interrupt(void) {
  s_rtc_hi += 1;
}

// =============================================================================
// local (static) code
