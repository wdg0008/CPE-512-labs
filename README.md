# CPE 512 Lab Code

This repository contains code written for [CPE 512 - Intro to Parallel Programming](https://catalog.uah.edu/#/courses/6732c244744e35849f2c6ea9) at [The University of Alabama in Huntsville](https://uah.edu).

It was originally developed for the ASAX supercomputer operated by the [Alabama Supercomputer Authority](https://asc.edu/) which runs Linux with a [PBS job-queueing system](https://en.wikipedia.org/wiki/Portable_Batch_System) to schedule execution of jobs written as bash scripts. These scripts contain examples of how to execute the various programs.

There are three main technologies/frameworks used to parallelize constructs in this code:

1. [POSIX Threads (pthreads)](https://pubs.opengroup.org/onlinepubs/9799919799/)
2. [OpenMP](https://www.openmp.org/)
3. [Message Passing Interface (MPI)](https://www.mpi-forum.org/)

It is organized with subfolders for each unit in the class, with most of the code based on serial versions originally provided by the instructor.

## Projects

### 1D Heat Transfer

This is a stencil computation to simulate the transfer of heat between two known boundary positions at the end of an array.
These boundary conditions are hardcoded in the simulation as constant expressions.

```bash
./heat_1d_PTH5.out <active_points> <iterations> <threads>
```

### 2D Heat Transfer

This is a similar stencil computation to simulate the transfer of heat from a set of boundary conditions on the edge of a matrix by averaging each of the four points adjacent to any point in the temperature matrix. The number of threads to use can be modified seperately by using the environment variable. The initial conditions are hardcoded constant expressions.

```bash
# next line is optional
export OMP_NUM_THREADS=4
# side length of the square matrix used in simulation
./heat_2d_OMP.out <matrix_side_len> <iterations>
```

### Travelling Salesman Problem

This is the classic algorithm to find the least expensive hamiltonian cycle in a graph. The functions to generate the matrix of travel costs along edges are the same between each implementation, allowing direct comparison of efficiency. This code will print out the results to the terminal.

>[!Warning]
>The problem here is O(n!) complex and will take huge amounts of time to solve for large graphs. Do not simulate more than 15 cities!

```bash
./tsp_OMP.out <num_cities>
```

## Building

This builds and runs easily on ASAX right now. If you want to try this at home, the setup details vary widely by platform and are listed below based on OS and compiler vendor. Once the prerequisites are installed, run `make -j N` to build in parallel on `N` many processors.

Each unit has an independent makefile. Run `make` from a unit directory to build it, or run `make <subfolder_name>` from the repository root to build just one unit.

## Linux

You need to install several packages from your distribution's package manager (YUM, pacman, apt, etc.). To get this working on Ubuntu 26.04 LTS, I ran the following:

```bash
sudo apt update && sudo apt upgrade
sudo apt install build-essential gdb openmpi-bin openmpi-common libopenmpi-dev
```

>[!Note] 
>This list of packages will vary widely form one distribution to another, but something generally similar should be pretty easy to find.

## Mac OS X

Install Xcode or the Xcode Command Line Tools version 16 or later, then run the following in [Homebrew](https://brew.sh) to install missing dependencies that Apple does not ship.

```zsh
brew install libomp
brew install openmpi
```

The `makefile` will also build some code that implements the optional `pthread_barrier_t` type from the POSIX standard which macOS does not provide.

## Windows

The `makefile` does not work yet on windows, but could probably be adapted to.

### MSYS2

Install `gcc` from [MSYS2](https://msys2.org) to use the same compiler options and build code that depends on pthreads. Grab the following dependencies from a UCRT64 shell.

```bash
pacman -Syu base-devel mingw-w64-ucrt-x86_64-toolchain
```

>[!Note]
>Probably needs some other stuff, still have to test with what is actually required in UCRT64 environment.

### MSVC

>[!Warning]
>Using MSVC is not reccomended because most of these programs assume a UNIX-like system and rely on headers like `<sys/time.h>`. POSIX threads are completely absent and unsupported in Visual Studio. [MSVC support for OpenMP](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support) is limited to the 2.0 standard from 2002, but you can get a little further with `/openmp:llvm` for some (experimental!) support for more modern features.

If you want to try it, install the latest version of Visual Studio with the "C++ Development Workload" and the "Clang tools for Visual Studio" optional component. Use the Developer Powershell environment that comes with it to compile OpenMP code as follows.

```powershell
cl .\hello_world_OMP.cpp /EHsc /std:c++20 /Zc:preprocessor /openmp
```

You can install the [Microsoft MPI](https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi) implementation with the following command in Windows 11:

```powershell
winget install Microsoft.msmpi
```

This has not been tested because we have yet to go over MPI in class, and it requires some additional setup. The MSMPI project seems to be on life support, with only one patch release in the last 5 years, but it isn't marked deprecated.

## A note on MPI C++ bindings

Apparently, the C++ bindings we are learning to use have been deprecated so long that they have been removed from most implementations in the wild today. To switch MPI libraries on ASAX so that the old version with these can be used, you have to load the appropriate module.

```bash
module load mpich
```

Code using this will likely fail to build in the future, and I'm not about to take up the maintenance burden this was removed to relieve from major software vendors.
