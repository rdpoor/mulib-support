# Make and run the mulib unit tests


LIB_DIR := ../../mu_src
LIB_SOURCES = $(wildcard $(LIB_DIR)/*.c)
LIB_OBJECTS = $(patsubst %.c, %.o, $(LIB_SOURCES))

TEST_DIR := ../../mu_tests
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(patsubst %.c, %.o, $(TEST_SOURCES))

PORT_DIR := .
PORT_SOURCES = $(wildcard $(PORT_DIR)/*.c)
PORT_OBJECTS = $(patsubst %.c, %.o, $(PORT_SOURCES))

OBJECTS := $(LIB_OBJECTS) $(TEST_OBJECTS) $(PORT_OBJECTS)

CC     = gcc
# Good settings for debugging
CFLAGS  = -O0 -Wall -g -DMU_LOG_ENABLED

IFLAGS = -I$(LIB_DIR) -I$(TEST_DIR) -I$(PORT_DIR)

%.o : %.c %.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

test_runner:	$(OBJECTS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS) -o test_runner

run_tests: test_runner
	./test_runner

clean:
	rm -f $(OBJECTS) ./test_runner
