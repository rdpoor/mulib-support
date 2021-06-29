# Main makefile for mulib unit testing:
#
# `make test`:
#   Invokes `make_mulib` to compile the mulib object files
#   Invokes `make_mulib_test` to compille the mulib unit test object files
#   Links all the objects into the ../build/mu_test executable
#   Runs ../build/mu_test

.PHONY: all clean mulib_objects mulib_test_objects

BUILD_DIR := ../build
BUILD_CORE_DIR := $(BUILD_DIR)/core
BUILD_EXTRAS_DIR := $(BUILD_DIR)/extras
BUILD_PLATFORM_DIR := $(BUILD_DIR)/platform

# NOTE: use ?= rather than := for late binding: the object files don't exist
# until make_mulib and make_mulib_test have run.
CORE_OBJECTS ?= $(wildcard $(BUILD_CORE_DIR)/*.o)
EXTRAS_OBJECTS ?= $(wildcard $(BUILD_EXTRAS_DIR)/*.o)
PLATFORM_OBJECTS ?= $(wildcard $(BUILD_PLATFORM_DIR)/*.o)
TEST_OBJECTS ?= $(wildcard $(BUILD_DIR)/*.o)
ALL_OBJECTS ?= $(CORE_OBJECTS) $(EXTRAS_OBJECTS) $(PLATFORM_OBJECTS) $(TEST_OBJECTS)

UNIT_TEST := $(BUILD_DIR)/mu_test

all : mulib_objects mulib_test_objects
	$(CC) $(CFLAGS) $(IFLAGS) $(ALL_OBJECTS) -o $(UNIT_TEST)

test : all
	cd $(BUILD_DIR) && $(UNIT_TEST)

clean :
	rm -rf $(BUILD_DIR)

mulib_objects :
	make -f make_mulib

mulib_test_objects :
	make -f make_mulib_test



