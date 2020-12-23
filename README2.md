# Welcome to <tt>mulib</tt>
<tt>mulib</tt> is curated collection of software modules written in C from the
ground up for microcontrollers and other resource-constrained environments,
designed to make writing embedded applications simpler, faster and more reliable.

Some of the features you'll find in <tt>mulib</tt>:

* a fast and lithe run-to-completion scheduler
* low-overhead timing and event handling
* advanced in-place string handling
* an in-memory publish / subscribe system
* flexible logging
* ... and a variety of other useful modules

## <tt>mulib</tt> is a more of a framework and less of an operating system
<tt>mulib</tt> does not call itself an "operating system", specifically because
it doesn't try to be one.  Like an operating system, <tt>mulib</tt> does have a
scheduler that can control when tasks are run, but unlike an operating system,
<tt>mulib</tt> does not insist on taking over your microcontroller: your code
remains in control.  

## <tt>mulib</tt> has a clear design philosophy

<tt>mulib</tt> keeps the following design goals in mind (even if some of the
goals are more aspirational than factual):

* **Self-contained, pure C code:** With minimal dependencies on external
  libraries, <tt>mulib</tt> "just works" with almost any modern C compiler.
* **No malloc. Ever**: All of mulib's modules support user-allocated data
  structures.  Of course, if you want to use malloc and free for the data
  structures, you're free to do so.
* **Super portable:** One concise mu_vms.c file defines mulib's interface
  to your target system's environment.
* **Fast and Dangerous:** <tt>mulib</tt> is written for embedded applications,
  so it favors minimizing time and code space over argument validation.
* **Two-file implementation:** Whenever practical, each module is embodied in
  one header file and one C file, allowing you to cherry pick the functions you
  want without bringing in all of <tt>mulib</tt>.
* **Well-documented:** API documentation is driven by documentation strings
  in the header files.
* **Well-tested:** Comprehensive unit tests validate the core API.
* **Low threshold, high ceiling:** A suite of example programs take you from the
  simplest "blink an LED" example to asynchronous multi-tasking applications.
* **Yours to use:** The entirety of <tt>mulib</tt> is covered under the MIT
  open source license.

## Getting started with <tt>mulib</tt>

* Study a well-spelled out example
* Browse the modules' APIs
* Clone the mulib repository and run an example
* Extend <tt>mulib</tt> for a specific IDE and processor
* Report a bug, suggest an enhancement, issue a pull request
