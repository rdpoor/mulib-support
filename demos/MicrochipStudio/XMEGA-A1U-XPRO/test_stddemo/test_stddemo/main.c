#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	mulib_init();
	mu_stddemo_init();

	/* Replace with your application code */
	test_stddemo_init();
	while (1) {
		test_stddemo_step();
	}
}
