/**
 */

 // =============================================================================
 // includes

#include "mu_rtc.h"
#include "mu_time.h"
#include <stdio.h>
#include <stdlib.h>
//#include "time.h"          // posix time functions
#include <time.h> // posix time functions
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

// =============================================================================
// local types and definitions

#define NANOSECS_PER_S  (1000000000)
#define NANOSECS_PER_MS (1000000)

// =============================================================================
// local (forward) declarations

static void start_alarm_thread();
static void *alarm_thread(void* vargp);
// =============================================================================
// local storage

static volatile mu_time_t s_rtc_ticks;
//static mu_time_t s_safe_ticks;
static mu_rtc_alarm_cb_t s_rtc_alarm_cb;
static mu_time_t s_rtc_alarm_time; 
static pthread_t alarm_thread_id = NULL;

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
 * @brief Set the absolute time at which the RTC should trigger a callback.
 */

void mu_rtc_set_alarm(mu_time_t alarm_time) {
  s_rtc_alarm_time = alarm_time;
  start_alarm_thread(alarm_time);
}

/**
 * @brief Get the time at which the RTC should trigger a callback.
 */
mu_time_t mu_rtc_get_alarm(void) {
  //return (uint32_t)s_match_count_hi << 16 | RTC.COMP;
  return s_rtc_alarm_time;
}

/**
 * @brief Set the function to be called when the RTC alarm fires
 *
 * Pass NULL for the CB to disable RTC compare callbacks.
 */

void mu_rtc_set_alarm_cb(mu_rtc_alarm_cb_t cb) {
  s_rtc_alarm_cb = cb;
}


// =============================================================================
// local (static) code

static void start_alarm_thread(mu_time_t alarm_time) {
  int r;
  void *res;
  if(alarm_thread_id) {
    // there is already an alarm
   r = pthread_cancel(alarm_thread_id);
    if (r != 0) {
      fprintf(stderr, "start_alarm_thread pthread_cancel error: %d\n",r);
      exit(-1); 
    }
    // Join with thread to check its exit status
   r = pthread_join(alarm_thread_id, &res);
   if (r != 0)
    fprintf(stderr, "start_alarm_thread pthread_join error: %d\n",r);

   if (res != PTHREAD_CANCELED) {
      fprintf(stderr, "start_alarm_thread pthread_cancel join error.\n");
      exit(-1); 
    }
  }
  pthread_create(&alarm_thread_id, NULL, alarm_thread, &alarm_time);
}

static void *alarm_thread(void* vargp)
{
  mu_time_t *alarm_time = (mu_time_t *)vargp;

  struct timespec alarm_spec;
  alarm_spec.tv_sec = *alarm_time / NANOSECS_PER_S;
  alarm_spec.tv_nsec = *alarm_time % NANOSECS_PER_S;

  int s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  if (s != 0) {
    fprintf(stderr, "pthread_setcancelstate error: %d\n",s);
     exit(-1); 
  }
  // nanosleep is unreliable for long sleeps (there is the possibility of rollover for anything longer than about 24 days (2^32 ms))
  // TODO: build a replacement routine along these lines: http://ab-initio.mit.edu/octave-Faddeeva/gnulib/lib/nanosleep.c
  nanosleep(&alarm_spec, NULL);

  if(s_rtc_alarm_cb)
    s_rtc_alarm_cb();

  alarm_thread_id = NULL;
  return NULL;
}
