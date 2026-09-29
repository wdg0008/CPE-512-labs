#!/bin/bash
# tsp_OMP benchmark script
# set number of cores = 14
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
make

#OpenMP runs
#redirect standard out to tsp_OMP.txt 
./tsp_OMP.out 5   > tsp_OMP.txt
./tsp_OMP.out 6  >> tsp_OMP.txt
./tsp_OMP.out 7  >> tsp_OMP.txt
./tsp_OMP.out 8  >> tsp_OMP.txt
./tsp_OMP.out 9  >> tsp_OMP.txt
./tsp_OMP.out 10 >> tsp_OMP.txt
./tsp_OMP.out 11 >> tsp_OMP.txt
./tsp_OMP.out 12 >> tsp_OMP.txt
./tsp_OMP.out 13 >> tsp_OMP.txt
./tsp_OMP.out 14 >> tsp_OMP.txt
./tsp_OMP.out 15 >> tsp_OMP.txt
