/**
@brief Spawn a thread that reads the keyboard and passes any activity along to mu_kbd_io

*/
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mu_kbd_io.h"
#include "kbd_read.h"

pthread_t thread_id;

void *reader_thread(void* vargp);

void start_kbd_reader_thread(void) {
  mu_kbd_enter_noncanonical_mode(); // so we dont wait for line feeds,  and we dont echo
  pthread_create(&thread_id, NULL, reader_thread, NULL);
  atexit(mu_kbd_exit_noncanonical_mode); // restores terminal attributes
}

void *reader_thread(void* vargp)
{
    while(1) {
      char ch = mu_kbd_get_key_press(); // assuming we are in noncanonical mode this won't hang
      if(ch) {
        fire_kbd_io_callback(ch);
      }
    }
    return NULL;
}

