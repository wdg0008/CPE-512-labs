#!/bin/bash
# tsp_serial benchmark script
# set number of cores = 1
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
# g++ tsp_serial.cpp -o tsp_serial -Ofast
make

# serial reference runs
# redirect standard out to tsp_serial.txt 
./tsp_serial.out 5   > tsp_serial.txt
./tsp_serial.out 6  >> tsp_serial.txt
./tsp_serial.out 7  >> tsp_serial.txt
./tsp_serial.out 8  >> tsp_serial.txt
./tsp_serial.out 9  >> tsp_serial.txt
./tsp_serial.out 10 >> tsp_serial.txt
./tsp_serial.out 11 >> tsp_serial.txt
./tsp_serial.out 12 >> tsp_serial.txt
./tsp_serial.out 13 >> tsp_serial.txt
./tsp_serial.out 14 >> tsp_serial.txt
./tsp_serial.out 15 >> tsp_serial.txt
