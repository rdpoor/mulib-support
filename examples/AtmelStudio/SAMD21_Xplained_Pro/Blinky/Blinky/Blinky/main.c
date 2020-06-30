#include <atmel_start.h>
#include "blinky.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
  blinky_init();

	/* Replace with your application code */
	while (1) {
    blinky_step();
	}
}
