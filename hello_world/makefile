CXX = g++
CXXFLAGS = -Wall -Wpedantic -std=c++20 -pthread

EXES := hello_world_MPI hello_world_MPI2 hello_world_OMP hello_world_PTH

all: $(EXES)

# pattern matching rule to build executables from .cpp files
# $@ is the target name
# $< is the first prerequisite
$(EXES): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

hello_world_MPI: hello_world_MPI.cpp
	mpic++ 

.PHONY: clean
clean:
	rm -f $(EXES) *.txt *.i* *.o*