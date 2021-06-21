#include <atmel_start.h>
#include "tower_eg/tower.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	tower_init();
	while (1) {
		tower_step();
	}
}
