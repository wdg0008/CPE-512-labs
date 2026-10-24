#!/bin/bash
module purge
module load openmpi
# mpiexec -np <num_processes> ./<file>.out <list_size> <bins> <uplim> <lowlim> [N]
mpiexec -np 1 ./histogram_MPI_rev2.out 17 4 100 0
mpiexec -np 2 ./histogram_MPI_rev2.out 17 4 100 0
mpiexec -np 4 ./histogram_MPI_rev2.out 17 4 100 0
mpiexec -np 5 ./histogram_MPI_rev2.out 17 4 100 0