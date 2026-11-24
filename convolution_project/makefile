#!/usr/bin/env make

include ../config/toolchains.mk

CPPFLAGS += -O3 -ffast-math
# add -march=native if you are brave and want optimizations

.PHONY: all clean

EXES := conv_serial.c

conv_serial_c.out: conv_serial_c.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -rf $(JUNK)
