#!/bin/bash
# tsp_PTH2 benchmark script
# set number of cores = 14
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
make

#pthread runs
#redirect standard out to tsp_PTH2.txt 
./tsp_PTH2.out 5   > tsp_PTH2.txt
./tsp_PTH2.out 6  >> tsp_PTH2.txt
./tsp_PTH2.out 7  >> tsp_PTH2.txt
./tsp_PTH2.out 8  >> tsp_PTH2.txt
./tsp_PTH2.out 9  >> tsp_PTH2.txt
./tsp_PTH2.out 10 >> tsp_PTH2.txt
./tsp_PTH2.out 11 >> tsp_PTH2.txt
./tsp_PTH2.out 12 >> tsp_PTH2.txt
./tsp_PTH2.out 13 >> tsp_PTH2.txt
./tsp_PTH2.out 14 >> tsp_PTH2.txt
./tsp_PTH2.out 15 >> tsp_PTH2.txt
