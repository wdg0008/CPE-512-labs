# CPE 512 Lab Code

This repository contains code written for [CPE 512 - Intro to Parallel Programming](https://catalog.uah.edu/#/courses/6732c244744e35849f2c6ea9) at [The University of Alabama in Huntsville](https://uah.edu).

It was originally developed for the ASAX supercomputer operated by the [Alabama Supercomputer Authority](https://asc.edu/) which runs Linux with a [PBS job-queueing system](https://en.wikipedia.org/wiki/Portable_Batch_System) to schedule execution of jobs written as bash scripts. These scripts contain examples of how to execute the various programs.

There are three main technologies/frameworks used to parallelize constructs in this code:

1. [POSIX Threads (pthreads)](https://pubs.opengroup.org/onlinepubs/9799919799/)
2. [OpenMP](https://www.openmp.org/)
3. [Message Passing Interface (MPI)](https://www.mpi-forum.org/)

It is organized with subfolders for each unit in the class, with most of the code based on serial versions originally provided by the instructor.

>[!Note]
>This project primarily targets UNIX-style/POSIX-compliant environments. Visual Studio on Windows is partially supported, but not the primary focus.

## Projects

The `config` folder is for managing the build system and contains no example code. The `pthread_barrier_macos` folder is a compatibility hack for macOS. The random headers floating around are also compatibility hacks to make the original code compile on systems besides ASAX.

Everything else gets its own separate folder that contains the following:

1. Source Code
2. Bash scripts to queue a job on ASAX
3. A `makefile`
4. A `winbuild.ps1` script
5. (sometimes) data or outputs from previous executions
6. (after building) the final executables you can run and test out

>[!Tip]
>If you use the `clangd` linter, the `.clangd` file in the repository root will give you correct syntax highlighting and pre-compilation warnings and errors while connected to ASAX. I used this extensively in Visual Studio Code with the Remote Devices extension pack.

### 1D Heat Transfer

This is a stencil computation to simulate the transfer of heat between two known boundary positions at the end of an array. These boundary conditions are hardcoded in the simulation as constant expressions.

```bash
./heat_1d_PTH5.out <active_points> <iterations> <threads>
```

### 2D Heat Transfer

This is a similar stencil computation to simulate the transfer of heat from a set of boundary conditions on the edge of a matrix by averaging each of the four points adjacent to any point in the temperature matrix. The number of threads to use can be modified seperately by using the environment variable. The initial conditions are hardcoded constant expressions.

```bash
# optionally pre-set the number of threads to use
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

### Add Numbers

These programs were provided in full by the instructor as examples of how to use MPI. Their arguments are as follows.

```bash
mpiexec -np <processor_count> ./add_num_MPI.out <number_of_numbers_to_add>
```

### Histogram

These programs generate a set of random numbers and sort them into histogram bins on each process which are then reduced back together on the root MPI process. The arguments are as follows, with a `1` suppressing the output.

```bash
mpiexec -np <num_processes> ./histogram_MPI.out <list_size> <bins> <uplim> <lowlim> [1]
```

## Dependencies and Setup for UNIX-Style Systems

Since these are making use of advanced features of C++ compilers and libraries that extend them, it is important that you follow the subsequent steps carefully to ensure your environment is properly configured.

>[!Tip]
>If you are connecting to ASAX, it is already set up and ready to go. You don't have to do anything! Skip to the section on building with makefiles.
### Linux

You need to install several packages from your distribution's package manager (YUM, pacman, apt, etc.). To get this working on Ubuntu 26.04 LTS, I ran the following:

```bash
sudo apt update && sudo apt upgrade
sudo apt install build-essential gdb openmpi-bin openmpi-common libopenmpi-dev
```

>[!Note]
>This list of packages will vary widely form one distribution to another, but something generally similar should be pretty easy to find.

### Mac OS X

Install Xcode or the Xcode Command Line Tools version 16 or later, then run the following in [Homebrew](https://brew.sh) to install missing dependencies that Apple does not ship.

```zsh
brew install libomp
brew install openmpi
```

The optional `pthread_barrier_t` type from the POSIX standard is not provided on macOS, so on this platform the code in `./pthread_barrier_macos` is built into a static library that is linked into each executable which requires it.

### MSYS2 on Windows

While it is not a full UNIX environment, it is sufficient to build and run all the examples presented in this repository.
If you are unfamiliar with this great project, read about it on the [MSYS2 website](https://msys2.org).

To get started, install `MSYS2` from the Windows Package Manager (requires elevation).

```powershell
winget install MSYS2.MSYS2
```

Once that is done, open the `UCRT64` shell (**not PowerShell or Command Prompt**) from the start menu or in your Windows Terminal and update before pulling the necessary packages:

```bash
# Update and upgrade your system before continuing
pacman -Syu

# Install the necessary packages
pacman -S base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-msmpi
```

### Building on UNIX-Style Systems

The build system for this repository is simply a set of makefiles, as each program is born of a self-contained source file.

On any of the above platforms, you can invoke `make` in the root folder or in any of the subfolders that contain project code. If you set everything up correctly, it should work seamlessly.

It uses a `toolchains.mk` file to automatically resolve the OS, apply compatibility patches, and set up processor-specific definitions along with build flags. Most of the targets are built from the pattern rules it contains, making individual project makefiles very short and sweet (check them out if you're curious).

```bash
# build everything in the repository
make

# build everything in parallel on 4 threads, continuing on an error
make -j 4 -k

# remove build artifacts
make clean

# run make in just a subdirectory to build its code
cd hello_world
make

# or target it from the repository root
make hello_world

# go crazy with portable executable files (not recommended)
# they must begin with the literal shebang: #!/usr/bin/env make
chmod +x makefile
./makefile
```

### A note on MPI C++ bindings

Apparently, the C++ bindings we are learning to use have been deprecated so long that they have been removed from most implementations in the wild today. The old `mpich` implementation installed on ASAX still has it, though, and can be used instead by loading the module first if you wish to manually compile code that uses it.

```bash
module load mpich
```

>[!Warning]
>Code using this will likely fail to build in the future, and I will not take up the maintenance burden relieved from major software vendors by this removal.

By default, each `makefile` will **not** compile examples that rely on these obsolete C++ bindings. To build them anyways, pass an additional argument to specify that it is being built on ASAX (and you know that you are intentionally using obsolete features).

```bash
make ASAX=1
```

## Using Microsoft Visual Studio on Windows

>[!Warning]
>Using MSVC is not recommended because most of these programs assume a UNIX-like system and POSIX threads are completely absent and unsupported in Visual Studio. [MSVC support for OpenMP](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support) is limited to the 2.0 standard from 2002. You can get a little further with `/openmp:llvm` for some (experimental!) support for more modern features, though none of the code in this repository needs it.

If you want to try it, install the latest version of Visual Studio with the "C++ Development Workload" and the "Clang tools for Visual Studio" optional component.

Install the [Microsoft MPI](https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi) implementation runtime and Software Development Kit (SDK) from the Windows Package Manager (requires elevation).

```powershell
winget install Microsoft.msmpi
winget install Microsoft.msmpisdk
```

### Building the Code

On Windows, simply run the PowerShell script from the root of the repository (adding the `clean` argument to remove build artifacts) or in any of the sub-folders to build just that folder.

>[!Note]
>This script **must** be invoked from a Developer Command Prompt with the appropriate Visual Studio environment variables set or else it will fail!

```powershell
# build everything that MSVC understands (i.e. not pthreads-dependent binaries)
.\winbuild.ps1

# clean build artifacts throughout the repository
.\winbuild.ps1 clean

# build only code in the hello_world folder
cd hello_world
.\winbuild.ps1
```

The individual commands to compile your code can be quite and cumbersome to use:

```powershell
# set up by moving to hello_world example code
cd hello_world

# serial code (not so bad)
cl /EHsc /std:c++20 .\hello_world.cpp /Zc:preprocessor

# can't compile pthreads code or combo OpenMP/pthreads code at all

# for OpenMP
cl /EHsc /std:c++20 .\hello_world_OMP.cpp /Zc:preprocessor /openmp

# for MPI, assuming you installed MSMPI and its SDK properly
# (change 64 --> 32 if using a 32-bit system)
cl.exe /EHsc /std:c++20 /I$env:MSMPI_INC /Fe:hello_world_MPI.exe hello_world_MPI.cpp /link /LIBPATH:$env:MSMPI_LIB64 msmpi.lib
```
