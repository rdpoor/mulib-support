# `mulib` - Software components for microcontrollers  [![Build Status](https://travis-ci.com/rdpoor/mulib-support.svg?branch=master)](https://travis-ci.com/rdpoor/mulib-support)

`mulib` is a curated collection of software components written in C, designed
specifically for microcontollers and other resource-constrained environments.
`mulib` features include:

* A fast and lithe run-to-completion (single threaded) scheduler
* Support for deferred events ('thunks')
* Fast, safe, copy-free string handling
* Efficient bit vector operations

## `mulib`'s design philosophy: Small, Fast and Dangerous

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

## Quick Start links for using `mulib`:

* [I want to understand how mulib is organized.](./docs/using_mulib.md#mulib_organization)
* [I want to browse some code examples that use `mulib`](./docs/using_mulib.md#browse_mulib)
* [I want to download `mublib`](./docs/using_mulib.md#download_mulib)
* [I want to run `mulib` code examples](./docs/using_mulib.md#run_examples)
* [I want to write my own `mulib` application for an existing platform](./docs/using_mulib.md#my_first_app)
* [I want to port mulib to a new IDE or platform](./docs/using_mulib.md#porting_mulib)

## Advanced topics:

* [I want to minimize power consumption in my `mulib` application](./docs/using_mulib.md#low_power_sleep)

## Administrivia

* [I have a suggestion or feature request for `mulib`](./docs/using_mulib.md#how_to_suggest)
* [I want to report a bug](./docs/using_mulib.md#report_a_bug)
* [I want to contribute code](./docs/using_mulib#contribute_code)
* [I want to contribute money](./docs/using_mulib#manna)
