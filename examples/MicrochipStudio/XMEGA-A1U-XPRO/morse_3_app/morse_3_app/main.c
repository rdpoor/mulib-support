#include <atmel_start.h>
#include "morse_3.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	morse_3_init();
	while (1) {
		morse_3_step();
	}
}
