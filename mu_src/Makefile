# Makefile for mulib source files.  Used (mostly) for compiling unit tests on
# a unix-like system.

EXCLUDES := _template.c _template.h

# In this src directory, all .c files are compiled into .o files
SOURCES := $(filter-out $(EXCLUDES), $(wildcard *.c))
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
# Global headers are .h files that *don't* have a .c counterpart
GLOBAL_HEADERS := $(filter-out $(patsubst %.c,%.h,$(SOURCES)) $(EXCLUDES), $(wildcard *.h))

# .h files that are not associated with a .c file, but should be copied to the
# .../include release directory.  Note that touching any of these files will
# cause a recompilation of the SOURCES.
INCLUDES :=

CC     = gcc

# Good settings for debugging
CFLAGS  = -O0 -Wall -g -DMU_LOG_ENABLED

IFLAGS = -I../ports/posix

%.o : %.c %.h $(GLOBAL_HEADERS) $(INCLUDES)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

objects: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
