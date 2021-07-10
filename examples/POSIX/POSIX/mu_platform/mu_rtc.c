/**
 */

 // =============================================================================
 // includes

#include "mu_rtc.h"
#include "mu_time.h"
#include "time.h"          // posix time functions

// =============================================================================
// local types and definitions

#define NANOSECS_PER_S  (1000000000)
#define NANOSECS_PER_MS (1000000)

// =============================================================================
// local (forward) declarations

// =============================================================================
// local storage

static volatile mu_time_t s_rtc_ticks;
//static mu_time_t s_safe_ticks;
static mu_rtc_alarm_cb_t s_rtc_alarm_cb;
static mu_time_t s_rtc_alarm_time; 

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

void mu_rtc_busy_wait(mu_duration_t duration) {
  mu_time_t until  = mu_time_offset(mu_rtc_now(), duration);
  while (mu_time_precedes(mu_rtc_now(), until)) {
    asm(" nop");
    // buzz...
  }
}

/**
 * @brief Set the time at which the RTC should trigger a callback.
 */
void mu_rtc_set_alarm(mu_time_t count) {
 // RTC.INTFLAGS |= (1 << 1);   // clear COMPARE Interrupt flag
 // s_match_count_hi = count >> 16;
 // RTC.COMP = count & 0xFFFF;



 /*
    iRet = getitimer( ITIMER_REAL, &itimer );
    if ( iRet )
    {
        prvFatalError( "getitimer", errno );
    }

    // Set the interval between timer events. 
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = portTICK_RATE_MICROSECONDS;

    // Set the current count-down. 
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = portTICK_RATE_MICROSECONDS;

    // Set-up the timer interrupt. 
    iRet = setitimer( ITIMER_REAL, &itimer, NULL );
    if ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

*/
}

/**
 * @brief Get the time at which the RTC should trigger a callback.
 */
mu_time_t mu_rtc_get_alarm(void) {
  //return (uint32_t)s_match_count_hi << 16 | RTC.COMP;
  return s_rtc_alarm_time;
}

/**
 * @brief Set the function to be called when the RTC count matches.
 *
 * Pass NULL for the CB to disable RTC compare callbacks.
 */
void mu_rtc_set_alarm_cb(mu_rtc_alarm_cb_t cb) {
  s_rtc_alarm_cb = cb;

  // if (cb == NULL) {
  //   // disable compare interrupts, enable overflow interrupts.
  //   RTC.INTCTRL = RTC_COMPINTLVL_OFF_gc | RTC_OVFINTLVL_LO_gc;
  // } else {
  //   // enable compare interrupts, enable overflow interrupts.
  //   RTC.INTCTRL = RTC_COMPINTLVL_LO_gc | RTC_OVFINTLVL_LO_gc;
  // }
  // while (RTC.STATUS > 0) {
  //   /* Wait for registers to synchronize before returning from the API */
  // }
}


// =============================================================================
// local (static) code
