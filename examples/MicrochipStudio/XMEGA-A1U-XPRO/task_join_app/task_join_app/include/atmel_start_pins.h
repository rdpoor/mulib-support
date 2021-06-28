/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <port.h>

/**
 * \brief Set PE2 output & pull configuration
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] output_pull_mode Pin output & pull mode
 */
static inline void PE2_set_output_pull_mode(const enum port_output_pull_mode output_pull_mode)
{
	PORTE_set_pin_output_pull_mode(2, output_pull_mode);
}

/**
 * \brief Set PE2 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PE2_set_dir(const enum port_dir dir)
{
	PORTE_set_pin_dir(2, dir);
}

/**
 * \brief Set PE2 input/sense configuration
 *
 * Enable/disable PE2 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PE2_set_isc(const PORT_ISC_t isc)
{
	PORTE_pin_set_isc(2, isc);
}

/**
 * \brief Set PE2 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PE2 is inverted
 *                     false = I/O on PE2 is not inverted
 */
static inline void PE2_set_inverted(const bool inverted)
{
	PORTE_pin_set_inverted(2, inverted);
}

/**
 * \brief Set PE2 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PE2_set_level(const bool level)
{
	PORTE_set_pin_level(2, level);
}

/**
 * \brief Toggle output level on PE2
 *
 * Toggle the pin level
 */
static inline void PE2_toggle_level()
{
	PORTE_toggle_pin_level(2);
}

/**
 * \brief Get level on PE2
 *
 * Reads the level on a pin
 */
static inline bool PE2_get_level()
{
	return PORTE_get_pin_level(2);
}

/**
 * \brief Set PE2 interrupt level
 *
 * Sets interrupt level for port
 *
 * \param[in] level Value to write to the port register
 */
static inline void PE2_int_level(const uint8_t level)
{
	PORTE_set_int_level(level);
}

/**
 * \brief Set PE2 interrupt vector 0 mask
 *
 * Sets interrupt mask for port vector 0
 *
 * \param[in] value Value to write to the port register
 */
static inline void PE2_int0_mask(const uint8_t value)
{
	PORTE_write_int0_mask(value);
}

/**
 * \brief Set PE2 interrupt vector 1 mask
 *
 * Sets interrupt mask for port vector 1
 *
 * \param[in] value Value to write to the port register
 */
static inline void PE2_int1_mask(const uint8_t value)
{
	PORTE_write_int1_mask(value);
}

/**
 * \brief Set PE3 output & pull configuration
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] output_pull_mode Pin output & pull mode
 */
static inline void PE3_set_output_pull_mode(const enum port_output_pull_mode output_pull_mode)
{
	PORTE_set_pin_output_pull_mode(3, output_pull_mode);
}

/**
 * \brief Set PE3 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PE3_set_dir(const enum port_dir dir)
{
	PORTE_set_pin_dir(3, dir);
}

/**
 * \brief Set PE3 input/sense configuration
 *
 * Enable/disable PE3 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PE3_set_isc(const PORT_ISC_t isc)
{
	PORTE_pin_set_isc(3, isc);
}

/**
 * \brief Set PE3 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PE3 is inverted
 *                     false = I/O on PE3 is not inverted
 */
static inline void PE3_set_inverted(const bool inverted)
{
	PORTE_pin_set_inverted(3, inverted);
}

/**
 * \brief Set PE3 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PE3_set_level(const bool level)
{
	PORTE_set_pin_level(3, level);
}

/**
 * \brief Toggle output level on PE3
 *
 * Toggle the pin level
 */
static inline void PE3_toggle_level()
{
	PORTE_toggle_pin_level(3);
}

/**
 * \brief Get level on PE3
 *
 * Reads the level on a pin
 */
static inline bool PE3_get_level()
{
	return PORTE_get_pin_level(3);
}

/**
 * \brief Set PE3 interrupt level
 *
 * Sets interrupt level for port
 *
 * \param[in] level Value to write to the port register
 */
static inline void PE3_int_level(const uint8_t level)
{
	PORTE_set_int_level(level);
}

/**
 * \brief Set PE3 interrupt vector 0 mask
 *
 * Sets interrupt mask for port vector 0
 *
 * \param[in] value Value to write to the port register
 */
static inline void PE3_int0_mask(const uint8_t value)
{
	PORTE_write_int0_mask(value);
}

/**
 * \brief Set PE3 interrupt vector 1 mask
 *
 * Sets interrupt mask for port vector 1
 *
 * \param[in] value Value to write to the port register
 */
static inline void PE3_int1_mask(const uint8_t value)
{
	PORTE_write_int1_mask(value);
}

/**
 * \brief Set USER_BUTTON output & pull configuration
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] output_pull_mode Pin output & pull mode
 */
static inline void USER_BUTTON_set_output_pull_mode(const enum port_output_pull_mode output_pull_mode)
{
	PORTQ_set_pin_output_pull_mode(2, output_pull_mode);
}

/**
 * \brief Set USER_BUTTON data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void USER_BUTTON_set_dir(const enum port_dir dir)
{
	PORTQ_set_pin_dir(2, dir);
}

/**
 * \brief Set USER_BUTTON input/sense configuration
 *
 * Enable/disable USER_BUTTON digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void USER_BUTTON_set_isc(const PORT_ISC_t isc)
{
	PORTQ_pin_set_isc(2, isc);
}

/**
 * \brief Set USER_BUTTON inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on USER_BUTTON is inverted
 *                     false = I/O on USER_BUTTON is not inverted
 */
static inline void USER_BUTTON_set_inverted(const bool inverted)
{
	PORTQ_pin_set_inverted(2, inverted);
}

/**
 * \brief Set USER_BUTTON level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void USER_BUTTON_set_level(const bool level)
{
	PORTQ_set_pin_level(2, level);
}

/**
 * \brief Toggle output level on USER_BUTTON
 *
 * Toggle the pin level
 */
static inline void USER_BUTTON_toggle_level()
{
	PORTQ_toggle_pin_level(2);
}

/**
 * \brief Get level on USER_BUTTON
 *
 * Reads the level on a pin
 */
static inline bool USER_BUTTON_get_level()
{
	return PORTQ_get_pin_level(2);
}

/**
 * \brief Set USER_BUTTON interrupt level
 *
 * Sets interrupt level for port
 *
 * \param[in] level Value to write to the port register
 */
static inline void USER_BUTTON_int_level(const uint8_t level)
{
	PORTQ_set_int_level(level);
}

/**
 * \brief Set USER_BUTTON interrupt vector 0 mask
 *
 * Sets interrupt mask for port vector 0
 *
 * \param[in] value Value to write to the port register
 */
static inline void USER_BUTTON_int0_mask(const uint8_t value)
{
	PORTQ_write_int0_mask(value);
}

/**
 * \brief Set USER_BUTTON interrupt vector 1 mask
 *
 * Sets interrupt mask for port vector 1
 *
 * \param[in] value Value to write to the port register
 */
static inline void USER_BUTTON_int1_mask(const uint8_t value)
{
	PORTQ_write_int1_mask(value);
}

/**
 * \brief Set USER_LED output & pull configuration
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] output_pull_mode Pin output & pull mode
 */
static inline void USER_LED_set_output_pull_mode(const enum port_output_pull_mode output_pull_mode)
{
	PORTQ_set_pin_output_pull_mode(3, output_pull_mode);
}

/**
 * \brief Set USER_LED data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void USER_LED_set_dir(const enum port_dir dir)
{
	PORTQ_set_pin_dir(3, dir);
}

/**
 * \brief Set USER_LED input/sense configuration
 *
 * Enable/disable USER_LED digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void USER_LED_set_isc(const PORT_ISC_t isc)
{
	PORTQ_pin_set_isc(3, isc);
}

/**
 * \brief Set USER_LED inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on USER_LED is inverted
 *                     false = I/O on USER_LED is not inverted
 */
static inline void USER_LED_set_inverted(const bool inverted)
{
	PORTQ_pin_set_inverted(3, inverted);
}

/**
 * \brief Set USER_LED level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void USER_LED_set_level(const bool level)
{
	PORTQ_set_pin_level(3, level);
}

/**
 * \brief Toggle output level on USER_LED
 *
 * Toggle the pin level
 */
static inline void USER_LED_toggle_level()
{
	PORTQ_toggle_pin_level(3);
}

/**
 * \brief Get level on USER_LED
 *
 * Reads the level on a pin
 */
static inline bool USER_LED_get_level()
{
	return PORTQ_get_pin_level(3);
}

/**
 * \brief Set USER_LED interrupt level
 *
 * Sets interrupt level for port
 *
 * \param[in] level Value to write to the port register
 */
static inline void USER_LED_int_level(const uint8_t level)
{
	PORTQ_set_int_level(level);
}

/**
 * \brief Set USER_LED interrupt vector 0 mask
 *
 * Sets interrupt mask for port vector 0
 *
 * \param[in] value Value to write to the port register
 */
static inline void USER_LED_int0_mask(const uint8_t value)
{
	PORTQ_write_int0_mask(value);
}

/**
 * \brief Set USER_LED interrupt vector 1 mask
 *
 * Sets interrupt mask for port vector 1
 *
 * \param[in] value Value to write to the port register
 */
static inline void USER_LED_int1_mask(const uint8_t value)
{
	PORTQ_write_int1_mask(value);
}

#endif /* ATMEL_START_PINS_H_INCLUDED */
