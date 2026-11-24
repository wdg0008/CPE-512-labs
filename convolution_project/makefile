#!/usr/bin/env make

include ../config/toolchains.mk

.PHONY: all clean

EXES := conv_serial_c.out conv_OMP_c.out conv_dynamic_serial_c.out \
		conv_serial.out

all: $(EXES)

clean:
	rm -rf $(JUNK)
