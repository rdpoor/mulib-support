# mulib posix-tests

This directory contains the files to build and run unit tests and coverage
testing of the [mulib embedded suite](https://github.com/rdpoor/mulib.git)
under Posix-like environments.

## To install

Create and switch to a directory of your choosing:

    $ cd my-project-directory

Download the mulib embedded suite files:

    $ git clone https://github.com/rdpoor/mulib.git

To run the unit tests:

    $ cd mulib-demos/posix-tests
    $ make run-tests

To run the coverage tests:

    $ cd mu_examples/posix_tests
    $ make run-coverage

... then examine the .gcov files in the mu_src directory.

To clean up:

    $ make clean

## Comments?  Questions?

In your favorite browser, visit https://github.com/rdpoor/mulib/issues
and leave your comment or question.
