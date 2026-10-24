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

histogram_serial.out: histogram_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

histogram_MPI.out: histogram_MPI.cpp
ifeq ($(UNAME_S),Linux)
	module load openmpi
endif
	$(MPICXX) $^ -o $@

histogram_MPI_rev1.out: histogram_MPI_rev1.cpp
ifeq ($(UNAME_S),Linux)
	module load openmpi
endif
	$(MPICXX) $^ -o $@

histogram_MPI_rev2.out: histogram_MPI_rev2.cpp
ifeq ($(UNAME_S),Linux)
	module load openmpi
endif
	$(MPICXX) $^ -o $@

histogram_MPI2.out: histogram_MPI2.cpp
ifeq ($(UNAME_S),Linux)
	module load mpich
endif
	$(MPICXX) $^ -o $@

clean:
	rm -f $(JUNK)