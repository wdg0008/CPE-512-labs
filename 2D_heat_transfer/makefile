#!/usr/bin/make -f

all: heat_2d_serial.out heat_2d_OMP.out

heat_2d_serial.out: heat_2d_serial.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

heat_2d_OMP.out: heat_2d_OMP.cpp
	$(CXX) $(CXXFLAGS) $(OMP_CXXFLAGS) $< -o $@ $(OMP_LDFLAGS) $(OMP_LIBS)

.PHONY: clean
clean:
	rm -f *.i* *.o*