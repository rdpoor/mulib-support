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

static void button_on_PB07_pressed(void)
{
}

static void button_on_PB31_pressed(void)
{
}

/**
 * Example of using WINC_IRQ
 */
void WINC_IRQ_example(void)
{

	ext_irq_register(PIN_PB07, button_on_PB07_pressed);
	ext_irq_register(PIN_PB31, button_on_PB31_pressed);
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

/**
 * Example of using EDBG_UART to write "Hello World" using the IO abstraction.
 */
void EDBG_UART_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&EDBG_UART, &io);
	usart_sync_enable(&EDBG_UART);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using WINC_SPI to write "Hello World" using the IO abstraction.
 */
static uint8_t example_WINC_SPI[12] = "Hello World!";

void WINC_SPI_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&WINC_SPI, &io);

	spi_m_sync_enable(&WINC_SPI);
	io_write(io, example_WINC_SPI, 12);
}

void delay_example(void)
{
	delay_ms(5000);
}
