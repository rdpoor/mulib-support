/**
 */

 // =============================================================================
 // includes

#include "mu_rtc.h"
#include "mu_time.h"
#include "time.h"          // posix time functions

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

static volatile mu_time_t s_rtc_ticks;
//static mu_time_t s_safe_ticks;
static mu_rtc_callback_t s_rtc_cb;

// =============================================================================
// public code

/**
 * @brief Initialize the Real Time Clock.  Must be called before any other rtc
 * functions are called.
 */
void mu_rtc_init(void) {
  s_rtc_ticks = 0;
}

/**
 * @brief Get the current time.
 */
mu_time_t mu_rtc_now(void) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  return (now.tv_sec * NANOSECS_PER_S + now.tv_nsec) / NANOSECS_PER_MS;
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
