CXX = g++
CXXFLAGS = -Wall -Wpedantic -std=c++20 -pthread

EXES := heat_1d_serial.out heat_1d_PTH1.out heat_1d_PTH2.out heat_1d_PTH3.out heat_1d_PTH4.out heat_1d_PTH5.out

all: $(EXES)

# pattern matching rule to build executables from .cpp files
# $@ is the target name
# $< is the first prerequisite

%.out: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(EXES) *.txt *.i* *.o*