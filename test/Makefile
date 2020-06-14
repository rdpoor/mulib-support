MU_ROOT ?= ..

TEST_SOURCES := \
	mu_bitvec_test.c \
	mu_buf_test.c \
	mu_bufref_test.c \
	mu_list_test.c \
	mu_pstore_test.c \
	mu_queue_test.c \
	mu_sched_test.c \
	mu_spscq_test.c \
	mu_str_test.c \
	mu_substr_test.c \
	mu_task_test.c \
	mu_time_test.c \
	mu_test_utils.c \
	mu_test.c

TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SOURCES))
MU_OBJS := $(wildcard ../source/*.o)
PORT_OBJS := $(wildcard ../ports/ubuntu/*.o)

IFLAGS = -I../include -I../test -I../ports/ubuntu
CFLAGS = -g -Wall

%.o : %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

mu_test: $(TEST_OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(TEST_OBJS) $(MU_OBJS) $(PORT_OBJS) -o mu_test

test: mu_test
	./mu_test

clean:
	rm -f $(TEST_OBJS)
	rm -f mu_test
