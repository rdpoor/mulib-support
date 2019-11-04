# mulib - A library for microcontrollers

mulib is curated collection of software modules written in C, specifically
tailored for microcontrollers and other resource-constrained environments.

## mulib's design philosophy

mulib strives towards the following design goals.

* Self-contained, pure C code: minimize dependencies on external libraries.
* Two-file implementation: whenever practical, each mulib module is embodied in
  one header file and one C file.
* Well-documented: API documentation is driven by documentation strings in the
  header files.
* Well-tested: unit tests validate the API, coverage tets validate the unit
  tests.
* Super portable: One concise mu_ports.c file defines mulib's interface to the
  target environment.
* Fast and Dangerous: mulib favors minimizing time and code space over argument
  validation.  (Coming soon: MU_ASSERT() will provide a safety net that can be
  switched on or off at compile time.
* Low threshold, high ceiling: A suite of demo programs take you from the
  simplest "flash an LED" example to complex multi-tasking applications.
*
* Yours to use: The entirety of mulib is covered under the MIT open source
  license.

## The modules

### mu_bcast

Low-overhead publish / subscribe message passing.

Status: Passing Tests.

### mu_task

Event handling.  Associates a `mu_time` with a `mu_task` (q.v.).

Status: Passing Tests.

### mu_heapsort

O(log N) in-place sorting of pointer-sized objects.

Status: Passing Tests.

### mu_log

No-nonsense logging with run-time control on reporting severity and multiple
reporting channels.

Status: Passing Tests.

### mu_ring

Fast, thread-safe queue for void * sized objects, designed for single-producer /
single-consumer applications.

Status: Passing Tests.

### mu_sched

Low-overhead, single-thread, interrupt safe, clock agnostic, run-to-completion
scheduler.

Status: Passing Tests.

### mu_strbuf

Safely append formatted string data to a buffer.

### mu_substring

Test and manipulate substrings in-place within a larger string.

### mu_task

Simple, versatile, fast message passing.  Commonly used for deferred function
calls.

Status: Passing Tests.

### mu_time

Functions to manipulate time values, properly handling roll-over.  Provides
interface to platform specific support in port/port.c.

Status: Passing Tests.

## Under Consideration

### mu_jemi

Json EMItter: Build complex data structures and emit them as JSON strings.

Status: In progress.

### mu_list

Manage linked lists of pointer-sized objects.

Status: In progress.

### mu_pool

Dynamically allocate and free fixed-sized objects from a static storage pool.

Status: In progress.

### mu_ring8, mu_ring16, mu_ring32, mu_ring64

Thread safe ring buffer for 8-, 16-, 32- and 64-bit wide objects.

Status: In progress.

## Developer Notes

* TODO: document API for each module

* TODO: create and document a series of demo applications, starting with "blink
an LED" through more complex ones.  Bonus points for cross platform / cross IDE.

* TODO: Inline some functions

### Future work

#### mu_string

Merge mu_strbuf and mu_substring into a single mu_string object.  The idea is
that a mu_string points to the start and end of the null-terminated C string.
You can use this either as a "cursor" (for writing) or a "view" (for reading)
into the underlying string.  

Implementation note: always carry the length of the underlying string so that
mu_strings are always safe.

```
typedef struct {
  uint8_t *buf;    // underlying data
  size_t length;   // length of buf
  int i0;          // 0 <= i0 <= i1
  int i1;          // i0 <= i1 < length
} mu_string_t;

// initialize s from src.  i0 = i1 = 0.
mu_string_t *mu_string_init(mu_string_t *s, char *c_str, size_t c_str_length);

// return &string[i0]
char *mu_string_data(mu_string_t *s);

// return length
size_t mu_string_capacity(mu_string_t *s);

// return length - i1
size_t mu_string_available(mu_string_t *s);

// return i1-i0
int mu_string_length(mu_string_t *s) { return s->i1 - s->i0; }

// reset i0 = i1 = 0;
mu_string_t *mu_string_reset(mu_string_t *s);

// take a substring of the underlying string.  negative end counts from end.
mu_string *mu_string_slice(mu_string *s, int start, int end);

// make a copy of src into dst.
mu_string_t *mu_string_duplicate(mu_string *dst, mu_string *src);

// Compare the referenced string to a C string
int mu_string_cmp(mu_string_t *s, const char *cstring);

// Return true if the referenced string is equal to a C string.
bool mu_string_eq(mu_string_t *s, const char *cstring);

// the following functions copy bytes:

// Copy data from string[i0] to string[i1] (plus null termination) into c_str.
char *mu_string_extract(mu_string_t *s, char *c_str, size_t c_str_length);

// append string referred to by src onto dst
char *mu_string_append(mu_string_t *dst, mu_string_t *src);

// sprintf() into &string[i1]
int mu_string_sprintf(mu_string_t *s, const char *fmt, ...);


```

#### double buffered async writes

Imagine the following task.  The idea is that mu_async handles double-buffered
writes, calling the target task when a buffer becomes available.

```
typedef struct {
  int state;
  mu_async_t async;
} writer_state_t;

mu_writer_state_t s_writer_state;
mu_task_t s_writer_task;

void init() {
  mu_async_init(&s_writer_state.async, s_sched, async_driver, async_buf, ASYNC_BUF_SIZE);

  writer_state.state = 0;
  mu_task_init_at(&s_writer_task, mu_time_now(), writer_fn, s_write_state, "W");
  mu_sched_add(&s_sched, &s_writer_task);
}

void writer_fn(void *self, void *arg) {
  writer_state_t *s = (writer_state_t *)self;

  if (s->state < 3) {
    // For states 0, 1, 2: get a buffer from the async driver.  Set up to call
    // s_writer_task if no buffer is immediately available.
    strbuf_t *sbuf = mu_async_get_buffer(s->async, &s_writer_task);
    if (sbuf == NULL) {
      return;
    }
    // Got a buffer.  Print some data into it, post it to the async driver for
    // printing to the screen and set up to call s_writer_task when the next
    // buffer becomes available.  (Note that we don't pass sbuf as an argument
    // because it must be the one we were given in `mu_async_get_buffer()`.)
    sbuf_printf("%d\r\n", s->state);
    mu_async_post_buffer(s->async, &s_writer_task);
    s->state += 1;

  } else {
    // after printing out three things, wait for 5 seconds and start again...
    mu_task_advance_time(&s_writer_task, mu_seconds_to_dt(5.0));
    mu_sched_add(&s_sched, &s_writer_task);
    s->state = 0;
  }
}

```
