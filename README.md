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
O(log N) heapsort with user-provided comparison function.  NOTE: mu_buf may be
superseded by mu_bstore and mu_pstore (byte store and pointer store), since
the overhead of handling variable size may not be worth it.

### mu_bufref

Supports slices (aka views, aka substrings) into a mu_buf.  Especially useful
as a means for "no copy" (in-place) string manipulation.  NOTE: mu_bufref may
be superseded by mu_bview and mu_pview for views into mu_bstore and mu_pstore.

### mu_bitvec

Bit vector operations on individual bits: set, clear, toggle, test.  Includes
functions that operate on the entire vector.

### mu_list

Supports classic singly linked list operations: push, pop, insert, delete,
and a traversal method for user-supplied functions.

### mu_pstore

Operations on a collection of pointer-sized objects.  Includes functions for
push, pop, peek, insert, delete, insertion sort and heapsort.

### mu_queue

Efficient, extensible first-in, first-out queue operations, built atop mu_list.

### mu_spscq

Thread-safe, lock-free, single-producer, single-consumer queue.  It is designed
primarily for use by mu_sched to transfer control between interrupt and the
foreground levels, but -- despite its intentionally obscure name -- may be
useful elsewhere.

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

### mu_log

Flexible logging with run-time control on reporting severity and multiple reporting channels.

### mu_sched

Low-overhead, single-thread, interrupt safe, clock agnostic, run-to-completion scheduler.

### mu_string

For reading, perform in-place "zero copy" operations on strings.  Take slices of substrings, compare them.  For writing, perform safe sprintf() and efficient append operations.

## Developer Notes
