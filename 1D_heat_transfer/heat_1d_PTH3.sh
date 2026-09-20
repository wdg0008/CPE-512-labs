#!/bin/bash
module load gcc
# run with varying number of threads and redirect stdout
# concatenate all output in heat_1d_PTH3.txt
./heat_1d_PTH3 12 20 1  > heat_1d_PTH3.txt
./heat_1d_PTH3 12 20 2 >> heat_1d_PTH3.txt
./heat_1d_PTH3 12 20 3 >> heat_1d_PTH3.txt
./heat_1d_PTH3 12 20 4 >> heat_1d_PTH3.txt