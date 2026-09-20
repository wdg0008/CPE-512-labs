# Makefile hand-written by Quin Goins
# 2026-09-19

CXX = g++
CXXFLAGS = -Wall -Wpedantic -std=c++20 -Ofast

all: tsp_serial tsp_PTH1 tsp_PTH2 tsp_OMP

tsp_serial: tsp_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@.out

tsp_PTH1: tsp_PTH1.cpp
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@.out

tsp_PTH2: tsp_PTH2.cpp
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@.out

tsp_OMP: tsp_OMP.cpp
	$(CXX) $(CXXFLAGS) -fopenmp $^ -o $@.out

.PHONY: clean
clean:
	rm -f *.txt *.i* *.o*