/**
 */

#include "mu_time.h"

static volatile mu_time_t s_rtc_ticks;
static mu_time_t s_safe_ticks;

/**
 * @brief Initialize the time system.  Must be called before any other time
 * functions are called.
 */
void mu_time_init(void) {
  s_rtc_ticks = 0;
}

/**
 * @brief Get the current system time.
 *
 * @return A value representing the current time.
 */
mu_time_t mu_time_now() {
  // Note that incrementing 32-bit s_rtc_ticks on an 8-bit system isn't atomic.
  // This approach means that we don't have to disable interrupts, but it does
  // assume that mu_time_now() is always called from foreground level.
  while (s_safe_ticks != s_rtc_ticks) {
    s_safe_ticks = s_rtc_ticks;
  }
  return s_safe_ticks;
}

/**
 * @brief Called from interrupt level RTC_FREQUENCY times per second.
 */
void mu_time_on_rtc_tick(void) {
  s_rtc_ticks += 1;
}

/**
 * @brief Add a time and a duration.
 *
 * `mu_time_offset` adds a time and a duration to produce a new time object.
 *
 * @param t1 a time object
 * @param dt a duration object
 * @return t1 offset by dt
 */
mu_time_t mu_time_offset(mu_time_t t1, mu_duration_t dt) {
  return t1 + dt;
}

/**
 * @brief Take the difference between two time objects
 *
 * `mu_time_difference` subtracts t2 from t1 to produce a duration object.
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return (t1-t2) as a duration object
 */
mu_duration_t mu_time_difference(mu_time_t t1, mu_time_t t2) {
  return t1 - t2;
}

/**
 * @brief Return true if t1 is strictly before t2
 *
 * Note that if you want to know if t1 is before or equal to t2, you can use the
 * construct `!mu_time_follows(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly before t2, false otherwise.
 */
bool mu_time_precedes(mu_time_t t1, mu_time_t t2) {
  return t1 < t2;
}

/**
 * @brief Return true if t1 is equal to t2
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 equals t2, false otherwise.
 */
bool mu_time_equals(mu_time_t t1, mu_time_t t2) {
  return t1 == t2;
}

/**
 * @brief Return true if t1 is strictly after t2
 *
 * Note that if you want to know if t1 is equal to or after t2, you can use the
 * construct `!mu_time_precedes(t2, t1)``
 *
 * @param t1 A time object
 * @param t2 A time object
 * @return true if t1 is strictly after t2, false otherwise.
 */
bool mu_time_follows(mu_time_t t1, mu_time_t t2) {
  return t1 > t2;
}

/**
 * @brief Convert a duration to milliseconds.
 *
 * @param dt A duration object
 * @return The duration in seconds
 */
mu_duration_ms_t mu_time_duration_to_ms(mu_duration_t dt) {
  return (dt * 1000) / RTC_FREQUENCY;
}

/**
 * @brief Convert milliseconds to a duration
 *
 * @param ms The duration in milliseconds
 * @return A duration object
 */
mu_duration_t mu_time_ms_to_duration(mu_duration_ms_t ms) {
  return (ms * RTC_FREQUENCY) / 1000;
}

#ifdef MU_FLOAT
/**
 * @brief Convert a duration to seconds.
 *
 * @param dt A duration object
 * @return The duration in seconds
 */
MU_FLOAT mu_time_duration_to_s(mu_duration_t dt) {
  #error "Provide a platform-specific implementation for mu_time_duration_to_s()"
}

/**
 * @brief Convert seconds to a duration.
 *
 * @param s The duration in seconds
 * @return A duration object
 */
mu_duration_t mu_time_s_to_duration(MU_FLOAT s) {
  #error "Provide a platform-specific implementation for mu_time_s_to_duration()"
}

#endif // #ifdef MU_FLOAT
