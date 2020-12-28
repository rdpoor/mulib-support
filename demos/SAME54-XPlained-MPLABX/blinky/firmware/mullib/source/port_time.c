/**
 * @file prt_time.c
 *
 */

 //DOM-IGNORE-BEGIN
 /******************************************************************************
 MIT License

 Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END

// =============================================================================
// include files

#include "port_time.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// =============================================================================
// types and definitions

#define MAX_DURATION ((uint32_t)0x7fffffff)

// =============================================================================
// forward declarations to local functions

// =============================================================================
// extra-libary declarations (must be resolved in user's code)

extern void RTC_Initialize(void);
extern void RTC_Timer32Start ( void );
extern void RTC_Timer32Stop ( void );
extern void RTC_Timer32CounterSet ( uint32_t count );
extern uint32_t RTC_Timer32CounterGet ( void );
extern uint32_t RTC_Timer32FrequencyGet ( void );
extern void RTC_Timer32Compare0Set ( uint32_t compareValue );
extern void RTC_Timer32Compare1Set ( uint32_t compareValue );
extern uint32_t RTC_Timer32PeriodGet ( void );
// extern void RTC_Timer32InterruptEnable( RTC_TIMER32_INT_MASK interrupt );
// extern void RTC_Timer32InterruptDisable( RTC_TIMER32_INT_MASK interrupt );
// extern void RTC_BackupRegisterSet( BACKUP_REGISTER reg, uint32_t value );
// extern uint32_t RTC_BackupRegisterGet( BACKUP_REGISTER reg );
// extern TAMPER_CHANNEL RTC_TamperSourceGet( void );
extern uint32_t RTC_Timer32TimeStampGet( void );
// extern void RTC_Timer32CallbackRegister ( RTC_TIMER32_CALLBACK callback, uintptr_t context );
extern void RTC_Timer32CallbackRegister ();

// =============================================================================
// local (static) storage

static port_time_seconds_dt s_rtc_period;

// =============================================================================
// main code starts here

void port_time_init() {
  RTC_Initialize();
  s_rtc_period = 1.0/(port_time_seconds_dt)RTC_Timer32FrequencyGet();
  RTC_Timer32Start();        // start counting
}

port_time_t port_time_offset(port_time_t t, port_time_dt dt) {
  return t + dt;
}

port_time_dt port_time_difference(port_time_t t1, port_time_t t2) {
  return t1 - t2;
}

bool port_time_is_before(port_time_t t1, port_time_t t2) {
  return port_time_difference(t1, t2) > MAX_DURATION;
}

bool port_time_is_equal(port_time_t t1, port_time_t t2) {
  return t1 == t2;
}

bool port_time_is_after(port_time_t t1, port_time_t t2) {
  return port_time_difference(t2, t1) > MAX_DURATION;
}

port_time_dt port_time_ms_to_duration(port_time_ms_dt ms) {
    return port_time_seconds_to_duration(ms / 1000.0); // could be better
}

port_time_ms_dt port_time_duration_to_ms(port_time_dt dt) {
    return port_time_duration_to_seconds(dt) * 1000;   // could be better
}

port_time_dt port_time_seconds_to_duration(port_time_seconds_dt secs) {
  return secs / s_rtc_period;
}

port_time_seconds_dt port_time_duration_to_seconds(port_time_dt dt) {
  return dt * s_rtc_period;
}

port_time_t port_time_now() {
  return RTC_Timer32CounterGet();
}

// =============================================================================
// private code
