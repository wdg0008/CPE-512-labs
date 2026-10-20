# Only works on ASAX where obsolete C++ bindings for MPI are installed
# Special directive to preserve environment modifications from modules
.ONESHELL:

hello_world_MPI2.out: hello_world_MPI2.cpp
	module load mpich
	mpic++ $< -o $@