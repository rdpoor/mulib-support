#include "platform_test.h"
#include <atmel_start.h>
#include <stdio.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	platform_test_init();

	/* Replace with your application code */
	while (1) {
		platform_test_step();
	}
}
