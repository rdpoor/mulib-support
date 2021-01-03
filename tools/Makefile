# As part of the unit testing process, this makefile compiles all of the sources
# in tests/, writing the .o object files into build/, then creates and runs the
# executable build/mu_tests

.PHONY: all clean

BUILD_DIR = ../build
PLATFORM_BUILD_DIR = ../build/platform
TEST_DRIVER_DIR = ../src
SOURCE_DIR = ../test/mulib
CORE_SOURCE_DIR = ../test/mulib/core
PLATFORM_SOURCE_DIR = ../test/platform

TEST_DRIVER_SOURCES := $(wildcard $(TEST_DRIVER_DIR)/*.c)
TEST_DRIVER_INCLUDES := $(wildcard $(TEST_DRIVER_DIR)/*.h)
TEST_DRIVER_OBJECTS = $(patsubst $(TEST_DRIVER_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_DRIVER_SOURCES))

ALL_OBJECTS = $(wildcard $(BUILD_DIR)/*.o)
PLATFORM_OBJECTS = $(wildcard $(PLATFORM_BUILD_DIR)/*.o)


UNIT_TEST := $(BUILD_DIR)/mu_test

CFLAGS = -Wall -g -DMU_LOG_ENABLED

IFLAGS = -I $(SOURCE_DIR) -I $(PLATFORM_SOURCE_DIR) -I $(TEST_DRIVER_DIR)

DIRS = ../build ../build/platform
$(shell mkdir -p $(DIRS))

# build ../src/*.o
$(BUILD_DIR)/%.o : $(TEST_DRIVER_DIR)/%.c $(TEST_DRIVER_INCLUDES)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

# link all .o files into mu_test executable
$(UNIT_TEST) : $(ALL_OBJECTS) $(PLATFORM_OBJECTS) $(TEST_DRIVER_OBJECTS)

all : 
	make -f make_core
	make -f make_core_tests
	make $(UNIT_TEST)

# run the unit tests
test : all
	cd $(BUILD_DIR) && $(UNIT_TEST)

clean :
	rm -f $(ALL_OBJECTS) $(PLATFORM_OBJECTS) $(UNIT_TEST)

core :
	make -f make_core

core_tests :
	make -f make_core_tests


