# Find the repository and configuration directories.
ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TOOLCHAIN_DIR := $(ROOT_DIR)config/

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
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CPPFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CPPFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CPPFLAGS += -D ARM
    endif
endif

# Common defaults, applied after platform configuration.
CFLAGS 	 += -Wall -Wpedantic -std=c++20
CXXFLAGS += -Wall -Wpedantic -std=c++20
MPICXX   ?= mpic++
# Build MPICH-dependent targets only when invoked with ASAX=1.
ASAX ?= 0
# Only needed on macOS, so default to nothing
PTHREAD_BARRIER_TARGET ?=
# Specify what to clean out
JUNK := *.i* *.o* *.exe

# Pattern Rules
%.out: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%_MPI.out: %_MPI.cpp
ifeq ($(UNAME_S),Linux)
	module load openmpi
endif
	$(MPICXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# compiled only when special argument passed
%_MPI2.out: %_MPI2.cpp
ifeq ($(UNAME_S),Linux)
	module load mpich
endif
	$(MPICXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%_PTH.out: %_PTH.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $< -o $@

%_OMP.out: %_OMP.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

# Combo pthreads and OpenMP code
%_PTH_OMP.out: %_PTH_OMP.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)
