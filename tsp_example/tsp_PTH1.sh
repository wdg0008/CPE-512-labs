#!/bin/bash
# tsp_PTH1 benchmark script
# set number of cores = 13
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
make

#pthread runs
#redirect standard out to tsp_PTH1.txt 
./tsp_PTH1.out 5   > tsp_PTH1.txt
./tsp_PTH1.out 6  >> tsp_PTH1.txt
./tsp_PTH1.out 7  >> tsp_PTH1.txt
./tsp_PTH1.out 8  >> tsp_PTH1.txt
./tsp_PTH1.out 9  >> tsp_PTH1.txt
./tsp_PTH1.out 10 >> tsp_PTH1.txt
./tsp_PTH1.out 11 >> tsp_PTH1.txt
./tsp_PTH1.out 12 >> tsp_PTH1.txt
./tsp_PTH1.out 13 >> tsp_PTH1.txt
./tsp_PTH1.out 14 >> tsp_PTH1.txt
