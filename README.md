# mulib - A library for microcontrollers  [![Build Status](https://travis-ci.com/rdpoor/mulib.svg?branch=master)](https://travis-ci.com/rdpoor/mulib)

`mulib` is curated collection of software modules written in C, specifically
tailored for microcontrollers and other resource-constrained environments.

`mulib` features:

* a fast and lithe run-to-completion scheduler
* low-overhead timing and event handling
* advanced in-place string handling
* an in-memory publish / subscribe system
* flexible logging
* ... and a variety of other useful modules

## mulib's design philosophy

mulib strives towards the following design goals.

* **Self-contained, pure C code:** minimize dependencies on external libraries.
* **Two-file implementation:** whenever practical, each module is embodied in
    one header file and one C file.
* **Well-documented:** API documentation is driven by documentation strings in
    the header files.
* **Well-tested:** unit tests validate the API, coverage tets validate the unit
    tests.
* **Super portable:** One concise mu_ports.c file defines mulib's interface to
    the target environment.
* **Fast and Dangerous:** mulib favors minimizing time and code space over
    argument validation.  But note that `MU_ASSERT()` provides a safety net that
    can be switched on or off at compile time.
* **Single-threaded scheduler:** There are many benefits to a single-threaded
    scheduler -- see [About Run To Completion Schedulers](./RunToCompletion.md)
    for the reasons behind this choice.
* **Low threshold, high ceiling:** A suite of demo programs take you from the
    simplest "blink an LED" example to complex multi-tasking applications.
* **Yours to use:** The entirety of mulib is covered under the MIT open source
    license.

## The modules

### mu_buf

Safe read and write access for buffers of homogeneous data.  Includes in-place
O(log N) heapsort with user-provided comparison function.

### mu_bufref

Supports slices (aka views, aka substrings) into a mu_buf.  Especially useful
as a means for "no copy" (in-place) string manipulation.

### mu_bitvec

Bit vector operations on individual bits: set, clear, toggle, test.  Includes
functions that operate on the entire vector.

### mu_list

Supports classic singly linked list operations: push, pop, insert, delete,
and a traversal method for user-supplied functions.

### mu_queue

Efficient, extensible first-in, first-out queue operations, built atop mu_list.

### mu_task

A simple implementation of deferred function calls, with a compile time option
for profiling.  Used extensively by `mu_sched`.

### mu_time

Functions to manipulate time values, properly handling roll-over.  Interfaces to
platform specific ports/.../port_time.c

## In the laboratory

These are modules yet to be written and tested (or in some cases, ported from a
previous version of mulib).

### mu_bcast

Low-overhead publish / subscribe message passing.

Status: 0% test coverage

Status: 82% test coverage.

### mu_cbuf

Fast, fixed-length, thread-safe circular buffer for void * sized objects,
designed for single-producer / single-consumer applications.  Used by the
scheduler to transfer information between interrupt and foreground level.

### mu_log

Flexible logging with run-time control on reporting severity and multiple reporting channels.

### mu_sched

Low-overhead, single-thread, interrupt safe, clock agnostic, run-to-completion scheduler.

Status: 100% test coverage.

### mu_string

For reading, perform in-place "zero copy" operations on strings.  Take slices of substrings, compare them.  For writing, perform safe sprintf() and efficient append operations.

Notes: consider `mu_strbuf` as the underlying storage with length:

    typedef struct {
      strbuf_type_t *bytes;
      size_t capacity;
    } mu_strbuf_t;

and most string operations use a `mu_strbuf_t` as follows:

    typedef struct {
      mu_strbuf_t *strbuf;
      size_t start;
      size_t end;
    } mu_str_t;

This creates an extra level of indirection, but allows multiple views onto the
same string.  This also means that mu_strbuf object could be use for circular
buffers, read-only strings, extensible strings, etc.  Also, strbuf can have its
on (simpler) API and unit tests.

#### Attribute bits

We could steal a few high-order bits from the capacity field to store attribute
bits, such as:
  - is_readonly [1 bit]
  - item_width [1, 2, 4, 8] mapped to [0, 1, 2, 3] [2 bits]

### mu_iostream

General purpose asynchronous I/O streaming operations.  For read operations, call a consumer task when more data is available, for write operations, call a producer task when more data is requested.

Status: concept only.  The design challenge will be finding the best boundaries into platform specific I/O operations.

### mu_jemi

Json EMItter: Build complex data structures and emit them as JSON strings.

Status: In progress.

## Developer Notes

* TODO: document API for each module

* TODO: create and document a series of demo applications, starting with "blink
an LED" through more complex ones.  Bonus points for cross platform / cross IDE.

* In mu_string, create functions that modify underlying buffer: insert, delete,
  replace.  These are actually all the same function: insert has a non-empty
  src slice and empty dst slice, delete has an empty src slice and a non-empty
  dst slice, replace has non-empty src and dst.

* Documentation: find a c-based project on github with great documentation and
  emulate its documentation structure.

* Refactor: all chars should be unsigned chars (or uint8_t).  Pick one.

### Thoughts on Time

Design idea: The native port provides a few core functions:

* typedef xxx port_rtc_t;
* void port_rtc_init(void);
* port_rtc_t port_rtc_now();
* port_rtc_t port_rtc_difference(port_rtc_t t1, port_rtc_t t2);
* port_rtc_t port_rtc_offset(port_rtc_t, port_rtc_dt);
* bool port_rtc_precedes(port_rtc_t t0, port_rtc_t t1);
* bool port_rtc_equals(port_rtc_t t0, port_rtc_t t1);
* bool port_rtc_follows(port_rtc_t t0, port_rtc_t t1);
* float port_rtc_to_seconds(port_rtc_t dt);

void mu_rtc_init(void);
mu_rtc_t mu_rtc_now();
float mu_rtc_seconds_since_init();

### Future work

* Can we sensibly emulate Scheme-style continuations or Note-style promises?

* Can we sensibly emulate functional list operations (map, collect, etc)?
