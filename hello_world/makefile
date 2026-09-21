CXX = g++
CXXFLAGS = -Wall -Wpedantic -std=c++20
# SHELL := /bin/bash

# Special directive so all recipe lines for one target run in the same shell
# preserves environment modifications from loading modules within a target
.ONESHELL:

EXES := hello_world.out hello_world_MPI.out hello_world_MPI2.out \
		hello_world_OMP.out hello_world_PTH.out hello_world_PTH_OMP.out \
		prime.out

# compile everything by default
all: $(EXES)

hello_world.out: hello_world.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

hello_world_MPI.out: hello_world_MPI.cpp
	module load openmpi
	mpic++ $< -o $@

hello_world_MPI2.out: hello_world_MPI2.cpp
	module load mpich
	mpic++ $< -o $@

hello_world_OMP.out: hello_world_OMP.cpp
	$(CXX) $(CXXFLAGS) -fopenmp $< -o $@

hello_world_PTH.out: hello_world_PTH.cpp
	$(CXX) $(CXXFLAGS) -pthread $< -o $@

hello_world_PTH_OMP.out: hello_world_PTH_OMP.cpp
	$(CXX) -pthread -fopenmp $< -o $@

prime.out: prime.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.i* *.o*