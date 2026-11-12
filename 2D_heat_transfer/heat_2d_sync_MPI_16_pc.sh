#!/bin/bash
# UAH Picocluster System
#
# 16 MPI Process Case
# This script executes the n x n point 2d-heat transfer problem 
# sweeping n from 32 to 16,384 for a total of 10 runs.
# For each run, a total of 100 iterations is executed. The G 
# option causes the output data to be suppressed and the 
# execution time for each run to be outputed in a format
# that it can be used by the gnuplot application. Data is 
# plotted in textual format in x y format, where x
# is the value of n and y is the execution time in seconds. The
# output of each run is concatenated and placed in the file
# "heat_2d_sync_MPI_16_tm.txt" by the redirect operators 
# in this script.
mpiexec ./heat_2d_sync_MPI    32 100 G >  heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI    64 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI   128 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI   256 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI   512 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI  1024 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI  2048 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI  4096 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI  8192 100 G >> heat_2d_sync_MPI_16_tm.txt
mpiexec ./heat_2d_sync_MPI 16384 100 G >> heat_2d_sync_MPI_16_tm.txt
