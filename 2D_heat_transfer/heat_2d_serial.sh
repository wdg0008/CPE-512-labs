#!/bin/bash
# ASC ASAX
module load gcc
make
# First Run -- generate checksum
./heat_2d_serial.out  17 50 S >  heat_2d_serial_1_check.txt
# Second Run -- output results -- file grows large very quick
#            -- for timing purposes should omit
./heat_2d_serial.out  17 50   >  heat_2d_serial_1_result.txt
# Third Run -- output run time only in gnuplot form
./heat_2d_serial.out  17 50 G >  heat_2d_serial_1_tm.txt
