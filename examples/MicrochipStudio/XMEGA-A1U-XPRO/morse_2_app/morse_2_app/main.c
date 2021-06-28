#include <atmel_start.h>
#include "morse_2.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	morse_2_init();
	while (1) {
		morse_2_step();
	}
}
