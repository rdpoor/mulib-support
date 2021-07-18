/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 // =============================================================================
 // includes

#include <stdio.h>
#include "mu_test_utils.h"
#include "mu_version.h"
#include "mu_drunken_bishop.h"

// =============================================================================
// types and definitions

// =============================================================================
// private declarations

static int read_output_from_shell_command(char *command, char *output_buffer);

int mu_bvec_test();
int mu_cirq_test();
int mu_dlist_test();
int mu_fsm_test();
int mu_list_test();
int mu_log_test();
int mu_pstore_test();
int mu_queue_test();
int mu_rtc_test();
int mu_sched_test();
// int mu_spsc_test();
int mu_str_test();
int mu_strbuf_test();
int mu_task_test();
int mu_time_test();
int mu_timer_test();
int mu_vect_test();
int mu_version_test();
int mu_ansi_term_test();
int mu_random_test();
int mu_drunken_bishop_test();

// =============================================================================
// local storage

// =============================================================================
// public code

int main(void) {
  char readbuf[80];

  printf("\r\nstarting mu_test...");
  printf("mu_version: %s\n",mu_version());

  mu_test_init();

  // get a string we can use for our ascii art seed
  // first we try all the source files
  // if that fails, we use the mu_version string
  int err = read_output_from_shell_command("md5sum ../../mulib/core/*.c ../../mulib/extras/*.c | md5sum", readbuf);
  if(err) {
    sprintf(readbuf, "%s", mu_version());
  }
  //else printf("dir hash is: %s",readbuf);
  print_random_art_from_string(readbuf, 25);

  mu_bvec_test();
  mu_cirq_test();
  mu_dlist_test();
  mu_fsm_test();
  mu_list_test();
  mu_log_test();
  mu_pstore_test();
  mu_queue_test();
  mu_rtc_test();
  mu_sched_test();
  // mu_spsc_test();
  mu_str_test();
  mu_strbuf_test();
  mu_task_test();
  mu_time_test();
  mu_timer_test();
  mu_vect_test();
  mu_version_test();
  // extras
  mu_ansi_term_test();
  mu_random_test();
  mu_drunken_bishop_test();

  printf("ending mu_test: %d error%s out of %d test%s\r\n",
         mu_test_error_count(),
         mu_test_error_count() == 1 ? "" : "s",
         mu_test_count(),
         mu_test_count() == 1 ? "" : "s");

  return mu_test_error_count();  // return error code 0 on success

  
}

// =============================================================================
// private code

static int read_output_from_shell_command(char *command, char *output_buffer) {
  FILE *input;
  input = popen (command, "r");
  if (!input)
    {
      fprintf (stderr, "incorrect parameters.\n");
      return -1;
    }
  while(fgets(output_buffer, 80, input))
  
  if (pclose (input) != 0)
    {
      fprintf (stderr, "Could not run shell command or other error.\n");
      return -1;
    }
    return 0;
}
