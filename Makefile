# Makefile for mulib unit tests for unit-like systems.
# Usage:
#   $ make run_tests

SRCDIR = ./src
TESTDIR = ./tests
EXCLUDES = $(TESTDIR)/_template_test.c $(SRCDIR)/_template.c

ALL_SOURCES := $(wildcard $(SRCDIR)/*.c) $(wildcard $(TESTDIR)/*.c)
SOURCES := $(filter-out $(EXCLUDES), $(ALL_SOURCES))
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
CC = gcc

src_objects :
	cd $(SRCDIR) && make objects

test_objects :
	cd $(TESTDIR) && make objects

unit_tests: src_objects test_objects
	$(CC) -o main $(OBJECTS) main.c

run_tests: unit_tests
	./main

clean:
	cd $(SRCDIR) && make clean
	cd $(TESTDIR) && make clean
	rm -rf main
