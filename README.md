# mulib - A library for microcontrollers  # [![Build Status](https://travis-ci.com/rdpoor/mulib.svg?branch=master)](https://travis-ci.com/rdpoor/mulib)

`mulib` is curated collection of software modules written in C, specifically
tailored for microcontrollers and other resource-constrained environments.

`mulib` features a fast and lithe run-to-completion scheduler, an in-memory
publish / subscribe system, advanced in-place string handling, flexible logging
and a list of other useful modules.

## mulib's design philosophy

mulib strives towards the following design goals.

* **Self-contained, pure C code:** minimize dependencies on external libraries.
* **Two-file implementation:** whenever practical, each mulib module is embodied in one header file and one C file.
* **Well-documented:** API documentation is driven by documentation strings in the header files.
* **Well-tested:** unit tests validate the API, coverage tets validate the unit tests.
* **Super portable:** One concise mu_ports.c file defines mulib's interface to the target environment.
* **Fast and Dangerous:** mulib favors minimizing time and code space over argument validation.  But note that `MU_ASSERT()` provides a safety net that can be switched on or off at compile time.
* **Single-threaded scheduler:** There are many benefits to a single-threaded scheduler -- see [About Run To Completion Schedulers](./RunToCompletion.md) for the reasons behind this choice.
* **Low threshold, high ceiling:** A suite of demo programs take you from the simplest "flash an LED" example to complex multi-tasking applications.
* **Yours to use:** The entirety of mulib is covered under the MIT open source license.

## The modules

### mu_bcast

Low-overhead publish / subscribe message passing.

Status: 0% test coverage

### mu_bvec

Bit vector module to `set`, `clear`, `test` individual bits.  Includes functions that operate on the entire vector, including test for all zeros or ones, `and`, `or`, `xor` bit vectors, etc.

Status: 82% test coverage.

### mu_heapsort

O(log N) in-place sorting of pointer-sized objects.

Status: 100% test coverage.

### mu_log

No-nonsense logging with run-time control on reporting severity and multiple reporting channels.

Status: 98% test coverage.

### mu_queue

Fast, thread-safe queue for void * sized objects, designed for single-producer / single-consumer applications.  Used by the scheduler to transfer between interrupt and foreground level.

Status: 100% test coverage.

### mu_sched

Low-overhead, single-thread, interrupt safe, clock agnostic, run-to-completion scheduler.

Status: 100% test coverage.

### mu_string

For reading, perform in-place "zero copy" operations on strings.  Take slices of substrings, compare them.  For writing, perform safe sprintf() and efficient append operations.

Status: 83% test coverage.

### mu_task

Simple, versatile, fast message passing.  Commonly used for deferred function calls, used extensively by `mu_sched`.

Status: 74% test coverage.

### mu_time

Functions to manipulate time values, properly handling roll-over.  Provides interface to platform specific support in port/port.c.

Status: 100% test coverage.

## In the laboratory

### mu_iostream

General purpose asynchronous I/O streaming operations.  For read operations, call a consumer task when more data is available, for write operations, call a producer task when more data is requested.

Status: concept only.  The design challenge will be finding the best boundaries into platform specific I/O operations.

### mu_jemi

Json EMItter: Build complex data structures and emit them as JSON strings.

Status: In progress.

### mu_list

Manage linked lists of pointer-sized objects.

Status: In progress.

### mu_pool

Dynamically allocate and free fixed-sized objects from a static storage pool.

Status: In progress.

### mu_queue8, mu_queue16, mu_queue32, mu_queue64

Thread safe ring buffer for 8-, 16-, 32- and 64-bit wide objects.

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

### Future work

* Can we sensibly emulate Scheme-style continuations or Note-style promises?

* Can we sensibly emulate functional list operations (map, collect, etc)?
