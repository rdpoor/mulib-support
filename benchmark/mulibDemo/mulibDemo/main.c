/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

// =============================================================================
// includes

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "driver_init.h"
#include "utils_ringbuffer.h"
#include "mu_evt.h"
#include "mu_sched.h"
#include "mu_task.h"
#include "mu_time.h"
#include "port.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>


// =============================================================================
// types and definitions

// # of interrupts notifications we can handle between calls to sched_step()
// must be a power of two.
#define ISR_QUEUE_POOL_SIZE 8

// =============================================================================
// forward declarations

/**
 * \brief Write string to console
 */
static void str_write(const char *s);

/**
 * \brief Read character from console
 */
static char char_read(void);

/**
 * \brief Return true if io_read() will not block.
 */
static bool char_is_available(void);

/**
 * \brief Dump statistics to console
 */
static void _os_show_statistics(void);

/**
 * \brief Print event state
 */
static void print_event(mu_evt_t *evt, char *buf, int maxlen);

/**
 * \brief mulib task that blinks LED
 */
static void task_led(void *self, void *arg);

/**
 * \brief Pause LED blinking
 * \param[in] pause Set to \c true to pause, \c false to start
 */
static void task_led_pause(bool pause);

/**
 *  \brief mulib task that monitor system status and show stats every 5 seconds
 */
static void task_monitor(void *self, void *arg);

/**
 * \brief Pause monitor statistics dumping
 * \param[in] pause Set to \c true to pause, \c false to start
 */
static void task_monitor_pause(bool pause);

/**
 * \brief mulib task to monitor console inputs and do actions
 */
static void task_console(void *self, void *arg);

/**
 * \brief interrupt-level callback when serial data is received.
 */
static void serial_rx_cb(const struct usart_async_descriptor *const descr);

/**
 * \brief interrupt-level callback when serial data is transmitted.
 */
static void serial_tx_cb(const struct usart_async_descriptor *const descr);

/**
 * \brief interrupt-level callback when error detected on serial port.
 */
static void serial_err_cb(const struct usart_async_descriptor *const descr);

// =============================================================================
// local storage

static struct io_descriptor *s_usart_io;

static mu_sched_t s_sched;
static mu_ring_obj_t s_isr_queue_pool[ISR_QUEUE_POOL_SIZE];
static mu_evt_t s_led_event;
static mu_evt_t s_monitor_event;
static mu_evt_t s_console_event;

// =============================================================================
// public code

int main(void) {
  atmel_start_init();
  port_init();          // set up rtc, etc.

  // Initialize the scheduler
  mu_sched_init(&s_sched, s_isr_queue_pool, ISR_QUEUE_POOL_SIZE);

  // Queue initial tasks
  task_led_pause(false);
  task_monitor_pause(false);
  mu_sched_add(&s_sched,
               mu_evt_init_immed(&s_console_event, task_console, NULL, "Console"));

  usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, serial_tx_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, serial_rx_cb);
  usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, serial_err_cb);
  usart_async_get_io_descriptor(&EDBG_COM, &s_usart_io);
  usart_async_enable(&EDBG_COM);

  while(1) {
    mu_sched_step(&s_sched);
    _os_show_statistics();
  }

  // Like Charlie on the MTA...
  return 0;
}

// =============================================================================
// private code

/**
 * \brief Write string to console
 */
static void str_write(const char *s) {
  io_write(s_usart_io, (const uint8_t *)s, strlen(s));
  delay_ms(500);
}

/**
 * \brief Read character from console
 */
static char char_read(void) {
  uint8_t tmp;
  while (ERR_TIMEOUT == io_read(&EDBG_COM.io, &tmp, 1)) {
    // we always call char_is_available() before calling char_read() so we
    // should never arrive here.
    ;
  }
  return (char)tmp;
}

/**
 * \brief Return true if io_read() will not block.
 */
static bool char_is_available(void) {
  return ringbuffer_num(&EDBG_COM.rx) != 0;
}

/**
 * \brief Dump statistics to console
 */
static void _os_show_statistics(void) {
  mu_evt_t *evt;
  static char szList[128];
  sprintf(szList, "%c%s%c%s", 0x1B, "[1;1H", 0x1B, "[2J");
  str_write(szList);
  sprintf(szList,
          "\r\n--- Number of tasks: %u",
          mu_sched_task_count(&s_sched) + 1);   // + 1 to account for current event
  str_write("\r\n> Tasks\tState\tPri\tStack\tNum");
  str_write("\r\n***********************************");
  if ((evt = mu_sched_current_event(&s_sched)) != NULL) {
    // print current event (if there is one...)
    str_write("\r\n");
    print_event(evt, szList, 128);
    str_write(szList);
  }
  evt = mu_sched_get_events(&s_sched);
  while(evt != NULL) {
    str_write("\r\n");
	  print_event(evt, szList, 128);
	  str_write(szList);
	  evt = evt->next;
  }

  str_write("\r\n--- Press a key:"
            "\r\n a: LED blink task active"
            "\r\n s: LED OFF and blink task suspend"
            "\r\n M: Monitor task suspend"
            "\r\n m: Monitor task resume"
            "\r\n others: Echo back"
            "\r\n# ");
}

static void print_event(mu_evt_t *evt, char *buf, int maxlen) {
  // todo: immediate vs no, sched time, is runnable
  // todo: is in schedule, call count, runtime
  mu_task_t *task = &evt->task;
  snprintf(buf, maxlen, "%10s: %d %f",
           mu_task_name(task),
           mu_task_call_count(task),
           mu_task_runtime(task));
}

static void task_led(void *self, void *arg) {
  (void)self;
  (void)arg;
  gpio_toggle_pin_level(LED0);
  // run again in 500 mSec
  s_led_event.time = mu_time_offset(s_led_event.time,
                                    mu_time_seconds_to_duration(0.5));
  mu_sched_add(&s_sched, &s_led_event);
}

static void task_led_pause(bool pause) {
  if (pause) {
    // remove LED event from schedule (if it's in the schedule)
    mu_sched_remove(&s_sched, &s_led_event);
  } else if (!mu_sched_has_event(&s_sched, &s_led_event)) {
    // LED task is not scheduled -- start it now.
    mu_time_t now = mu_time_now();
    mu_sched_add(&s_sched,
                 mu_evt_init_at(&s_led_event, now, task_led, NULL, "LED"));
  }
}

static void task_monitor(void *self, void *arg) {
  (void)self;
  (void)arg;
  _os_show_statistics();
  // run again in 5 sec
  s_monitor_event.time = mu_time_offset(s_monitor_event.time,
                                        mu_time_seconds_to_duration(5.0));
  mu_sched_add(&s_sched, &s_monitor_event);
}

static void task_monitor_pause(bool pause) {
  if (pause) {
    // remove Monitor event from schedule (if it's in the schedule)
    mu_sched_remove(&s_sched, &s_monitor_event);
  } else if (!mu_sched_has_event(&s_sched, &s_monitor_event)) {
    // Monitor task is not scheduled -- start it now.
    mu_time_t now = mu_time_now();
    mu_sched_add(&s_sched,
                 mu_evt_init_at(&s_monitor_event, now, task_monitor, NULL, "Monitor"));
  }
}

/**
 * mulib task to monitor console inputs and do actions
 */
static void task_console(void *self, void *arg) {
  (void)self;
  (void)arg;

  while(char_is_available()) {
    char *str = "";
    char ch[2] = {0, 0};
    ch[0] = char_read();
    switch (ch[0]) {
    case 'a':
      task_led_pause(false);
      str = ("- LED blink task active.\r\n");
      break;
    case 's':
      task_led_pause(true);
      str = ("- LED OFF and blink task suspended.\r\n");
      gpio_set_pin_level(LED0, true);
      break;
    case 'm':
      task_monitor_pause(false);
      str = ("- Monitor task active.\r\n");
      break;
    case 'M':
      task_monitor_pause(true);
      str_write("- Monitor task suspended.\r\n");
      break;
    default:
      str = ch;
      break;
    }

    str_write(str);
  }
}

static void serial_rx_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  // a character was received.  start the console task.
  mu_sched_from_isr(&s_sched, &s_console_event);
}

static void serial_tx_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  asm("nop");
}

static void serial_err_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  asm("nop");
}
