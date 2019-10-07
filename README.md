# mulib - A library for microcontrollers

mulib is curated collection of software modules written in C, specifically
tailored for microcontrollers and other resource-constrained environments.

## mulib's design philosophy

mulib strives towards the following design goals.

* Self-contained: minimize dependencies on external libraries
* Two-file implementation: whenever practical, mulib modules are embodied in one
  header file and one C file.
* Well-tested and documented: unit tests validate the API, which is clearly
  documented in the header files.
* Fast and Dangerous: mulib favors minimizing time and code space over argument
  validation.  However, MU_ASSERT() provides a safety net that can be switched
  on or off at compile time.
* Yours to use: The entirety of mulib is covered under the MIT open source
  license.

## The modules

### mu_bcast

Low-overhead publish / subscribe message passing.

Status: Passing Tests.

### mu_evt

Event handling.  Associates a `mu_time` with a `mu_task` (q.v.).

Status: Passing Tests.

### mu_jemi

Json EMItter: Build complex data structures and emit them as JSON strings.

Status: In progress.

### mu_mu_heapsort

O(log N) in-place sorting of pointer-sized objects.

Status: Passing Tests.

### mu_list

Manage linked lists of pointer-sized objects.

Status: In progress.

### mu_log

No-nonsense logging with run-time control on reporting severity and multiple
reporting channels.

Status: Passing Tests.

### mu_sched

Fast, lightweight run-to-completion scheduler.  

Status: Passing Tests.  (Needs work & testing for interrupt-safe actions.)

### mu_strbuf

Safely append formatted string data to a buffer.

### mu_task

Simple, versatile, fast message passing.  Commonly used for deferred function
calls.

Status: Passing Tests.

### mu_pool

Dynamically allocate and free fixed-sized objects from a static storage pool.

Status: In progress.

### mu_ring

Thread safe ring buffer for pointer-sized objects.  Also provides variants to
handle 8-, 16-, 32- and 64-bit wide objects.

Status: In progress.

### mu_sched

Clock agnostic, single-thread, run-to-completion scheduler

Status: Passing Tests.

### mu_time

Functions to manipulate time values, properly handling roll-over.  Requires
platform specific support in port/port.h

Status: Passing Tests.
