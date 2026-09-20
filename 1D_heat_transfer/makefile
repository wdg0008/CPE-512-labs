CXX = g++
CXXFLAGS = -Wall -Wpedantic -std=c++20 -pthread

EXES := heat_1d_serial heat_1d_PTH1 heat_1d_PTH2 heat_1d_PTH3 heat_1d_PTH4 heat_1d_PTH5

all: $(EXES)

# pattern matching rule to build executables from .cpp files
# $@ is the target name
# $< is the first prerequisite
$(EXES): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(EXES) *.txt *.i* *.o*