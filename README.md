# README

This repository contains code written for [CPE 512 - Intro to Parallel Programming](https://catalog.uah.edu/#/courses/6732c244744e35849f2c6ea9) at [The University of Alabama in Huntsville](https://uah.edu).

It was originally developed for the ASAX supercomputer operated by the [Alabama Supercomputer Authority](https://asc.edu/) which runs Linux with a [PBS job-queueing system](https://en.wikipedia.org/wiki/Portable_Batch_System) to schedule execution of jobs written as bash scripts. These scripts contain examples of how to execute the various programs.

There are three main technologies/frameworks used to parallelize constructs in this code:

1. [POSIX Threads (pthreads)](https://pubs.opengroup.org/onlinepubs/9799919799/)
2. [OpenMP](https://www.openmp.org/)
3. [Message Passing Interface (MPI)](https://www.mpi-forum.org/)

It is organized with subfolders for each unit in the class, with most of the code based on serial versions originally provided by the instructor.

## Setup

### Linux

You need to install `gcc` version 12 or later from your package manager, along with a few other things.
**This list is definitely NOT comprehensive, and will need to be updated.**

### Mac OS X

Install Xcode or the Xcode Command Line Tools version 16 or later, then run the following in [Homebrew](https://brew.sh) to install missing dependencies that Apple does not ship.

```zsh
brew install libomp
brew install openmpi
```

### Windows

Install `gcc` from [MSYS2](https://msys2.org) to use the same compiler options and build code that uses pthreads.

If you want to try it, install the latest version of Visual Studio with the "C++ Development Workload" and the "Clang tools for Visual Studio" optional component. It cannot do pthread and only has old, outdated OpenMP support (which is sufficient for most of this code).

## Building

The build process depends on which operating system you use and what framework your code is using. The goal is to run a single makefile for everything, but that's not implemented yet.

### Linux

Currently, you can use makefiles for each unit on ASAX but have to build everything yourself otherwise. Adapting for general Linux should be easy.

### Mac OS X

The makefile is being rewritten to work on macs, but that is still in-progress.

### Windows

The easiest way to get started is to use `gcc` like before. It hasn't been implemented yet, but this would ideally allow for a single makefile to detect your OS and set appropriate variables/flags with some imports. This is a low priority and won't be added until after OS X builds are working, as it will be much harder.

[MSVC support for OpenMP](https://learn.microsoft.com/en-us/cpp/build/reference/openmp-enable-openmp-2-0-support) is limited to the 2.0 standard from 2002, and you can get a little further with `/openmp:llvm` for some (experimental!) support for more modern features.

```powershell
cl .\hello_world_OMP.cpp /EHsc /std:c++20 /Zc:preprocessor /openmp
```

You can install the [Microsoft MPI](https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi) implementation with the following command in Windows 11:

```powershell
winget install Microsoft.msmpi
```

Hasn't been tested yet and requires some additional setup. The MSMPI project seems dead, with only one patch release in the last 5 years, but it isn't makred deprecated--yet.
