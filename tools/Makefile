# As part of the unit testing process, this makefile compiles all of the sources
# in tests/, writing the .o object files into build/, then creates and runs the
# executable build/mu_tests

.PHONY: all clean

BUILD_DIR = ../build
TEST_DRIVER_DIR = ../tests
SOURCE_DIR = ../src
CORE_SOURCE_DIR = ../src/core

TEST_DRIVER_SOURCES := $(wildcard $(TEST_DRIVER_DIR)/*.c)

TEST_DRIVER_INCLUDES := $(wildcard $(TEST_DRIVER_DIR)/*.h)

TEST_DRIVER_OBJECTS = $(patsubst $(TEST_DRIVER_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_DRIVER_SOURCES))

ALL_OBJECTS = $(wildcard $(BUILD_DIR)/*.o)

UNIT_TEST := $(BUILD_DIR)/mu_test

CFLAGS = -Wall -g

IFLAGS = -I $(SOURCE_DIR) -I $(TEST_DRIVER_DIR) -I $(CORE_SOURCE_DIR)

# compile tests/*.c into tests/*.o
$(BUILD_DIR)/%.o : $(TEST_DRIVER_DIR)/%.c $(TEST_DRIVER_INCLUDES)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

# link all .o files into mu_test executable
$(UNIT_TEST) : $(TEST_DRIVER_OBJECTS) core core_tests
	@$(CC) $(CFLAGS) $(IFLAGS) $(ALL_OBJECTS) -o $(UNIT_TEST)

all : $(UNIT_TEST)

# run the unit tests
test : all
	cd $(BUILD_DIR) && $(UNIT_TEST)

clean :
	rm -f $(ALL_OBJECTS) $(UNIT_TEST)

core :
	make -f make_core

core_tests :
	make -f make_core_tests
