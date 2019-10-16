# Makefile for mulib unit tests for posix-like systems.
# Usage:
#   $ make run_tests
#   $ make clean

SOURCE_DIR = ./src
TEST_DIR = ./tests
PORT_DIR = ./ports/posix
EXCLUDE_SOURCES = $(TEST_DIR)/_template_test.c $(SOURCE_DIR)/_template.c $(PORT_DIR)/_template.c

ALL_SOURCES := $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(TEST_DIR)/*.c) $(wildcard $(PORT_DIR)/*.c)
SOURCES := $(filter-out $(EXCLUDE_SOURCES), $(ALL_SOURCES))
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
CC = gcc
# Good settings for debugging
CFLAGS  = -O0 -Wall -g -DMU_LOG_ENABLED
IFLAGS =

src_objects :
	cd $(SOURCE_DIR) && make objects

test_objects :
	cd $(TEST_DIR) && make objects

port_objects :
	cd $(PORT_DIR) && make objects

unit_tests: src_objects test_objects port_objects
	$(CC) $(CFLAGS) $(IFLAGS) -o main $(OBJECTS) main.c

run_tests: unit_tests
	./main

clean:
	cd $(SOURCE_DIR) && make clean
	cd $(TEST_DIR) && make clean
	cd $(PORT_DIR) && make clean
	rm -rf main
