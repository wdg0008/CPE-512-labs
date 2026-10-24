#!/usr/bin/env make

include ../toolchains.mk

.ONESHELL:

.PHONY: all clean

EXES := add_num_MPI.out add_num_rec_level_PTH1.out \
		add_num_rec_level_serial.out add_num_recursive_serial.out \
		add_num_serial.out

ifeq ($(ASAX),1)
EXES += add_num_MPI2.out
endif

all: $(EXES)

add_num_MPI.out: add_num_MPI.cpp
ifeq ($(UNAME_S),Linux)
	module load openmpi
endif
	$(MPICXX) $< -o $@

add_num_MPI2.out: add_num_MPI2.cpp
ifeq ($(UNAME_S),Linux)
	module load mpich
endif
	$(MPICXX) $< -o $@

add_num_rec_level_PTH1.out: add_num_rec_level_PTH1.cpp
	$(CXX) $(CXXFLAGS) -pthread $< -o $@

add_num_rec_level_serial.out: add_num_rec_level_serial.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

add_num_recursive_serial.out: add_num_recursive_serial.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

add_num_serial.out: add_num_serial.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.i* *.o*