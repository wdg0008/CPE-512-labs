#!/bin/bash
module load openmpi
#redirect standard out to hello_world_MPI.txt file
mpirun ./hello_world_MPI >hello_world_MPI.txt
