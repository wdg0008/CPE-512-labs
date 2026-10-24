#!/usr/bin/env make

include ../toolchains.mk

.ONESHELL:

.PHONY: all clean

EXES := add_num_MPI.out add_num_rec_level_PTH1.out \
		add_num_rec_level_serial.out add_num_recursive_serial.out \
		add_num_serial.out

ifeq ($(ASAX),1)
EXES += add_num_MPI2.out
endif

all: $(EXES)

clean:
	rm -f $(JUNK)