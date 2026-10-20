#!/usr/bin/make -f

# Special directive so all recipe lines for one target run in the same shell
# preserves environment modifications from loading modules within a target
.ONESHELL:


EXES := hello_world.out hello_world_OMP.out hello_world_PTH.out \
		hello_world_PTH_OMP.out prime.out hello_world_MPI.out

# compile everything by default
all: $(EXES)

hello_world.out: hello_world.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

hello_world_MPI.out: hello_world_MPI.cpp
ifeq ($(HOST_OS),Linux)
	module load openmpi
endif
	mpic++ $< -o $@

hello_world_OMP.out: hello_world_OMP.cpp
	$(CXX) $(CXXFLAGS) $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

hello_world_PTH.out: hello_world_PTH.cpp
	$(CXX) $(CXXFLAGS) -pthread $< -o $@

hello_world_PTH_OMP.out: hello_world_PTH_OMP.cpp
	$(CXX) $(CXXFLAGS) -pthread $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

prime.out: prime.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.i* *.o*