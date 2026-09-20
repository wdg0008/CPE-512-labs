#!/bin/bash
# tsp_serial benchmark script
# set number of cores = 1
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
g++ tsp_serial.cpp -o tsp_serial -Ofast

# serial reference runs
# redirect standard out to tsp_serial.txt 
./tsp_serial 5   > tsp_serial.txt
./tsp_serial 6  >> tsp_serial.txt
./tsp_serial 7  >> tsp_serial.txt
./tsp_serial 8  >> tsp_serial.txt
./tsp_serial 9  >> tsp_serial.txt
./tsp_serial 10 >> tsp_serial.txt
./tsp_serial 11 >> tsp_serial.txt
./tsp_serial 12 >> tsp_serial.txt
./tsp_serial 13 >> tsp_serial.txt
./tsp_serial 14 >> tsp_serial.txt
./tsp_serial 15 >> tsp_serial.txt
