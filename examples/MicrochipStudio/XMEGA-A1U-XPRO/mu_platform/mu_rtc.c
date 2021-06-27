/**
 */

#include "mu_time.h"

static volatile mu_time_t s_rtc_ticks;
static mu_time_t s_safe_ticks;

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
  // Note that incrementing 32-bit s_rtc_ticks on an 8-bit system isn't atomic.
  // This approach means that we don't have to disable interrupts, but it does
  // assume that mu_time_now() is always called from foreground level.
  while (s_safe_ticks != s_rtc_ticks) {
    s_safe_ticks = s_rtc_ticks;
  }
  return s_safe_ticks;
}

/**
 * @brief Set the function to be called when the RTC ticks
 *
 * Pass NULL for the CB to disable RTC callbacks.
 */
void mu_rtc_set_callback(mu_rtc_callback_t cb) {
  s_rtc_cb = cb;
}

// =============================================================================
// local (static) code

/**
 * @brief Called from interrupt level RTC_FREQUENCY times per second.
 */
static void on_rtc_tick(void) {
  s_rtc_ticks += 1;
  if (s_rtc_cb) {
    s_rtc_cb();
  }
}
