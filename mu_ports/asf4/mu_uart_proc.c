/**
 * mu_uart_proc creates a process that manages asynchronous transmit and receive
 * operations with the Atmel ASF usart driver.
 */

#include "mu_uart_proc.h"
#include "hal_usart_async.h"
#include "mu_sched.h"
#include <stdbool.h>
#include <string.h>

// helpful:
// ASF4 API Reference Manual, Chapter 38
// http://ww1.microchip.com/downloads/en/DeviceDoc/50002633B.pdf

// =============================================================================
// local types and definitions

#ifndef MIN
#define MIN(a, b) (((a)<(b))?(a):(b))
#endif

// We need a reference to *proc available from interrupt level
static mu_uart_proc_t *s_proc;

// =============================================================================
// local storage

// =============================================================================
// local declarations

static void txc_cb(const struct usart_async_descriptor *const descr);
static void rxc_cb(const struct usart_async_descriptor *const descr);
static void err_cb(const struct usart_async_descriptor *const descr);

// =============================================================================
// public code

/**
 * Initialize a mu uart process with the given asynchronous device.
 *
 * NOTE: mu_uart_proc makes a copy of the callback functions and arguments.
 * This means that you cannot dynamically update the callback function or
 * argument (thought it's rare that you'd want to...)
 *
 * Design note: we could have the user pass in mu_task_t objects instead.
 */
mu_uart_proc_err_t mu_uart_proc_init(mu_uart_proc_t *proc,
                                     mu_sched_t *scheduler,
                                     struct usart_async_descriptor *uart,
                                     mu_task_t *tx_callback,
                                     mu_task_t *rx_callback) {
  s_proc = proc;  // save a static copy for use from interrupt level
  proc->scheduler = scheduler;
  proc->uart = uart;
  // The UART driver callbacks are called at interrupt level, but we need to
  // call the user's callbacks from foreground level.  We use the scheduler to
  // bridge the two: At interrupt level, we queue tx_notification with the
  // scheduler.  At foreground level, the next time the scheduler runs, it will
  // call tx_notification, which in turn will call the user's tx_callback.
  mu_task_init_immed(&(proc->tx_notification_evt),
                    tx_callback ? tx_callback->fn : NULL,
                    tx_callback ? tx_callback->self : NULL,
                    "TX_CALLBACK");
  mu_task_init_immed(&(proc->rx_notification_evt),
                    rx_callback ? rx_callback->fn : NULL,
                    rx_callback ? rx_callback->self : NULL,
                    "RX_CALLBACK");

  usart_async_register_callback(uart, USART_ASYNC_TXC_CB, txc_cb);
  usart_async_register_callback(uart, USART_ASYNC_RXC_CB, rxc_cb);
  usart_async_register_callback(uart, USART_ASYNC_ERROR_CB, err_cb);
  return mu_uart_proc_start(proc);
}

mu_sched_t *mu_uart_proc_scheduler(mu_uart_proc_t *proc) {
  return proc->scheduler;
}

struct usart_async_descriptor *mu_uart_proc_uart_descriptor(mu_uart_proc_t *proc) {
  return proc->uart;
}

mu_task_t *mu_uart_proc_tx_callback(mu_uart_proc_t *proc) {
  return mu_task_task(&(proc->tx_notification_evt));
}

mu_task_t *mu_uart_proc_rx_callback(mu_uart_proc_t *proc) {
  return  mu_task_task(&(proc->rx_notification_evt));
}

mu_uart_proc_err_t mu_uart_proc_start(mu_uart_proc_t *proc) {
  proc->is_tx_pending = false;
  usart_async_get_io_descriptor(proc->uart, &(proc->uart_io));
  usart_async_flush_rx_buffer(proc->uart);
  usart_async_enable(proc->uart);
  return MU_UART_PROC_ERR_NONE;
}

mu_uart_proc_err_t mu_uart_proc_stop(mu_uart_proc_t *proc) {
  usart_async_disable(proc->uart);
  proc->is_tx_pending = false;
  return MU_UART_PROC_ERR_NONE;
}

/**
 * Start transmitting data to the uart.  Returns MU_UART_ERR_BUSY if the uart
 * hasn't completed a previous tx operation.
 *
 * When all nchars have been transmitted and the uart tx buffer is empty,
 * the uart process will call the tx_callback if it is non-null.  Otherwise,
 * you can call mu_uart_proc_tx_is_busy() to find if the previous operation
 * has completed.
 */
mu_uart_proc_err_t mu_uart_proc_tx(mu_uart_proc_t *proc, const char *s, int nchars) {
  if (proc->is_tx_pending) {
    return MU_UART_PROC_ERR_BUSY;
  }
  // start the write process
  proc->is_tx_pending = true;
  io_write(proc->uart_io, (const uint8_t *)s, strlen(s));
  return MU_UART_PROC_ERR_NONE;
}

/**
 * Return true if the tx process is still sending data.
 */
bool mu_uart_proc_tx_is_busy(mu_uart_proc_t *proc) {
  // NOTE: even though all the data may have been transferred to the UART,
  // there may still be data buffered by the uart itself.  This is why we
  // also check usart_async_is_tx_empty().
  return proc->is_tx_pending || !usart_async_is_tx_empty(proc->uart);
}

/**
 * Read up to nchars of received data into s.  Returns (by reference) the number
 * of characters read.
 *
 * When one or more characters become available, if rx_callback is set, it will
 * be called.  Otherwise you can call mu_uart_proc_rx_is_available() to find if
 * there are any characters available for reading.
 */
mu_uart_proc_err_t mu_uart_proc_rx(mu_uart_proc_t *proc, const char *s, int *nchars) {
  int desired = *nchars;
  int available = ringbuffer_num(&(proc->uart->rx));
  int n = MIN(desired, available);

  if (ERR_NONE != io_read(proc->uart_io, (uint8_t *)s, n)) {
    return MU_UART_PROC_ERR_READ;
  } else {
    *nchars = n;
    return MU_UART_PROC_ERR_NONE;
  }
}

/**
 * Return true if there are any characters available for reading.
 */
bool mu_uart_proc_rx_is_available(mu_uart_proc_t *proc) {
  return ringbuffer_num(&(proc->uart->rx)) != 0;
}

// =============================================================================
// local code

static void txc_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  s_proc->is_tx_pending = false;
  if (mu_uart_proc_tx_callback(s_proc)->fn) {
    // schedule an immediate event (from interrupt level) that will call
    // the tx_callback (from forground level).
    mu_sched_queue_from_isr(s_proc->scheduler, &(s_proc->tx_notification_evt));
  }
}

static void rxc_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  if (mu_uart_proc_rx_callback(s_proc)->fn) {
    // schedule an immediate event (from interrupt level) that will call
    // the rx_callback (from forground level).
    mu_sched_queue_from_isr(s_proc->scheduler, &(s_proc->rx_notification_evt));
  }
}

static void err_cb(const struct usart_async_descriptor *const descr) {
  (void)descr;
  asm("nop");
}
