# `mulib` - Software components for microcontrollers  [![Build Status](https://travis-ci.com/rdpoor/mulib-support.svg?branch=master)](https://travis-ci.com/rdpoor/mulib-support)

`mulib` is a curated collection of software components written in C, designed
specifically for microcontollers and other resource-constrained environments.
`mulib` features include:

* A fast and lithe run-to-completion (single threaded) scheduler
* Support for deferred events ('thunks')
* Fast, safe, copy-free string handling
* Efficient bit vector operations

## `mulib`'s philosophy: Small, Fast and Dangerous

`mulib`'s design is guided by the following principals:

* **Self-contained, pure C code**: `mulib` minimizes dependencies on external
libraries, except for the most common ones such as `stdint` and `stdbool`.
* **Two-file implementation**: Whenever practical, each `mulib` module is
embodied as one header file and one C file.  This lets you pick and choose just
the modules you need for your application.
* **No malloc.  Ever.**: `mulib`'s modules accept user-allocated data structures.
If your application already has malloc, you're free to use it, but `mulib` will
never malloc behind your back.
* **Well tested and supported.**: `mulib` is validated through ample unit testing
and continuous integration.
* **Super portable.**: To port `mulib` to a new target environment, you only
need to follow a well-documented process to create a few interface functions,
none of it requiring any assembly code.
* **Fast and Dangerous.**: `mulib` favors maximizing speed and minimizing code
space over safety.  In general, `mulib` assumes that you're passing valid
parameters and that your code implements argument validation as needed.
* **Single-threaded scheduler.**: Much of `mulib`'s small code footprint and
low-overhead is attributed to its single-threaded, run to completion scheduler.
See [About Single-Threaded Schedulers](./docs/about_single_threaded_schedulers.md)
for the reasons -- and implications -- behind this choice.
* **Low threshold, high ceiling**: A suite of well-documented code examples
guide you from the simplest "blink an LED" demo to complex multi-tasking
applications.
* **Yours to use**: The entirety of `mulib` is covered under the permissive MIT
Open Source license.


[Read the mulib documentation.](./docs/using_mulib.md)
