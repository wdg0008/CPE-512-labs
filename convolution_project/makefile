#!/usr/bin/env make

include ../config/toolchains.mk

.PHONY: all clean

EXES := conv_serial_c.out conv_OMP_c.out conv_dynamic_serial_c.out \
		conv_dynamic_OMP_c.out conv_serial.out conv_span_serial.out \
		conv_OMP.out

all: $(EXES)

clean:
	rm -rf $(JUNK)
