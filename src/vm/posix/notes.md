mu_vm for POSIX notes

mu_vm for POSIX is implemented with two threads: The primary thread runs the
primary application (e.g. the unit test suite).  The secondary thread controls
the external events, such as button presses, Real Time Clock events and serial
I/O.

## Primary Thread

```c
void mu_vm_sleep_until(mu_vm_time_t t) {
  while (mu_vm_time_precedes(t, s_current_time) && !interrupt_occurred()) {
    pthread_yield();
  }
}

void mu_vm_sleep(void) {
  while (!interrupt_occurred()) {
    pthread_yield();
  }
}
```

## Secondary Thread

```c
void set_rtc(mu_vm_time_t t) {
  s_current_time = t;
  vm_rtc_interrupt(t);
}

void set_button_state(bool state) {
  if (state != s_button_state) {
    s_button_state = state;
    vm_button_interrupt(state);
  }
}

void send_serial_byte(uint8_t byte) {
  s_rx_byte = byte;
  vm_serial_interrupt(byte);
}

bool led_get_state(void) {
  return s_led_state;
}


```
