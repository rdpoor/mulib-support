/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

/*! The buffer size for USART */
#define EDBG_COM_BUFFER_SIZE 16

struct usart_async_descriptor EDBG_COM;

static uint8_t EDBG_COM_buffer[EDBG_COM_BUFFER_SIZE];

struct calendar_descriptor CALENDAR_0;

void EXTERNAL_IRQ_0_init(void)
{
	_gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);

	// Set pin direction to input
	gpio_set_pin_direction(USER_BUTTON, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(USER_BUTTON,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(USER_BUTTON, PINMUX_PA15A_EIC_EXTINT15);

	ext_irq_init();
}

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void EDBG_COM_CLOCK_init()
{

	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM3);
	_gclk_enable_channel(SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void EDBG_COM_PORT_init()
{

	gpio_set_pin_function(EDBG_COM_TX, PINMUX_PA22C_SERCOM3_PAD0);

	gpio_set_pin_function(EDBG_COM_RX, PINMUX_PA23C_SERCOM3_PAD1);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void EDBG_COM_init(void)
{
	EDBG_COM_CLOCK_init();
	usart_async_init(&EDBG_COM, SERCOM3, EDBG_COM_buffer, EDBG_COM_BUFFER_SIZE, (void *)NULL);
	EDBG_COM_PORT_init();
}

void CALENDAR_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, RTC);
	_gclk_enable_channel(RTC_GCLK_ID, CONF_GCLK_RTC_SRC);
}

void CALENDAR_0_init(void)
{
	CALENDAR_0_CLOCK_init();
	calendar_init(&CALENDAR_0, RTC);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PB30

	gpio_set_pin_level(LED0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED0, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_0_init();

	EDBG_COM_init();

	CALENDAR_0_init();
}
