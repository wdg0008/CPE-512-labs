#!/usr/bin/env make

include ../config/toolchains.mk

CXXFLAGS += -O3 -ffast-math
# add -march=native if you are brave and want optimizations

.PHONY: all clean

# mostly use included pattern rules
all: tsp_serial.out tsp_PTH1.out tsp_PTH2.out tsp_OMP.out

# static rule to generate pthreads targets
PTHREAD_SOURCES := $(wildcard *_PTH[1-9].cpp)
PTHREAD_TARGETS := $(PTHREAD_SOURCES:.cpp=.out)

$(PTHREAD_TARGETS): %.out: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $< -o $@

clean:
	rm -f $(JUNK)
