#ifndef MU_UART_PROC_H_
#define MU_UART_PROC_H_

#include "hal_usart_async.h"
#include "mu_sched.h"
#include <stdbool.h>

typedef struct {
  mu_sched_t *scheduler;
  struct usart_async_descriptor *uart;

  struct io_descriptor *uart_io;
  struct usart_async_status uart_status;
  mu_evt_t tx_notification_evt;
  mu_evt_t rx_notification_evt;
  volatile bool is_tx_pending;
} mu_uart_proc_t;

typedef enum {
  MU_UART_PROC_ERR_NONE,
  MU_UART_PROC_ERR_BUSY,
  MU_UART_PROC_ERR_READ
} mu_uart_proc_err_t;

/**
 * mu_uart_proc creates a process that manages asynchronous transmit and receive
 * operations with the Atmel ASF usart driver.
 */

/**
 * Initialize a mu uart process with the given asynchronous device.
 */
mu_uart_proc_err_t mu_uart_proc_init(mu_uart_proc_t *proc,
                                     mu_sched_t *scheduler,
                                     struct usart_async_descriptor *uart,
                                     mu_task_t *tx_callback,
                                     mu_task_t *rx_callback);

mu_sched_t *mu_uart_proc_scheduler(mu_uart_proc_t *proc);

struct usart_async_descriptor *mu_uart_proc_uart_descriptor(mu_uart_proc_t *proc);

mu_task_t *mu_uart_proc_tx_callback(mu_uart_proc_t *proc);

mu_task_t *mu_uart_proc_rx_callback(mu_uart_proc_t *proc);

/**
 * enable / disable mu_uart process
 */
mu_uart_proc_err_t mu_uart_proc_start(mu_uart_proc_t *proc);
mu_uart_proc_err_t mu_uart_proc_stop(mu_uart_proc_t *proc);

/**
 * Start transmitting data to the uart.  Returns MU_UART_ERR_BUSY if the uart
 * hasn't completed a previous tx operation.
 *
 * When all nchars have been transmitted and the uart tx buffer is empty,
 * the uart process will call the tx_callback if it is non-null.  Otherwise,
 * you can call mu_uart_proc_tx_is_busy() to find if the previous operation
 * has completed.
 */
mu_uart_proc_err_t mu_uart_proc_tx(mu_uart_proc_t *proc, const char *s, int nchars);

/**
 * Return true if the tx process is still sending data.
 */
bool mu_uart_proc_tx_is_busy(mu_uart_proc_t *proc);

/**
 * Read up to nchars of received data into s.  Returns (by reference) the number
 * of characters read.
 *
 * When one or more characters become available, if rx_callback is set, it will
 * be called.  Otherwise you can call mu_uart_proc_rx_is_available() to find if
 * there are any characters available for reading.
 */
mu_uart_proc_err_t mu_uart_proc_rx(mu_uart_proc_t *proc, const char *s, int *nchars);

/**
 * Return true if there are any characters available for reading.
 */
bool mu_uart_proc_rx_is_available(mu_uart_proc_t *proc);

#endif /* #ifndef MU_UART_PROC_H_ */
