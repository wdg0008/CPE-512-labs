#!/bin/bash
module load gcc
# use 12 active points and 20 iterations
# redirect standard out to heat_1d_serial.txt file
./heat_1d_serial 12 20 > heat_1d_serial.txt