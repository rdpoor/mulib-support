/**
 */

 // =============================================================================
 // includes

#include "mu_rtc.h"
#include "mu_time.h"
#include "time.h"          // posix time functions
#include "stm32g4xx_hal.h"

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations


// =============================================================================
// local storage

static volatile mu_time_t s_rtc_ticks;
// //static mu_time_t s_safe_ticks;
// static volatile uint16_t s_timer_h;  // high order 16 bits
// static uint16_t s_safe_timer_h;      // high order 16 bits, sanitized

static mu_rtc_callback_t s_rtc_cb;

// =============================================================================
// public code

/**
 * @brief Initialize the Real Time Clock.  Must be called before any other rtc
 * functions are called.
 */
void mu_rtc_init(void) {
    // no initialization required

  // s_rtc_ticks = 0;
  //   init_timer();
  // s_timer_h = 0;
  // __enable_interrupt();
}

/**
 * @brief Get the current time.
 */
mu_time_t mu_rtc_now(void) {
  return HAL_GetTick();
}


void mu_rtc_busy_wait(mu_time_t ticks) {
  mu_time_t until  = mu_time_offset(mu_rtc_now(), ticks);
  while (mu_time_precedes(mu_rtc_now(), until)) {
    asm(" nop");
    // buzz...
  }
}
/**
 * @brief Set the function to be called when the RTC ticks
 *
 * Pass NULL for the CB to disable RTC callbacks.
 */
void mu_rtc_set_callback(mu_rtc_callback_t cb) {
  s_rtc_cb = cb;
}

/**
 * @brief Called from interrupt level RTC_FREQUENCY times per second.
 */
void mu_rtc_on_rtc_tick(void) {
  s_rtc_ticks += 1;
  if (s_rtc_cb) {
    s_rtc_cb();
  }
}

// =============================================================================
// local (static) code
