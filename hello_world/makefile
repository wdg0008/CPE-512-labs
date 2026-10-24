#!/usr/bin/env make

include ../toolchains.mk

# Special directive so all recipe lines for one target run in the same shell
# preserves environment modifications from loading modules within a target
.ONESHELL:

.PHONY: all clean

EXES := hello_world.out hello_world_OMP.out hello_world_PTH.out \
		hello_world_PTH_OMP.out prime.out hello_world_MPI.out

# ignore obsolete C++ bindings unless ASAX=1 passed
ifeq ($(ASAX),1)
EXES += hello_world_MPI2.out
endif

# compile everything by default
all: $(EXES)

clean:
	rm -f $(JUNK)