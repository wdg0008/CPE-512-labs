#!/usr/bin/env make

include ../toolchains.mk

.PHONY: all clean

all: heat_2d_serial.out heat_2d_OMP.out

heat_2d_serial.out: heat_2d_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

heat_2d_OMP.out: heat_2d_OMP.cpp
	$(CXX) $(CXXFLAGS) $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

clean:
	rm -f *.i* *.o*