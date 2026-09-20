#!/bin/bash
# tsp_PTH1 benchmark script
# set number of cores = 13
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
g++ tsp_PTH1.cpp -o tsp_PTH1 -lpthread -Ofast

#pthread runs
#redirect standard out to tsp_PTH1.txt 
./tsp_PTH1 5   > tsp_PTH1.txt
./tsp_PTH1 6  >> tsp_PTH1.txt
./tsp_PTH1 7  >> tsp_PTH1.txt
./tsp_PTH1 8  >> tsp_PTH1.txt
./tsp_PTH1 9  >> tsp_PTH1.txt
./tsp_PTH1 10 >> tsp_PTH1.txt
./tsp_PTH1 11 >> tsp_PTH1.txt
./tsp_PTH1 12 >> tsp_PTH1.txt
./tsp_PTH1 13 >> tsp_PTH1.txt
./tsp_PTH1 14 >> tsp_PTH1.txt
