.PHONY: all clean foo

SOURCES := \
	mu_buf.c \
	mu_bufref.c \
	mu_list.c \
	mu_queue.c

# .h files that lack a corresponding .c fle but are part of the distro
INCLUDES := \
  mu_types.h

OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
HEADERS := $(patsubst %.c, %.h, $(SOURCES))

CFLAGS = -g -I../include -Wall

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
