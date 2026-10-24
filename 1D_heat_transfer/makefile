#!/usr/bin/env make

include ../toolchains.mk

.PHONY: all clean

EXES := heat_1d_serial.out heat_1d_PTH1.out heat_1d_PTH2.out heat_1d_PTH3.out heat_1d_PTH4.out heat_1d_PTH5.out

all: $(EXES)

heat_1d_serial.out: heat_1d_serial.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

heat_1d_PTH1.out: heat_1d_PTH1.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ -pthread

heat_1d_PTH2.out: heat_1d_PTH2.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ -pthread

# The following executables use pthread_barrier_t
# This requires the pthread_barrier_macos library on macOS
heat_1d_PTH3.out: heat_1d_PTH3.cpp $(PTHREAD_BARRIER_TARGET)
	$(CXX) $(CPPFLAGS) $(PTHREAD_CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

heat_1d_PTH4.out: heat_1d_PTH4.cpp $(PTHREAD_BARRIER_TARGET)
	$(CXX) $(CPPFLAGS) $(PTHREAD_CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

heat_1d_PTH5.out: heat_1d_PTH5.cpp $(PTHREAD_BARRIER_TARGET)
	$(CXX) $(CPPFLAGS) $(PTHREAD_CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

clean:
	rm -f $(EXES) *.txt *.i* *.o*