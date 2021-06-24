#include <atmel_start.h>
#include "platform_test/platform_test.h"
#include <stddef.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	platform_test_init();
	while (1) {
		platform_test_step();
	}
}
