# Makefile for the tower_eg example
#
# `make` will compile the example.
# `make run` will build and run the example.
# `make clean` will clean all

.PHONY: all clean mulib_core_objects mulib_extras_objects mu_platform_objects tower_eg_objects

TOWER_EG = tower_app

SRC_MULIB_DIR = ../../../../mulib
SRC_MULIB_CORE_DIR = $(SRC_MULIB_DIR)/core
SRC_MULIB_EXTRAS_DIR = $(SRC_MULIB_DIR)/extras
SRC_MU_PLATFORM_DIR = ../mu_platform
SRC_TOWER_EG_DIR = ../../../shared/tower_eg

SOURCES_MULIB_CORE := $(wildcard $(SRC_MULIB_CORE_DIR)/*.c)
SOURCES_MULIB_EXTRAS := $(wildcard $(SRC_MULIB_EXTRAS_DIR)/*.c)
SOURCES_MU_PLATFORM := $(wildcard $(SRC_MU_PLATFORM_DIR)/*.c)
SOURCES_TOWER_EG := $(wildcard $(SRC_TOWER_EG_DIR)/*.c)

BUILD_DIR := build
BUILD_MULIB_CORE_DIR := $(BUILD_DIR)/mulib/core
BUILD_MULIB_EXTRAS_DIR := $(BUILD_DIR)/mulib/extras
BUILD_MU_PLATFORM_DIR := $(BUILD_DIR)/mu_platform
BUILD_TOWER_EG_DIR := $(BUILD_DIR)/tower_eg

OBJECTS_MULIB_CORE ?= $(patsubst $(SRC_MULIB_CORE_DIR)/%.c, $(BUILD_MULIB_CORE_DIR)/%.o, $(SOURCES_MULIB_CORE))
OBJECTS_MULIB_EXTRAS ?= $(patsubst $(SRC_MULIB_EXTRAS_DIR)/%.c, $(BUILD_MULIB_EXTRAS_DIR)/%.o, $(SOURCES_MULIB_EXTRAS))
OBJECTS_MU_PLATFORM ?= $(patsubst $(SRC_MU_PLATFORM_DIR)/%.c, $(BUILD_MU_PLATFORM_DIR)/%.o, $(SOURCES_MU_PLATFORM))
OBJECTS_TOWER_EG ?= $(patsubst $(SRC_TOWER_EG_DIR)/%.c, $(BUILD_TOWER_EG_DIR)/%.o, $(SOURCES_TOWER_EG))


CFLAGS = -Wall -Werror -g -DMU_LOG_ENABLED

IFLAGS = -I $(SRC_MULIB_DIR) -I $(SRC_MULIB_EXTRAS_DIR) -I $(SRC_MU_PLATFORM_DIR) -I $(SRC_TOWER_EG_DIR)

all: $(OBJECTS_MULIB_CORE) $(OBJECTS_MULIB_EXTRAS) $(OBJECTS_MU_PLATFORM) $(OBJECTS_TOWER_EG) $(SRC_MULIB_DIR)/mulib.o main.o
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS_MULIB_CORE) $(OBJECTS_MULIB_EXTRAS) $(OBJECTS_MU_PLATFORM) $(OBJECTS_TOWER_EG) $(SRC_MULIB_DIR)/mulib.o main.o -o $(TOWER_EG)

run: all
	./$(TOWER_EG)

clean:
	rm -rf $(BUILD_DIR)
	rm main.o

$(BUILD_MULIB_CORE_DIR)/%.o : $(SRC_MULIB_CORE_DIR)/%.c $(SRC_MULIB_CORE_DIR)/*.h $(SRC_MU_PLATFORM_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MULIB_EXTRAS_DIR)/%.o: $(SRC_MULIB_EXTRAS_DIR)/%.c $(SRC_MULIB_EXTRAS_DIR)/*.h $(SRC_MU_PLATFORM_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MU_PLATFORM_DIR)/%.o: $(SRC_MU_PLATFORM_DIR)/%.c $(SRC_MU_PLATFORM_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_TOWER_EG_DIR)/%.o: $(SRC_TOWER_EG_DIR)/%.c $(SRC_TOWER_EG_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

main.o: main.c
		$(CC) $(CFLAGS) $(IFLAGS) -c main.c -o main.o
