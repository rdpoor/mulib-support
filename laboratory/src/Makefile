# Makefile for the oblique_eg example
#
# `make` will compile the example.
# `make run` will build and run the example.
# `make clean` will clean all

.PHONY: all clean mulib_core_objects mulib_extras_objects mu_platform_objects oblique_eg_objects

LAB_APP := lab_app
ENCRYPTOR_APP := encryptor_app

SRC_MULIB_SUPPORT_DIR = ../..
SRC_MULIB_DIR = $(SRC_MULIB_SUPPORT_DIR)/mulib
SRC_MULIB_CORE_DIR = $(SRC_MULIB_DIR)/core
SRC_MULIB_EXTRAS_DIR = $(SRC_MULIB_DIR)/extras
SRC_MU_PLATFORM_DIR = $(SRC_MULIB_SUPPORT_DIR)/examples/POSIX/POSIX/mu_platform
SRC_LAB_DIR = ../src

SOURCES_MULIB_CORE := $(wildcard $(SRC_MULIB_CORE_DIR)/*.c)
SOURCES_MULIB_EXTRAS := $(wildcard $(SRC_MULIB_EXTRAS_DIR)/*.c)
SOURCES_MU_PLATFORM := $(wildcard $(SRC_MU_PLATFORM_DIR)/*.c)
SOURCES_LAB := $(wildcard $(SRC_LAB_DIR)/*.c)

BUILD_DIR := build
BUILD_MULIB_CORE_DIR := $(BUILD_DIR)/mulib/core
BUILD_MULIB_EXTRAS_DIR := $(BUILD_DIR)/mulib/extras
BUILD_MU_PLATFORM_DIR := $(BUILD_DIR)/mu_platform
BUILDLAB_DIR := $(BUILD_DIR)/lab

OBJECTS_MULIB_CORE ?= $(patsubst $(SRC_MULIB_CORE_DIR)/%.c, $(BUILD_MULIB_CORE_DIR)/%.o, $(SOURCES_MULIB_CORE))
OBJECTS_MULIB_EXTRAS ?= $(patsubst $(SRC_MULIB_EXTRAS_DIR)/%.c, $(BUILD_MULIB_EXTRAS_DIR)/%.o, $(SOURCES_MULIB_EXTRAS))
OBJECTS_MU_PLATFORM ?= $(patsubst $(SRC_MU_PLATFORM_DIR)/%.c, $(BUILD_MU_PLATFORM_DIR)/%.o, $(SOURCES_MU_PLATFORM))
OBJECTS_LAB ?= $(patsubst $(SRC_LAB_DIR)/%.c, $(BUILDLAB_DIR)/%.o, $(SOURCES_LAB))

CFLAGS = -Wall -Werror -g -DMU_LOG_ENABLED

IFLAGS = -I $(SRC_MULIB_DIR) -I $(SRC_MU_PLATFORM_DIR) -I $(SRC_LAB_DIR) -I $(SRC_MULIB_EXTRAS_DIR)

 $(info    SOURCES_LAB is $(SOURCES_LAB))
 $(info    OBJECTS_LAB is $(OBJECTS_LAB))
 


encryptor: $(OBJECTS_MULIB_CORE) $(OBJECTS_MULIB_EXTRAS) $(OBJECTS_MU_PLATFORM) $(OBJECTS_LAB) 
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS_MULIB_CORE) $(OBJECTS_MULIB_EXTRAS) $(OBJECTS_MU_PLATFORM) $(OBJECTS_LAB) -o $(ENCRYPTOR_APP)

clean:
	rm -rf $(BUILD_DIR)


.c.o:
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MULIB_CORE_DIR)/%.o : $(SRC_MULIB_CORE_DIR)/%.c $(SRC_MULIB_CORE_DIR)/*.h $(SRC_MU_PLATFORM_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MULIB_EXTRAS_DIR)/%.o: $(SRC_MULIB_EXTRAS_DIR)/%.c $(SRC_MULIB_EXTRAS_DIR)/*.h $(SRC_MU_PLATFORM_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILD_MU_PLATFORM_DIR)/%.o: $(SRC_MU_PLATFORM_DIR)/%.c $(SRC_MU_PLATFORM_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@

$(BUILDLAB_DIR)/%.o: $(SRC_LAB_DIR)/%.c $(SRC_LAB_DIR)/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(<) -o $@
