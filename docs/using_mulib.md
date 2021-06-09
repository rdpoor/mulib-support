# `mulib` Quick Start

<h2 id="mulib_organization">I want to understand how mulib is organized.</h2>

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
** <IDE_NAME>/<DEV_BOARD_NAME>/<demo_application_name>/ contains a complete,
runnable project for a specific IDE and specific embedded processor development
board.  For example:
** STM32CubeIDE/NUCLEO-G431RB/morse_2_app contains a project file that runs the
morse-2 example for the ST Microsystems IDE on a NUCLEO-G431RB development board.
** shared/ contains common source code among all of the example projects.
* laboratory/ is a place where code languishes until the Powers That Be decide
if it should be promoted to the mulib or mulib-support/examples or simply
deleted.  Think of it as the developer's playpen.
* mulib-test contains unit tests for all the modules in the mulib repository.

<h2 id="browse_mulib">I want to browse some code examples that use `mulib`</h2>

The easiest way to get a sense of mulib's style and capabilities is to look at
some of the code examples.  You can find the common (non-platform specific)
code examples in [the mulib-support examples directory]
(https://github.com/rdpoor/mulib-support/tree/master/demos/shared).  

If you want to view the API for individual mulib modules, you can find them in
the [mulib core repository](https://github.com/rdpoor/mulib/tree/main/core).

<h2 id="download_mulib">I want to download `mulib`</h2>
Here are the instructions for downloading mulib.

<h2 id="run_examples">I want to run some `mulib` code examples</h2>
Here are the instructions for running the mulib code examples.
