#include <atmel_start.h>
#include "join_wto_eg/join_wto_eg.h"
#include <stddef.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	join_wto_eg_init();
	while (1) {
		join_wto_eg_step();
	}
}
