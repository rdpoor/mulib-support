# Makefile for mulib unit tests for unit-like systems.
# Usage:
#   $ make run_tests
#   $ make clean

SOURCE_DIR = ./src
TEST_DIR = ./tests
EXCLUDE_SOURCES = $(TEST_DIR)/_template_test.c $(SOURCE_DIR)/_template.c

ALL_SOURCES := $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(TEST_DIR)/*.c)
SOURCES := $(filter-out $(EXCLUDE_SOURCES), $(ALL_SOURCES))
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
CC = gcc

src_objects :
	cd $(SOURCE_DIR) && make objects

test_objects :
	cd $(TEST_DIR) && make objects

unit_tests: src_objects test_objects
	$(CC) -o main $(OBJECTS) main.c

run_tests: unit_tests
	./main

clean:
	cd $(SOURCE_DIR) && make clean
	cd $(TEST_DIR) && make clean
	rm -rf main
