# Makefile for mulib unit tests for posix-like systems.
# Usage:
#   $ make run-tests
#   $ make clean

vpath %.h .
MULIB_DIR = ../../mulib

SUITE_DIR = $(MULIB_DIR)/mu_src
SUITE_SRCS = $(wildcard $(SUITE_DIR)/*.c)
SUITE_OBJS := $(patsubst %.c,%.o,$(SUITE_SRCS))

TESTS_DIR = $(MULIB_DIR)/mu_tests
TESTS_SRCS = $(wildcard $(TESTS_DIR)/*.c)
TESTS_OBJS := $(patsubst %.c,%.o,$(TESTS_SRCS))

PORT_DIR = .
PORT_SRCS = mu_port.c
PORT_OBJS := $(patsubst %.c,%.o,$(PORT_SRCS))

CC = gcc
CFLAGS  = -Wall -g -DMU_LOG_ENABLED
IFLAGS  = -I$(SUITE_DIR) -I$(PORT_DIR) -I$(TESTS_DIR)

run-tests: unit_tests
	./unit_tests

unit_tests: $(PORT_OBJS) $(SUITE_OBJS) $(TESTS_OBJS) unit_tests.o
	$(CC) $(CFLAGS) -o unit_tests $(PORT_OBJS) $(SUITE_OBJS) $(TESTS_OBJS) unit_tests.o

$(TESTS_OBJS): %.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(PORT_OBJS): %.o: %.c %.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(SUITE_OBJS): %.o: %.c %.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

unit_tests.o: %.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

clean:
	rm -f $(SUITE_OBJS) $(TESTS_OBJS) $(PORT_OBJS) ./unit_tests unit_tests.o
