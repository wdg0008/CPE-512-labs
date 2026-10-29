/* Simple Hello World Program C++ Version
FILE: hello_world.cpp

Compilation on ascx.asc.edu

   to compile the program

     Using GNU Compiler
     module load gcc
     g++ hello_world.cpp -o hello_world

     Using Intel Compiler
     module load intel
     icpx hello_world.cpp -o hello_world_intel

     Using NVIDIA hpc Compiler
     module load pgi
     pgc++ hello_world.cpp -o hello_world_hpc
*/

#include <iostream>

using namespace std;

int main (int argc, char *argv[]) {
   #ifdef __PGI
      // Compiled with the NVIDA HPC Compiler
      cout << "Hello world from NVIDA HPC compiler" << endl << flush;
   #elif  __INTEL_LLVM_COMPILER
      // Compiled with Intel Compiler
      cout << "Hello world from Intel LLVM compiler" << endl << flush;
   #elif __GNUC__
      // Compiled with GNU Compiler
      cout << "Hello world from GNU compiler" << endl << flush;
   #endif
}
