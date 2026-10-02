# Makefile hand-written by Quin Goins
# 2026-09-19

CXX = g++
CXXFLAGS = -Wall -Wpedantic -std=c++20 -Ofast

all: heat_2d_serial.out heat_2d_OMP.out

heat_2d_serial.out: heat_2d_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

heat_2d_OMP.out: heat_2d_OMP.cpp
	$(CXX) $(CXXFLAGS) -fopenmp $^ -o $@

.PHONY: clean
clean:
	rm -f *.i* *.o*