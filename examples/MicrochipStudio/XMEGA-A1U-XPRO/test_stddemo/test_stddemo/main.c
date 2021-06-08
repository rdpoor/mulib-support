#include <atmel_start.h>
#include "test_stddemo/test_stddemo.h"
#include <stddef.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	test_stddemo_init();
	while (1) {
		test_stddemo_step();
	}
}
