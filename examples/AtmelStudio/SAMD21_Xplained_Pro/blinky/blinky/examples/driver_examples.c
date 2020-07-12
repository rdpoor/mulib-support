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

static void button_on_PA15_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PA15, button_on_PA15_pressed);
}

/*
 * \Write data to usart interface
 *
 * \param[in] buf Data to write to usart
 * \param[in] length The number of bytes to write
 *
 * \return The number of bytes written.
 */
static uint32_t USART_0_write(const uint8_t *const buf, const uint16_t length)
{
	uint32_t offset = 0;

	ASSERT(buf && length);

	while (!USART_0_is_byte_sent())
		;
	do {
		USART_0_write_byte(buf[offset]);
		while (!USART_0_is_byte_sent())
			;
	} while (++offset < length);

	return offset;
}

/*
 * \Read data from usart interface
 *
 * \param[in] buf A buffer to read data to
 * \param[in] length The size of a buffer
 *
 * \return The number of bytes read.
 */
static uint32_t USART_0_read(uint8_t *const buf, const uint16_t length)
{
	uint32_t offset = 0;

	ASSERT(buf && length);

	do {
		while (!USART_0_is_byte_received())
			;
		buf[offset] = USART_0_read_byte();
	} while (++offset < length);

	return offset;
}

/**
 * Example of using USART_0 to write the data which received from the usart interface to IO.
 */
void USART_0_example(void)
{
	uint8_t data[2];

	if (USART_0_read(data, sizeof(data)) == 2) {
		USART_0_write(data, 2);
	}
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
