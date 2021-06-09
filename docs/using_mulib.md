# `mulib` documentation

## Getting started with `mulib`:

* [How `mulib` is organized](./docs/using_mulib.md#mulib_organization)
* [Browsing code examples that use `mulib`](./docs/using_mulib.md#browsing_mulib)
* [Downloading `mulib`](./docs/using_mulib.md#downloading_mulib)
* [Running `mulib` code examples](./docs/using_mulib.md#running_examples)
* [Writing a `mulib`-based application for an existing platform](./docs/using_mulib.md#writing_app)
* [Porting `mulib` to a new IDE or platform](./docs/using_mulib.md#porting_mulib)

## Advanced topics:

* [Reducing `mulib`'s code footprint](./docs/using_mulib.md#cherry_picking)
* [Using `mulib` to minimize power consumption](./docs/using_mulib.md#low_power_sleep)

## Administrivia

* [Reporting bugs and feature requests](./docs/using_mulib.md#reporting_bugs)
* [Contributing code or bugfixes](./docs/using_mulib#contributing_code)

## Getting Started

<h3 id="mulib_organization">How mulib is organized.</h3>

`mulib` is split across two git repositories:
The [`mulib`](https://github.com/rdpoor/mulib.git) repository contains just the source files that comprise mulib.
The [`mulib-support`](https://github.com/rdpoor/mulib-support.git) repository contains documentation, code examples,
unit tests, etc.

This approach makes it easy for you to incorporate mulib into your own projects
by cloning the `mulib` repository without dragging in any extraneous files.

The mulib repository is simple:
* mulib.[ch] are top level files that provide declarations and initializations
for all of the mulib modules.
* mulib/core/ contains the individual core mulib modules.
* mulib/extras/ contain modules that aren't considered central to mulib, but
are useful nonetheless.  (Yes, the distinction between core/ and extras/ is a
bit arbitrary.)

The mulib-support repository is more extensive.  Its main components are:
* docs/ provides documentation for mulib.  If you want the entire mulib library,
you can `#include "mulib.h"` in your code, and call `mulib_init()` upon starup.
However you don't have to -- you can pick and choose individual mulib modules.
See [Incorporating `mulib` into my project](#incorporating_mulib) below for
more information.
* examples/ is a suite of IDE projects that demonstrate mulib functions.  The
examples directory is further subdivided:
  * \<IDE_NAME\>/\<DEV_BOARD_NAME\>/\<demo_application_name\>/ contains a complete,
runnable project for a specific IDE and specific embedded processor development
board.  For example:
  * STM32CubeIDE/NUCLEO-G431RB/morse_2_app contains a project file that runs the
morse-2 example for the ST Microsystems IDE on a NUCLEO-G431RB development board.
  * shared/ contains common source code among all of the example projects.
* laboratory/ is a place where code languishes until the Powers That Be decide
if it should be promoted to the mulib or mulib-support/examples or simply
deleted.  Think of it as the developer's playpen.
* mulib-test contains unit tests for all the modules in the mulib repository.

<h3 id="browsing_mulib">Browsing code examples that use `mulib`</h3>

The easiest way to get a sense of mulib's style and capabilities is to look at
some of the code examples.  You can find the common (non-platform specific)
code examples in [the mulib-support examples directory]
(https://github.com/rdpoor/mulib-support/tree/master/demos/shared).  

If you want to view the API for individual mulib modules, you can find them in
the [mulib core repository](https://github.com/rdpoor/mulib/tree/main/core).

<h3 id="downloading_mulib">Downloading `mulib`</h3>

When getting started, you should to download both mulib-support and mulib.  To
do this, open a shell window and:

```
$ cd <my project directory>
$ git clone --branch=master https://github.com/rdpoor/mulib-support.git mulib-support
$ cd mulib-support
$ git clone --branch=main https://github.com/rdpoor/mulib.git mulib
```

If you've already downloaded mulib-support and mulib, the recipe for getting
the latest versions is:

```
$ cd <my project directory>
$ git fetch
$ git pull
$ cd mulib-support
$ git fetch
$ git pull
```

<h3 id="running_examples">Running `mulib` code examples</h3>

To run one of the code examples, you will need an embedded system development
board and an IDE to go with it.  Once you have chosen and acquired a development
board, consult the appropriate vendor site for instructions on how to download
and run the IDE.

<h3 id="writing_app">Writing a `mulib`-based application for an existing platform</h3>

This will be how to create an application based on an existing mu\_platform and
mu\_stdlib.


<h3 id="porting_mulib">Porting mulib to a new IDE or platform</h3>

This will be how to create a mu\_platform and mu\_stdlib and how to test it
(e.g. test\_stdlib).

## Advanced topics:

<h3 id="cherry_picking">Reducing the size of `mulib`-based apps</h3>

This will be about using individual `#include mu_xxx.h` files rather than using
the catch-all `#include "mulib.h"`

<h3 id="low_power_sleep">Using `mulib` to minimize power consumption</h3>

This will be a longer discussion that touches on:
* Sleep / Low Power States for your target processor
* Defining a real-time clock that can wake the processor from sleep
* Running the scheduler from the real time clock
* Creating an idle task for the scheduler that puts the processor into low-power mode
* `will_sleep` and `did_wake` considerations

## Administrivia

<h3 id="reporting_bugs">Reporting bugs and feature requests</h3>

This will be about how to use the github issues mechanism.

<h3 id="contributing_code">Contributing code or bugfixes</h3>

This will be about how to create a pull request, either for mulib or for
mulib-support.
