.PHONY: all clean foo

SOURCES := \
  mu_buf.c

# .h files that lack a corresponding .c fle but are part of the distro
INCLUDES := \
  mu_types.h

OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
HEADERS := $(patsubst %.c, %.h, $(SOURCES))

CFLAGS = -I../include

foo:
	echo $(OBJECTS)

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
