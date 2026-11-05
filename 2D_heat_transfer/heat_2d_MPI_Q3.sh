#!/bin/bash
./heat_2d_serial.out 4096 100 S > Q3.txt
# ASAX has a hard time with multiple mpiexec calls in one script
mpiexec -np 1 ./heat_2d_loc_blk_MPI.out 4096 100 S >> Q3.txt
mpiexec -np 3 ./heat_2d_loc_blk_MPI.out 4096 100 S >> Q3.txt
mpiexec -np 5 ./heat_2d_loc_blk_MPI.out 4096 100 S >> Q3.txt
