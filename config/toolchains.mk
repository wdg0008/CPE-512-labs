# Find the configuration directory.
TOOLCHAIN_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
# The repository root is the parent of the configuration directory.
ROOT_DIR := $(abspath $(TOOLCHAIN_DIR)..)/

# Detect the host operating system, unless the caller supplied one.
# From https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS),Windows_NT)
	CPPFLAGS += -D WIN32
	include $(TOOLCHAIN_DIR)msys2.mk
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		CPPFLAGS += -D AMD64
	else ifeq ($(PROCESSOR_ARCHITECTURE),ARM64)
		CPPFLAGS += -DARM -DARM64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			CPPFLAGS += -D AMD64
		else ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			CPPFLAGS += -D IA32
        else ifeq ($(PROCESSOR_ARCHITECTURE),ARM64)
			CPPFLAGS += -DARM -DARM64
		else ifeq ($(PROCESSOR_ARCHITECTURE),ARM)
			CPPFLAGS += -DARM
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		include $(TOOLCHAIN_DIR)linux.mk
		CPPFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		include $(TOOLCHAIN_DIR)macos.mk
		CPPFLAGS += -D OSX
	endif
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M),x86_64)
		CPPFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_M)),)
		CPPFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_M)),)
		CPPFLAGS += -D ARM
	endif
endif

# Common defaults, applied after platform configuration.
CFLAGS 	 += -Wall -Wpedantic -std=c17
CXXFLAGS += -Wall -Wpedantic -std=c++20
MPICXX   ?= mpic++
# Build MPICH-dependent targets only when invoked with ASAX=1.
ASAX ?= 0
# Only needed on macOS, so default to nothing
PTHREAD_BARRIER_TARGET ?=
# Specify what to clean out
JUNK := *.i* *.o* *.exe *.obj

# Pattern Rules
# ORDER MATTERS ON MACOS BSD-STYLE MAKE!!!!!
# Place general rules at the bottom, and more specific rules at the top.

# C-binding MPI code using OpenMPI
%_MPI.out: %_MPI.cpp
ifeq ($(ASAX),1)
	module load openmpi
endif
	$(MPICXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# Obsoloete C++ bindings from old mpich versions (only compile on ASAX=1)
%_MPI2.out: %_MPI2.cpp
ifeq ($(ASAX),1)
	module load mpich
endif
	$(MPICXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# Combo pthreads and OpenMP code
%_PTH_OMP.out: %_PTH_OMP.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

# Compile pthreads code (need special changes for macOS barriers, see. ./1D_heat_transfer/makefile)
%_PTH.out: %_PTH.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $< -o $@

# OpenMP code
%_OMP.out: %_OMP.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

# Serial versions of algorithms (catch as many as possible before rule for all other .cpp files)
%serial.out: %serial.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# Regular C++ code
%.out: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
