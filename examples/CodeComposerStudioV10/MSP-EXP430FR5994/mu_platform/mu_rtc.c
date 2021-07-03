/**
 */

 // =============================================================================
 // includes

#include "mu_rtc.h"
#include "mu_time.h"
#include "time.h"          // posix time functions
#include <msp430.h>

// =============================================================================
// local types and definitions

#define NANOSECS_PER_S  (1000000000)
#define NANOSECS_PER_MS (1000000)

// =============================================================================
// local (forward) declarations
static uint16_t get_timer_l(void);
static void init_timer(void);
static void on_timer_tick(void);

// =============================================================================
// local storage

static volatile mu_time_t s_rtc_ticks;
//static mu_time_t s_safe_ticks;
static volatile uint16_t s_timer_h;  // high order 16 bits
static uint16_t s_safe_timer_h;      // high order 16 bits, sanitized

static mu_rtc_callback_t s_rtc_cb;

// =============================================================================
// public code

/**
 * @brief Initialize the Real Time Clock.  Must be called before any other rtc
 * functions are called.
 */
void mu_rtc_init(void) {
  s_rtc_ticks = 0;
    init_timer();
  s_timer_h = 0;
  __enable_interrupt();
}

/**
 * @brief Get the current time.
 */
mu_time_t mu_rtc_now(void) {
  uint16_t timer_l;

  do {
    // snapshot high order and low order bits of timer
    s_safe_timer_h = s_timer_h;
    timer_l = get_timer_l();
    // values are safe if high order bits haven't changed
  } while (s_safe_timer_h != s_timer_h);

  // assemble high and low 16 bits into a 32 bit value.
  return ((mu_time_t)s_safe_timer_h << 16) | (timer_l);
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

/**
 *  About the timer:
 *
 *  Goal:
 *
 *  User Timer T0_A in continuous count up mode, driven from the external 32KHz
 *  XTAL.  As a 16 bit counter, it will interrupt on overflow every 2 seconds
 *  (i.e. 2^16/32768).
 *
 *  We implement an "extended 32 bit counter" by incrementing a uint16_t value
 *  at every overflow.  The tick resolution is then 30.518 uSec, the maximum
 *  count is 131,072 seconds.
 */
static void init_timer(void) {
    TA0CTL = TASSEL__ACLK |      // 32768 Hz source
             MC__CONTINUOUS |    // continuous mode
             TAIE |              // Interrupt on overflow
             0; // TACLR;
}

static uint16_t get_timer_l(void) {
    return TA0R;
}

static void on_timer_tick(void) {
  s_timer_h += 1;
}

// Timer B0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(TIMER0_A1_VECTOR))) Timer_A0(void)
#else
#error Compiler not supported!
#endif
{
  on_timer_tick();
  TA0CTL &= ~TAIFG;                     // clear interrupt flag
  __bic_SR_register_on_exit(LPM3_bits); // Exit LPM3
}
