#!/bin/bash
# tsp_PTH2 benchmark script
# set number of cores = 14
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
g++ tsp_PTH2.cpp -o tsp_PTH2 -lpthread -Ofast

#pthread runs
#redirect standard out to tsp_PTH2.txt 
./tsp_PTH2 5   > tsp_PTH2.txt
./tsp_PTH2 6  >> tsp_PTH2.txt
./tsp_PTH2 7  >> tsp_PTH2.txt
./tsp_PTH2 8  >> tsp_PTH2.txt
./tsp_PTH2 9  >> tsp_PTH2.txt
./tsp_PTH2 10 >> tsp_PTH2.txt
./tsp_PTH2 11 >> tsp_PTH2.txt
./tsp_PTH2 12 >> tsp_PTH2.txt
./tsp_PTH2 13 >> tsp_PTH2.txt
./tsp_PTH2 14 >> tsp_PTH2.txt
./tsp_PTH2 15 >> tsp_PTH2.txt

