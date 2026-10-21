#!/usr/bin/env make
# shebang allows direct execution with chmod +x (very cursed)

# Specify target when none is provided
.DEFAULT_GOAL := all

# Find the directory containing this Makefile.
ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
# MK_DIR   := $(ROOT_DIR)mk/

# Detect the host operating system.
HOST_OS := $(shell uname -s)

# Load the appropriate toolchain configuration.
ifeq ($(HOST_OS),Darwin)
include $(ROOT_DIR)macos.mk
# conditionally define pthread barrier library for macOS as a dependency of all
PTHREAD_BARRIER_TARGET := $(PTHBAR_LIB)
else ifeq ($(HOST_OS),Linux)
include $(ROOT_DIR)linux.mk
else ifneq ($(filter MINGW64_NT%,$(HOST_OS)),)
include $(ROOT_DIR)msys2.mk
else
	$(error Unsupported operating system: $(HOST_OS))
endif

# Common defaults, applied after platform configuration.
CXXFLAGS ?= -Wall -Wpedantic -std=c++20
CPPFLAGS ?=
LDFLAGS  ?=
LDLIBS   ?=
# Temporary placeholder until this is used a lot more in the course.
MPICXX ?= mpic++
# Build MPICH-dependent targets only when invoked with ASAX=1.
ASAX ?= 0

# Export configuration to child Make processes.
export CXX CPPFLAGS CXXFLAGS LDFLAGS LDLIBS
export OMP_CXXFLAGS OMP_LDFLAGS OMP_LIBS
export PTHREAD_CPPFLAGS PTHREAD_CXXFLAGS PTHREAD_LDFLAGS PTHREAD_LIBS
export MPICXX
export HOST_OS
export ASAX

# Subdirectories containing course units.
UNITS := 1D_heat_transfer 2D_heat_transfer hello_world tsp_example histogram

# each of these are phony targets, so make will always run the recipe
.PHONY: all clean $(UNITS)

# Run make recursively in each unit's subdirectory.
# barrier target only needed on macOS and resolves to nothing otherwise
all: $(PTHREAD_BARRIER_TARGET) $(UNITS)

# Separate targets for each unit, so they can be built independently.
# Run the 'all' target in a unit's subdirectory.
$(UNITS): $(PTHREAD_BARRIER_TARGET)
	+$(MAKE) -C $@ all

# Run the 'clean' target in each unit's subdirectory.
# The extra $ is needed to pass arguments to the shell invoked by make.
clean:
	@for unit in $(UNITS); do \
		$(MAKE) -C "$$unit" clean || exit $$?; \
	done
