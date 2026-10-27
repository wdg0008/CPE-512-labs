#!/usr/bin/env make

include ../config/toolchains.mk

.PHONY: all clean

EXES := heat_1d_serial.out heat_1d_PTH1.out heat_1d_PTH2.out heat_1d_PTH3.out heat_1d_PTH4.out heat_1d_PTH5.out

all: $(EXES)

# static rule to generate pthreads targets
PTHREAD_SOURCES := $(wildcard *_PTH[1-2].cpp)
PTHREAD_TARGETS := $(PTHREAD_SOURCES:.cpp=.out)

$(PTHREAD_TARGETS): %.out: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $< -o $@

# The following executables use pthread_barrier_t
# This requires the pthread_barrier_macos library on macOS

PTHREAD_EXT_SOURCES := $(wildcard *_PTH[3-5].cpp)
PTHREAD_EXT_TARGETS := $(PTHREAD_EXT_SOURCES:.cpp=.out)

$(PTHREAD_EXT_TARGETS): %.out: %.cpp $(PTHREAD_BARRIER_TARGET)
	$(CXX) $(CPPFLAGS) $(PTHREAD_CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

clean:
	rm -f $(JUNK)