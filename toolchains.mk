# Find the repository and configuration directories.
ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TOOLCHAIN_DIR := $(ROOT_DIR)config/

# Detect the host operating system, unless the caller supplied one.
ifeq ($(OS),Windows_NT)
HOST_OS ?= MINGW64_NT
else
HOST_OS ?= $(shell uname -s)
endif

# Load the appropriate toolchain configuration.
ifeq ($(HOST_OS),Darwin)
include $(TOOLCHAIN_DIR)macos.mk
else ifeq ($(HOST_OS),Linux)
include $(TOOLCHAIN_DIR)linux.mk
else ifneq ($(filter MINGW64_NT%,$(HOST_OS)),)
include $(TOOLCHAIN_DIR)msys2.mk
else
$(error Unsupported operating system: $(HOST_OS))
endif

# Common defaults, applied after platform configuration.
CCFLAGS  ?= -Wall -Wpedantic -std=c17
CXXFLAGS ?= -Wall -Wpedantic -std=c++20
CPPFLAGS ?=
LDFLAGS  ?=
LDLIBS   ?=
MPICXX ?= mpic++
# Build MPICH-dependent targets only when invoked with ASAX=1.
ASAX ?= 0
# Only needed on macOS, so default to nothing
PTHREAD_BARRIER_TARGET ?=