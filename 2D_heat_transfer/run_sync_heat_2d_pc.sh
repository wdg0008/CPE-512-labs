#!/bin/bash
# This script calls the run_script utility six times to schedule 
# the serial reference simulation and the five other multi-process MPI
# simulations (1, 2, 4, 8, and 16 MPI process) that utilize synchronous
# point-to-point communication routines for the ghost-point communications.
# The script requests the number of processing nodes to equal the number
# of processes that are to be executed on the UAH ECE Picocluster System 
#
# Re-compile source files (serial & MPI)
# Re-compiling Reference Source File
if test -f "./heat_2d_serial.cpp" 
  then
  echo "Recompiling Reference Serial Source file heat_2d_serial.cpp"
  echo "g++ heat_2d_serial.cpp -o heat_2d_serial.out -std=c++11 -Ofast"
  g++ heat_2d_serial.cpp -o heat_2d_serial.out -std=c++11 -Ofast
  echo "complete!"
else
  if test -f "./heat_2d_serial.c" 
    then
    echo "Recompiling Source file heat_2d_serial.c"
    echo "gcc heat_2d_serial.c -o heat_2d_serial.out -Ofast"
    gcc heat_2d_serial.c -o heat_2d_serial.out -Ofast
    echo "complete!"
  else
    cp /shared/wells_class/2D_heat_transfer/heat_2d_serial.cpp .
    echo "Recompiling Reference Serial Source file heat_2d_serial.cpp"
    echo "g++ heat_2d_serial.cpp -o heat_2d_serial.out -std=c++11 -Ofast"
    g++ heat_2d_serial.cpp -o heat_2d_serial.out -std=c++11 -Ofast
    echo "complete!"
  fi  
fi  
# Re-compiling MPI Source File
if test -f "./heat_2d_sync_MPI.cpp" 
  then
  echo "Recompiling Source file heat_2d_sync_MPI.cpp"
  echo "mpic++ heat_2d_sync_MPI.cpp -o heat_2d_sync_MPI.out -std=c++11 -Ofast"
  mpic++ heat_2d_sync_MPI.cpp -o heat_2d_sync_MPI.out -std=c++11 -Ofast
  echo "complete!"
else
  if test -f "./heat_2d_sync_MPI2.cpp" 
    then
    echo "Recompiling Source file heat_2d_sync_MPI2.cpp"
    echo "mpic++ heat_2d_sync_MPI2.cpp -o heat_2d_sync_MPI.out -std=c++11 -Ofast"
    mpic++ heat_2d_sync_MPI2.cpp -o heat_2d_sync_MPI.out -std=c++11 -Ofast
    echo "complete!"
  else
    if test -f "./heat_2d_sync_MPI.c" 
      then
      echo "Recompiling Source file heat_2d_sync_MPI.c"
      echo "mpicc heat_2d_sync_MPI.c -o heat_2d_sync_MPI.out -Ofast"
      mpicc heat_2d_sync_MPI.c -o heat_2d_sync_MPI.out -Ofast
      echo "complete!"
    else
      echo "No valid source file found!"
      echo "Looking for file heat_2d_sync_MPI.cpp, heat_2d_sync_MPI2.cpp"
      echo "or heat_2d_sync_MPI.c to compile"
      exit
    fi  
  fi  
fi  
# copy general scripts to the current directory
echo "Copying plot scripting files to current directory"
cp /shared/wells_class/2D_heat_transfer/.sync_scripts/* . 

# Serial Reference Run
# Number of Nodes: 1 
# Script File: heat_2d_serial_tm.txt
echo -e "Scheduling Serial Job on the Picocluster System\n"
echo -e "1\n\n\n" | run_script heat_2d_serial_pc.sh >heat_2d_serial_tm.txt
#
# 1 MPI Process Run
# Number of Nodes: 1 
# Script File: heat_2d_sync_MPI_1_tm.txt
echo -e "Scheduling 1 MPI Process Job on the Picocluster System\n"
echo -e "1\n\n\n" | run_script heat_2d_sync_MPI_1_pc.sh >heat_2d_sync_MPI_1_tm.txt
#
# 2 MPI Process Run
# Number of Nodes: 2 
# Script File: heat_2d_sync_MPI_2_tm.txt
echo -e "Scheduling 2 MPI Process Job on the Picocluster System\n"
echo -e "2\n\n\n" | run_script heat_2d_sync_MPI_2_pc.sh >heat_2d_sync_MPI_2_tm.txt
#
# 4 MPI Process Run
# Number of Nodes: 4 
# Script File: heat_2d_sync_MPI_4_tm.txt
echo -e "Scheduling 4 MPI Process Job on the Picocluster System\n"
echo -e "4\n\n\n" | run_script heat_2d_sync_MPI_4_pc.sh >heat_2d_sync_MPI_4_tm.txt
#
# 8 MPI Process Run
# Number of Nodes: 8 
# Script File: heat_2d_sync_MPI_8_tm.txt
echo -e "Scheduling 8 MPI Process Job on the Picocluster System\n"
echo -e "8\n\n\n" | run_script heat_2d_sync_MPI_8_pc.sh >heat_2d_sync_MPI_8_tm.txt
#
# 16 MPI Process Run
# Number of Nodes: 16 
# Script File: heat_2d_sync_MPI_16_tm.txt
echo -e "Scheduling 16 MPI Process Job on the Picocluster System\n"
echo -e "16\n\n\n" | run_script heat_2d_sync_MPI_16_pc.sh >heat_2d_sync_MPI_16_tm.txt

