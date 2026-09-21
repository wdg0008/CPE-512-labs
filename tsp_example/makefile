# Makefile hand-written by Quin Goins
# 2026-09-19

CXX = g++
CXXFLAGS = -Wall -Wpedantic -std=c++20 -Ofast

all: tsp_serial.out tsp_PTH1.out tsp_PTH2.out tsp_OMP.out

tsp_serial.out: tsp_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

tsp_PTH1.out: tsp_PTH1.cpp
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@

tsp_PTH2.out: tsp_PTH2.cpp
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@

tsp_OMP.out: tsp_OMP.cpp
	$(CXX) $(CXXFLAGS) -fopenmp $^ -o $@

.PHONY: clean
clean:
	rm -f *.i* *.o*