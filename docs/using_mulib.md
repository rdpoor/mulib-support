# Introducing `mulib`

## Getting Started

<h3 id="mulib_organization">I want to understand how mulib is organized.</h3>

mulib is split across two git repositories:
The `mulib` repository contains just the source files that comprise mulib.
The `mulib-support` repository contains documentation, code examples,
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

<h3 id="browse_mulib">I want to browse some code examples that use `mulib`</h3>

The easiest way to get a sense of mulib's style and capabilities is to look at
some of the code examples.  You can find the common (non-platform specific)
code examples in [the mulib-support examples directory]
(https://github.com/rdpoor/mulib-support/tree/master/demos/shared).  

If you want to view the API for individual mulib modules, you can find them in
the [mulib core repository](https://github.com/rdpoor/mulib/tree/main/core).

<h3 id="download_mulib">I want to download `mulib`</h3>

When getting started, you will probably want to download both mulib-support and
mulib.  To do this, open a shell window and:

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

<h3 id="run_examples">I want to run `mulib` code examples</h3>

To run one of the code examples, you will need an embedded system development
board and an IDE to go with it.  Once you have chosen and acquired a development
board, consult the appropriate vendor site for instructions on how to download
and run the IDE.

<h3 id="my_first_app">I want to write my own `mulib` application for an existing platform</h3>

This will be how to create an application based on an existing mu\_platform and
mu\_stdlib.


<h3 id="porting_mulib">I want to port mulib to a new IDE or platform</h3>

This will be how to create a mu\_platform and mu\_stdlib and how to test it
(e.g. test\_stdlib).

## Advanced topics:

<h3 id="low_power_sleep">I want to minimize power consumption in my `mulib` application</h3>

This will be a longer discussion that touches on:
* Sleep / Low Power States for your target processor
* Defining a real-time clock that can wake the processor from sleep
* Running the scheduler from the real time clock
* Creating an idle task for the scheduler that puts the processor into low-power mode
* `will_sleep` and `did_wake` considerations

## Administrivia

<h3 id="reporting_bugs">I have bug or feature request to report</h3>

This will be about how to use the github issues mechanism.

<h3 id="contribute_code">I want to contribute code or bugfixes</h3>

This will be about how to create a pull request, either for mulib or for
mulib-support.
