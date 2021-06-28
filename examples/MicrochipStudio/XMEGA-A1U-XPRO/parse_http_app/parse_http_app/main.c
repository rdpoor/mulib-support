#include <atmel_start.h>
#include "parse_http.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	parse_http_init();
	while (1) {
		parse_http_step();
	}
}
