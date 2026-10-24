#!/usr/bin/env make

include ../toolchains.mk

CXXFLAGS += -O3 -ffast-math
# add -march=native if you are brave and want optimizations

.PHONY: all clean

all: tsp_serial.out tsp_PTH1.out tsp_PTH2.out tsp_OMP.out

tsp_serial.out: tsp_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

tsp_PTH1.out: tsp_PTH1.cpp
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@

tsp_PTH2.out: tsp_PTH2.cpp
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@

tsp_OMP.out: tsp_OMP.cpp
	$(CXX) $(CXXFLAGS) $(OMP_CXXFLAGS) $^ -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

clean:
	rm -f *.i* *.o*