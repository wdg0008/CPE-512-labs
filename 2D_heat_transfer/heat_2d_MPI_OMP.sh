#!/bin/bash
mpiexec ./heat_2d_MPI_OMP.out 10000 75 S > heat_2d_MPI_OMP_16.txt  # warmup case 
mpiexec ./heat_2d_MPI_OMP.out 10000 75 S > heat_2d_MPI_OMP_16.txt  # actual timing run
mpiexec ./heat_2d_MPI_OMP.out 10000 75 C >> heat_2d_MPI_OMP_16.txt # checksum run