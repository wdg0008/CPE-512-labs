#!/usr/bin/env make

.ONESHELL:

EXES := histogram_serial.out histogram_MPI.out

# only build obsolete C++ bindings when special argument passed
ifeq ($(ASAX),1)
EXES += histogram_MPI2.out
endif

.PHONY: all clean
all: $(EXES)

histogram_serial.out: histogram_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

histogram_MPI.out: histogram_MPI.cpp
ifeq ($(HOST_OS),Linux)
	module load openmpi
endif
	mpic++ $^ -o $@

histogram_MPI2.out: histogram_MPI2.cpp
ifeq ($(HOST_OS),Linux)
	module load mpich
endif
	mpic++ $^ -o $@

clean:
	rm -f *.i* *.o*