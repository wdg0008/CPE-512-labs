#!/bin/bash
#Uses the mpich distribution of MPI which supports C++ bindings
#of MPI functions -- gnu compiler
module load mpich
#redirect standard out to hello_world_MPI2.txt file
mpiexec ./hello_world_MPI2 >hello_world_MPI.txt
