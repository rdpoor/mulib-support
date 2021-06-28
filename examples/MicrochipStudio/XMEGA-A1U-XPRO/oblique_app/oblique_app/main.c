#include <atmel_start.h>
#include "oblique.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	oblique_init();
	while (1) {
		oblique_step();
	}
}
