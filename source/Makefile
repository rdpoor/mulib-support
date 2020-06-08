.PHONY: all clean

SOURCES := \
	mu_buf.c \
	mu_bufref.c \
	mu_bitvec.c \
	mu_list.c \
	mu_queue.c \
	mu_time.c

# .h files that lack a corresponding .c fle but are part of the distro
INCLUDES := \
  mu_types.h

OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
HEADERS := $(patsubst %.c, %.h, $(SOURCES))

CFLAGS = -g -I../include -I../ports/ubuntu -Wall

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
