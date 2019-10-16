# Makefile for mulib source files.  Used (mostly) for compiling unit tests on
# a unix-like system.

EXCLUDES := _template_test.c

# In this src directory, all .c files are compiled into .o files
SOURCES := $(filter-out $(EXCLUDES), $(wildcard *.c))
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
HEADERS := $(filter-out $(EXCLUDES), $(wildcard *.h))

# .h files that are not associated with a .c file, but should be copied to the
# .../include release directory.  Note that touching any of these files will
# cause a recompilation of the SOURCES.
INCLUDES :=

CC     = gcc

# Good settings for debugging
CFLAGS  = -O0 -Wall -g -DMU_LOG_ENABLED

IFLAGS = -I../ports/posix

woof:
	echo $(HEADERS)

# In this unit test directory, individual unit test files don't have .h headers;
# only the files for the unit test framework do.
%.o : %.c $(HEADERS) $(INCLUDES)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

objects: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
