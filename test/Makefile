MU_ROOT ?= ..

SOURCES := \
  mu_test_utils.c \
  mu_test.c \
	mu_buf_test.c \
	mu_bufref_test.c \
	mu_queue_test.c

OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
LIBOBJS := $(wildcard ../source/*.o)

IFLAGS = -I../include -I../test
CFLAGS = -g -Wall

%.o : %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

mu_test: $(OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS) $(LIBOBJS) -o mu_test

test: mu_test
	./mu_test

clean:
	rm -f $(OBJECTS)
	rm -f mu_test
