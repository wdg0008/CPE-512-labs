#!/usr/bin/env make

include ../toolchains.mk

.ONESHELL:

EXES := histogram_serial.out histogram_MPI.out \
		histogram_MPI_rev1.out histogram_MPI_rev2.out

# only build obsolete C++ bindings when special argument passed
ifeq ($(ASAX),1)
EXES += histogram_MPI2.out
endif

.PHONY: all clean
all: $(EXES)

# static rule to generate MPI revision targets
REV_SOURCES := $(wildcard *MPI_rev[1-9].cpp)
REV_TARGETS := $(REV_SOURCES:.cpp=.out)

$(REV_TARGETS): %.out: %.cpp
ifeq ($(UNAME_S),Linux)
	module load openmpi
endif
	$(MPICXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(JUNK)
