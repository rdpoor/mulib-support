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
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip
 * Support</a>
 */

#include "atmel_start.h"
#include "atmel_start_pins.h"

#include "hal_io.h"
#include "hal_rtos.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define TASK_LED_STACK_SIZE (108 / sizeof(portSTACK_TYPE))
#define TASK_LED_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

#define TASK_MONITOR_STACK_SIZE (300 / sizeof(portSTACK_TYPE))
#define TASK_MONITOR_STACK_PRIORITY (tskIDLE_PRIORITY + 2)

#define TASK_CLI_STACK_SIZE (256 / sizeof(portSTACK_TYPE))
#define TASK_CLI_PRIORITY (tskIDLE_PRIORITY + 0)

static SemaphoreHandle_t disp_mutex;
static TaskHandle_t xCreatedLedTask;
static TaskHandle_t xCreatedConsoleTask;
static TaskHandle_t xCreatedMonitorTask;

/**
 * \brief Write string to console
 */
static void str_write(const char *s) {
  io_write(&EDBG_COM.io, (const uint8_t *)s, strlen(s));
}

/**
 * \brief Read character from console
 */
static char char_read(void) {
  uint8_t tmp;
  while (ERR_TIMEOUT == io_read(&EDBG_COM.io, &tmp, 1)) {
    ;
  }
  return (char)tmp;
}

static void disp_mutex_create(void) {
  disp_mutex = xSemaphoreCreateMutex();

  if (disp_mutex == NULL) {
    while (1) {
      ;
    }
  }
}

static bool disp_mutex_take(void) { return xSemaphoreTake(disp_mutex, ~0); }

static void disp_mutex_give(void) { xSemaphoreGive(disp_mutex); }

/**
 * \brief Dump statistics to console
 */
static void _os_show_statistics(void) {
  static portCHAR szList[128];
  sprintf(szList, "%c%s%c%s", 0x1B, "[1;1H", 0x1B, "[2J");
  str_write(szList);
  sprintf(szList,
          "--- Number of tasks: %u\r\n",
          (unsigned int)uxTaskGetNumberOfTasks());
  str_write(szList);
  str_write("> Tasks\tState\tPri\tStack\tNum\r\n");
  str_write("***********************************\r\n");
  vTaskList(szList);
  str_write(szList);

  str_write("--- Press a key:\r\n"
            " a: LED blink task active\r\n"
            " s: LED OFF and blink task suspend\r\n"
            " M: Monitor task suspend\r\n"
            " m: Monitor task resume\r\n"
            " others: Echo back\r\n"
            "#");
}

/**
 * OS task that blinks LED
 */
static void task_led(void *p) {
  (void)p;
  for (;;) {
    gpio_toggle_pin_level(LED0);
    os_sleep(500);
  }
}

/**
 * OS task that monitor system status and show statistics every 5 seconds
 */
static void task_monitor(void *p) {
  (void)p;
  for (;;) {
    if (disp_mutex_take()) {
      _os_show_statistics();
      disp_mutex_give();
    }

    os_sleep(5000);
  }
}

/**
 * \brief Pause LED blinking
 * \param[in] pause Set to \c true to pause, \c false to start
 */
static void task_led_pause(bool pause);

/**
 * \brief Pause monitor statistics dumping
 * \param[in] pause Set to \c true to pause, \c false to start
 */
static void task_monitor_pause(bool pause);

/**
 * OS task that monitor console inputs and do actions
 */
static void task_console(void *p) {
  (void)p;
  for (;;) {
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

    if (disp_mutex_take()) {
      str_write(str);
      disp_mutex_give();
    }
  }
}

/**
 * \brief Create OS task for LED blinking
 */
static void task_led_create(void) {
  /* Create task to make led blink */
  if (xTaskCreate(task_led,
                  "Led",
                  TASK_LED_STACK_SIZE,
                  NULL,
                  TASK_LED_STACK_PRIORITY,
                  &xCreatedLedTask) != pdPASS) {
    while (1) {
      ;
    }
  }
}

static void task_led_pause(bool pause) {
  if (pause) {
    vTaskSuspend(xCreatedLedTask);
  } else {
    vTaskResume(xCreatedLedTask);
  }
}

/**
 * \brief Create OS task for system status monitoring
 */
static void task_monitor_create(void) {
  /* Create task to monitor processor activity */
  if (xTaskCreate(task_monitor,
                  "Monitor",
                  TASK_MONITOR_STACK_SIZE,
                  NULL,
                  TASK_MONITOR_STACK_PRIORITY,
                  &xCreatedMonitorTask) != pdPASS) {
    while (1) {
      ;
    }
  }
}

static void task_monitor_pause(bool pause) {
  if (pause) {
    vTaskSuspend(xCreatedMonitorTask);
  } else {
    vTaskResume(xCreatedMonitorTask);
  }
}

/**
 * \brief Create OS task for console input/output
 */
static void task_console_create(void) {
  /* Create the task that handles the CLI. */
  if (xTaskCreate(task_console,
                  "Console",
                  TASK_CLI_STACK_SIZE,
                  NULL,
                  TASK_CLI_PRIORITY,
                  &xCreatedConsoleTask) != pdPASS) {
    while (1) {
      ;
    }
  }
}

static void tasks_run(void) { vTaskStartScheduler(); }

int main(void) {
  atmel_start_init();
#define TEST_TEMPLATE_EXAMPLE 0
#if TEST_TEMPLATE_EXAMPLE

  if (xTaskCreate(USART_0_example_task,
                  "Example",
                  TASK_CLI_STACK_SIZE,
                  NULL,
                  TASK_CLI_PRIORITY,
                  &xCreatedConsoleTask) != pdPASS) {
    while (1) {
      ;
    }
  }

#else
  disp_mutex_create();
  task_monitor_create();
  task_led_create();
  task_console_create();
#endif
  tasks_run();

  return 0;
}

#define _SPRINTF_OVERRIDE 1
#if _SPRINTF_OVERRIDE
/* Override sprintf implement to optimize */

static const unsigned m_val[] = {1000000000u,
                                 100000000u,
                                 10000000u,
                                 1000000u,
                                 100000u,
                                 10000u,
                                 1000u,
                                 100u,
                                 10u,
                                 1u};
int sprintu(char *s, unsigned u) {
  char tmp_buf[12];
  int i, n = 0;
  int m;

  if (u == 0) {
    *s = '0';
    return 1;
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      if (u >= m_val[i]) {
        u -= m_val[i];
      } else {
        break;
      }
    }
    tmp_buf[i] = m + '0';
  }
  for (i = 0; i < 10; i++) {
    if (tmp_buf[i] != '0') {
      break;
    }
  }
  for (; i < 10; i++) {
    *s++ = tmp_buf[i];
    n++;
  }
  return n;
}

int sprintf(char *s, const char *fmt, ...) {
  int n = 0;
  va_list ap;
  va_start(ap, fmt);
  while (*fmt) {
    if (*fmt != '%') {
      *s = *fmt;
      s++;
      fmt++;
      n++;
    } else {
      fmt++;
      switch (*fmt) {
      case 'c': {
        char valch = va_arg(ap, int);
        *s = valch;
        s++;
        fmt++;
        n++;
        break;
      }
      case 'd': {
        int vali = va_arg(ap, int);
        int nc;

        if (vali < 0) {
          *s++ = '-';
          n++;
          nc = sprintu(s, -vali);
        } else {
          nc = sprintu(s, vali);
        }

        s += nc;
        n += nc;
        fmt++;
        break;
      }
      case 'u': {
        unsigned valu = va_arg(ap, unsigned);
        int nc = sprintu(s, valu);
        n += nc;
        s += nc;
        fmt++;
        break;
      }
      case 's': {
        char *vals = va_arg(ap, char *);
        while (*vals) {
          *s = *vals;
          s++;
          vals++;
          n++;
        }
        fmt++;
        break;
      }
      default:
        *s = *fmt;
        s++;
        fmt++;
        n++;
      }
    }
  }
  va_end(ap);
  *s = 0;
  return n;
}
#endif /* _SPRINTF_OVERRIDE */
