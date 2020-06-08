.PHONY: all clean foo

SOURCES := \
  port_time.c

OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
HEADERS := $(patsubst %.c, %.h, $(SOURCES))

CFLAGS = -g -Wall

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
