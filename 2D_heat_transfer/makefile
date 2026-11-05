#!/usr/bin/env make

include ../config/toolchains.mk

.ONESHELL:

CPPFLAGS += -O3 -ffast-math
# add -march=native if you are brave and want optimizations

.PHONY: all clean

EXES := heat_2d_serial.out heat_2d_OMP.out heat_2d_loc_blk_MPI_rev1.out \
		heat_2d_loc_blk_MPI_rev2.out heat_2d_loc_blk_MPI.out \
		heat_2d_sync_MPI.out heat_2d_serial_c.out

ifeq ($(ASAX),1)
EXES += heat_2d_loc_blk_MPI2.out
endif

all: $(EXES)

# cannot make this a pattern rule because of default C++ guess
# %.out: %.cpp
heat_2d_serial_c.out: heat_2d_serial.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

# static rule to generate MPI revision targets
REV_SOURCES := $(wildcard *MPI_rev[1-9].cpp)
REV_TARGETS := $(REV_SOURCES:.cpp=.out)

$(REV_TARGETS): %.out: %.cpp
ifeq ($(ASAX),1)
	module load openmpi
endif
	$(MPICXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(JUNK)
