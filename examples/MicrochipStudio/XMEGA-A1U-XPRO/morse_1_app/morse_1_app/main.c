#include <atmel_start.h>
#include "morse_1.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	morse_1_init();
	while (1) {
		morse_1_step();
	}
}
