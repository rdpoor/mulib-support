#include "printf_support.h"
#include "mu_vm.h"

// ================
// support for printf()

int _write(int file, char *ptr, int len) {
  // void file;
  int n = len;

  while (n-- > 0) {
    while (!mu_vm_serial_can_write())
  		;
    mu_vm_serial_write(*ptr++);
  }
  return len;
}

int _read(int file, char *ptr, int len) {
  // void file;
  int n = len;

  while (n-- > 0) {
    while (!mu_vm_serial_can_read())
			;
    *ptr++ = mu_vm_serial_read();
  }
  return len;
}
