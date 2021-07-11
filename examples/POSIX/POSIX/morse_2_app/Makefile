# Makefile for the morse_2_eg example
#
# `make` will compile the example.
# `make run` will build and run the example.
# `make clean` will clean all

.PHONY: all clean mulib_core_objects mulib_extras_objects mu_platform_objects

EXAMPLE_APP = morse_2_app

MULIB_SUPPORT_DIR = ../../../..
MULIB_DIR = $(MULIB_SUPPORT_DIR)/mulib

SRC_MULIB_CORE_DIR = $(MULIB_DIR)/core
SRC_MULIB_EXTRAS_DIR = $(MULIB_DIR)/extras
SRC_MU_PLATFORM_DIR = ../mu_platform
SRC_EXAMPLE_DIR = $(MULIB_SUPPORT_DIR)/examples/shared/morse_2_eg

MULIB_INCLUDE_DIR = $(MULIB_DIR)/include
MULIB_CORE_INCLUDE_DIR = $(MULIB_INCLUDE_DIR)/core
MULIB_EXTRAS_INCLUDE_DIR = $(MULIB_INCLUDE_DIR)/extras
MU_PLATFORM_INCLUDE_DIR = $(MULIB_SUPPORT_DIR)/examples/shared/include

SOURCES_MULIB_CORE := $(wildcard $(SRC_MULIB_CORE_DIR)/*.c)
SOURCES_MULIB_EXTRAS := $(wildcard $(SRC_MULIB_EXTRAS_DIR)/*.c)
SOURCES_MU_PLATFORM := $(wildcard $(SRC_MU_PLATFORM_DIR)/*.c)
SOURCES_EXAMPLE := $(wildcard $(SRC_EXAMPLE_DIR)/*.c)

BUILD_DIR := build
BUILD_MULIB_CORE_DIR := $(BUILD_DIR)/mulib/core
BUILD_MULIB_EXTRAS_DIR := $(BUILD_DIR)/mulib/extras
BUILD_MU_PLATFORM_DIR := $(BUILD_DIR)/mu_platform
BUILD_EXAMPLE_DIR := $(BUILD_DIR)/morse_2_eg

OBJECTS_MULIB_CORE ?= $(patsubst $(SRC_MULIB_CORE_DIR)/%.c, $(BUILD_MULIB_CORE_DIR)/%.o, $(SOURCES_MULIB_CORE))
OBJECTS_MULIB_EXTRAS ?= $(patsubst $(SRC_MULIB_EXTRAS_DIR)/%.c, $(BUILD_MULIB_EXTRAS_DIR)/%.o, $(SOURCES_MULIB_EXTRAS))
OBJECTS_MU_PLATFORM ?= $(patsubst $(SRC_MU_PLATFORM_DIR)/%.c, $(BUILD_MU_PLATFORM_DIR)/%.o, $(SOURCES_MU_PLATFORM))
OBJECTS_EXAMPLE ?= $(patsubst $(SRC_EXAMPLE_DIR)/%.c, $(BUILD_EXAMPLE_DIR)/%.o, $(SOURCES_EXAMPLE))

CFLAGS = -Wall -Werror -g -DMU_LOG_ENABLED

IFLAGS = -I $(MULIB_INCLUDE_DIR) -I $(MULIB_CORE_INCLUDE_DIR) -I $(MULIB_EXTRAS_INCLUDE_DIR) -I $(MU_PLATFORM_INCLUDE_DIR) -I $(SRC_EXAMPLE_DIR) -I $(SRC_MU_PLATFORM_DIR)

all: $(OBJECTS_MULIB_CORE) $(OBJECTS_MULIB_EXTRAS) $(OBJECTS_MU_PLATFORM) $(OBJECTS_EXAMPLE) $(MULIB_DIR)/mulib.o main.o
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS_MULIB_CORE) $(OBJECTS_MULIB_EXTRAS) $(OBJECTS_MU_PLATFORM) $(OBJECTS_EXAMPLE) $(MULIB_DIR)/mulib.o main.o  -o $(EXAMPLE_APP)

run: all
	./$(EXAMPLE_APP)

clean:
	rm -rf $(BUILD_DIR)
	rm -f main.o
	rm -f $(EXAMPLE_APP)

.c.o:
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MULIB_CORE_DIR)/%.o : $(SRC_MULIB_CORE_DIR)/%.c $(SRC_MULIB_CORE_DIR)/*.h $(SRC_MU_PLATFORM_DIR)/mu_config.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MULIB_EXTRAS_DIR)/%.o: $(SRC_MULIB_EXTRAS_DIR)/%.c $(SRC_MULIB_EXTRAS_DIR)/*.h $(SRC_MU_PLATFORM_DIR)/mu_config.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MU_PLATFORM_DIR)/%.o: $(SRC_MU_PLATFORM_DIR)/%.c $(SRC_MU_PLATFORM_DIR)/mu_config.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_EXAMPLE_DIR)/%.o: $(SRC_EXAMPLE_DIR)/%.c $(SRC_EXAMPLE_DIR)/*.h $(SRC_MU_PLATFORM_DIR)/mu_config.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@
