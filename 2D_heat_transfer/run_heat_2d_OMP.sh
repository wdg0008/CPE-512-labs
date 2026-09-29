#!/bin/bash
# This script calls the run_script_omp utility six times to schedule 
# the serial reference simulation and the five other multi-threaded OMP
# simulations (1, 2, 4, 8, and 16 OMP threads). 
# The script requests the number of processing cores that is equal 
# to the number of threads that are to be executed on the asax 
#
# Re-compile source files (serial & OMP)
# Re-compiling Reference Source File
module load gcc
cp /home/shared/wells_class/2D_heat_transfer/heat_2d_serial.cpp .
if test -f "./heat_2d_serial.cpp" 
  then
  echo "Recompiling Reference Serial Source file heat_2d_serial.cpp"
  echo "g++ heat_2d_serial.cpp -o heat_2d_serial -std=c++11 -Ofast"
  g++ heat_2d_serial.cpp -o heat_2d_serial -std=c++11 -Ofast
  echo "complete!"
else
  echo "No valid serial source file found!"
  echo "Looking for file heat_2d_serial.cpp to compile :{"
  exit
fi  
# Re-compiling OMP Source File
if test -f "./heat_2d_OMP.cpp" 
  then
  echo "Recompiling Source file heat_2d_OMP.cpp"
  echo "g++ heat_2d_OMP.cpp -o heat_2d_OMP -fopenmp -std=c++11 -Ofast"
  g++ heat_2d_OMP.cpp -o heat_2d_OMP -fopenmp -std=c++11 -Ofast
  echo "complete!"
  # copy mpi scripts to the current directory
  echo "Copying script files to current directory"
  cp /home/shared/wells_class/2D_heat_transfer/.omp/.scripts/* . 
  echo "complete!"
else
  echo "No valid OMP source file found!"
  echo "Looking for file heat_2d_OMP.cpp to compile :{"
  exit
fi  
# copy general scripts to the current directory
#echo "Copying plot scripting files to current directory"
cp /home/shared/wells_class/OMP_Examples/2D_heat_transfer/.scripts/*.sh . 

# Check for script run time arch constraint parameter
if test $# -ne 0
  then
  constraint=$1
else
  constraint=""
fi
echo "Architectural Constraint: "$constraint
echo

# Serial Reference Run
# Number of Nodes: 1 
# Script File: heat_2d_serial_tm.txt
echo -e "Scheduling Serial Job on the ASA-X\n"
echo -e "class\n1\n01:00:00\n32gb\n\n"$constraint"\n" | run_script_omp heat_2d_serial_asax.sh >heat_2d_serial_tm.txt
#
# 1-Threaded OMP Run
# Number of Nodes: 1 
# Script File: heat_2d_OMP_1_tm.txt
echo -e "Scheduling 1-Threaded OMP Job on the ASA-X\n"
echo -e "class\n1\n01:00:00\n32gb\n\n"$constraint"\n" | run_script_omp heat_2d_OMP_1_asax.sh >heat_2d_OMP_1_tm.txt
#
# 2-Threaded OMP Run
# Number of Nodes: 2 
# Script File: heat_2d_OMP_2_tm.txt
echo -e "Scheduling 2-Threaded OMP Job on the ASA-X\n"
# ASA-X Node Architecture Constraint: first argument
echo -e "class\n2\n01:00:00\n32gb\n\n"$constraint"\n" | run_script_omp heat_2d_OMP_2_asax.sh >heat_2d_OMP_2_tm.txt
#
# 4-Threaded OMP Run
# Number of Nodes: 4 
# Script File: heat_2d_OMP_4_tm.txt
echo -e "Scheduling 4-Threaded OMP Job on the ASA-X\n"
echo -e "class\n4\n01:00:00\n32gb\n\n"$constraint"\n" | run_script_omp heat_2d_OMP_4_asax.sh >heat_2d_OMP_4_tm.txt
#
# 8-Threaded OMP Run
# Number of Nodes: 8 
# Script File: heat_2d_OMP_8_tm.txt
echo -e "Scheduling 8-Threaded OMP Job on the ASA-X\n"
echo -e "class\n8\n01:00:00\n32gb\n\n"$constraint"\n" | run_script_omp heat_2d_OMP_8_asax.sh >heat_2d_OMP_8_tm.txt
#
# 16-Threaded OMP Run
# Number of Nodes: 16 
# Script File: heat_2d_OMP_16_tm.txt
echo -e "Scheduling 16-Threaded OMP Job on the ASA-X\n"
echo -e "class\n16\n01:00:00\n32gb\n\n"$constraint"\n" | run_script_omp heat_2d_OMP_16_asax.sh >heat_2d_OMP_16_tm.txt

