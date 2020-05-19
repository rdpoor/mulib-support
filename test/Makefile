MU_ROOT ?= ..

SOURCES := \
  mu_buf_test.c \
	mu_test_utils.c \
	mu_test.c

OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
LIBOBJS := $()

IFLAGS = -I../include -I../test

%.o : %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

mu_test: $(OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS) $(LIBOBJS) -o mu_test

test: mu_test
	./mu_test

clean:
	rm -f $(OBJECTS)
