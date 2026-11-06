#!/bin/bash
# This script calls the run_script utility six times to schedule 
# the serial reference simulation and the five other multi-process MPI
# simulations (1, 2, 4, 8, and 16 MPI process) that utilize synchronous
# point-to-point communication routines for the ghost-point communications.
# The script requests the number of processing nodes to equal the number
# of processes that are to be executed on the Jetson Cluster 
#
# Re-compile source files (serial & MPI)
# Re-compiling Reference Source File
if test -f "./heat_2d_serial.cpp" 
  then
  echo "Recompiling Reference Serial Source file heat_2d_serial.cpp"
  echo "g++ heat_2d_serial.cpp -o heat_2d_serial.out-std=c++11 -Ofast"
  g++ heat_2d_serial.cpp -o heat_2d_serial.out-std=c++11 -Ofast
  echo "complete!"
else
  if test -f "./heat_2d_serial.c" 
    then
    echo "Recompiling Source file heat_2d_serial.c"
    echo "gcc heat_2d_serial.c -o heat_2d_serial.out-Ofast"
    gcc heat_2d_serial.c -o heat_2d_serial.out-Ofast
    echo "complete!"
  else
    cp /shared/wells_class/2D_heat_transfer/heat_2d_serial.cpp .
    echo "Recompiling Reference Serial Source file heat_2d_serial.cpp"
    echo "g++ heat_2d_serial.cpp -o heat_2d_serial.out-std=c++11 -Ofast"
    g++ heat_2d_serial.cpp -o heat_2d_serial.out-std=c++11 -Ofast
    echo "complete!"
  fi  
fi  
# Re-compiling MPI Source File
if test -f "./heat_2d_MPI_OMP.cpp" 
  then
  echo "Recompiling Source file heat_2d_MPI_OMP.cpp"
  echo "mpic++ -fopenmp heat_2d_MPI_OMP.cpp -o heat_2d_MPI_OMP -std=c++11 -Ofast"
  mpic++ -fopenmp heat_2d_MPI_OMP.cpp -o heat_2d_MPI_OMP -std=c++11 -Ofast
  echo "complete!"
else
  if test -f "./heat_2d_MPI2_OMP.cpp" 
    then
    echo "Recompiling Source file heat_2d_MPI_OMP2.cpp"
    echo "mpic++ -fopenmp heat_2d_MPI_OMP2.cpp -o heat_2d_MPI_OMP -std=c++11 -Ofast"
    mpic++ -fopenmp heat_2d_MPI_OMP2.cpp -o heat_2d_MPI_OMP -std=c++11 -Ofast
    echo "complete!"
  else
    if test -f "./heat_2d_MPI_OMP.c" 
      then
      echo "Recompiling Source file heat_2d_MPI_OMP.c"
      echo "mpicc -fopenmp heat_2d_MPI_OMP.c -o heat_2d_MPI_OMP -Ofast"
      mpicc -fopenmp heat_2d_MPI_OMP.c -o heat_2d_MPI_OMP -Ofast
      echo "complete!"
    else
      echo "No valid source file found!"
      echo "Looking for file heat_2d_MPI_OMP.cpp, heat_2d_MPI_OMP2.cpp"
      echo "or heat_2d_MPI_OMP.c to compile"
      exit
    fi  
  fi  
fi  
# copy general scripts to the current directory
echo "Copying plot scripting files to current directory"
cp /shared/wells_class/2D_heat_transfer/.MPI_OMP_scripts/*.sh . 
cp /shared/wells_class/2D_heat_transfer/.MPI_OMP_scripts/*.gnu . 

# Serial Reference Run
# Number of Nodes: 1 
# Script File: heat_2d_serial_tm.txt
echo -e "Scheduling Serial Job on the Jetson Cluster\n"
echo -e "1\n\n\n" | run_script heat_2d_serial_pc.sh > heat_2d_serial_pc.txt
#
# 1 MPI Process Run
# Number of Nodes: 1 
echo -e "Scheduling 1 MPI Process Jobs on the Jetson Cluster\n"
echo -e "1\n4\n\n\n" | run_script_hyb heat_2d_MPI_OMP_1_pc.sh > heat_2d_MPI_OMP_1_pc.txt
#
# 2 MPI Process Run
# Number of Nodes: 2 
echo -e "Scheduling 2 MPI Process 1 Jobs on the Jetson Cluster\n"
echo -e "2\n4\n\n\n" | run_script_hyb heat_2d_MPI_OMP_2_pc.sh > heat_2d_MPI_OMP_2_pc.txt
#
# 4 MPI Process Run
# Number of Nodes: 4 
echo -e "Scheduling 4 MPI Process 1 Jobs on the Jetson Cluster\n"
echo -e "4\n4\n\n\n" | run_script_hyb heat_2d_MPI_OMP_4_pc.sh > heat_2d_MPI_OMP_4_pc.txt
#
# 8 MPI Process Run
# Number of Nodes: 8 
echo -e "Scheduling 8 MPI Process Jobs on the Jetson Cluster\n"
echo -e "8\n4\n\n\n" | run_script_hyb heat_2d_MPI_OMP_8_pc.sh > heat_2d_MPI_OMP_8_pc.txt
#
# 16 MPI Process Run
# Number of Nodes: 16 
echo -e "Scheduling 16 MPI Process Jobs on the Jetson Cluster\n"
echo -e "16\n4\n\n\n" | run_script_hyb heat_2d_MPI_OMP_16_pc.sh > heat_2d_MPI_OMP_16_pc.txt

