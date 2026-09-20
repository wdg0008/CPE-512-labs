#!/bin/bash
# tsp_OMP benchmark script
# set number of cores = 14
# set total memory 1gb

module load gcc

#(re-)compiling programs with fast optimization
g++ tsp_OMP.cpp -o tsp_OMP -fopenmp -Ofast

#OpenMP runs
#redirect standard out to tsp_OMP.txt 
./tsp_OMP 5   > tsp_OMP.txt
./tsp_OMP 6  >> tsp_OMP.txt
./tsp_OMP 7  >> tsp_OMP.txt
./tsp_OMP 8  >> tsp_OMP.txt
./tsp_OMP 9  >> tsp_OMP.txt
./tsp_OMP 10 >> tsp_OMP.txt
./tsp_OMP 11 >> tsp_OMP.txt
./tsp_OMP 12 >> tsp_OMP.txt
./tsp_OMP 13 >> tsp_OMP.txt
./tsp_OMP 14 >> tsp_OMP.txt
./tsp_OMP 15 >> tsp_OMP.txt
