/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using EDBG_COM to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_EDBG_COM[12] = "Hello World!";

static void tx_cb_EDBG_COM(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void EDBG_COM_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&EDBG_COM, USART_ASYNC_TXC_CB, tx_cb_EDBG_COM);
	/*usart_async_register_callback(&EDBG_COM, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&EDBG_COM, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&EDBG_COM, &io);
	usart_async_enable(&EDBG_COM);

	io_write(io, example_EDBG_COM, 12);
}

/**
 * Example of using CALENDAR_0.
 */
static struct calendar_alarm alarm;

static void alarm_cb(struct calendar_descriptor *const descr)
{
	/* alarm expired */
}

void CALENDAR_0_example(void)
{
	struct calendar_date date;
	struct calendar_time time;

	calendar_enable(&CALENDAR_0);

	date.year  = 2000;
	date.month = 12;
	date.day   = 31;

	time.hour = 12;
	time.min  = 59;
	time.sec  = 59;

	calendar_set_date(&CALENDAR_0, &date);
	calendar_set_time(&CALENDAR_0, &time);

	alarm.cal_alarm.datetime.time.sec = 4;
	alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm.cal_alarm.mode              = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm, alarm_cb);
}
