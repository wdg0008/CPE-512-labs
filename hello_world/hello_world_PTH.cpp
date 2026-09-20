/*
  Pthreads Example - Hello World - C/C++ Version
File: hello_world_PTH.cpp
  Compilation on ascx.asc.edu

     Using GNU Compiler
     module load gcc
     g++ hello_world_PTH.cpp -o hello_world_PTH -lpthread

     Using Intel Compiler
     module load intel
     icpc hello_world_PTH.cpp -o hello_world_PTH_intel -lpthread

     Using NVIDIA hpc Compiler
     module load pgi
     pgc++ hello_world_PTH.cpp -o hello_world_PTH_hpc -lpthread

*/

using namespace std;
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t MUTEX; // globaly defined lock variable used to ensure
                       // uninterrupted print operations from each thread

// This is the worker thread code
void *hello(void * arg) {
   int myid=*(int *) arg;

   // enter critical region
   pthread_mutex_lock(&MUTEX);

   cout << "Hello World, from PThread " << myid << endl;

   // exit critical region
   pthread_mutex_unlock(&MUTEX);

  return arg;
}

// this is the main thread's code -- it spawns the worker threads and
// then waits for all the worker threads to return before it exits
int main(int argc,char *argv[])
{
   int nthreads=1;                // total number of threads to be launched
                                  //  default nthreads=1
   int tid;                       // tid thread id number
   int errcode;                   // pthread error code

   // get total number of threads to be launched
   // if there is a command line parameter use it to set number of threads
   if (argc==2) {
       nthreads=atoi(argv[1]); // get number of threads from command line 
   }
           // if there is no command line parameter check to see if
   else {  // OMP_NUM_THREADS is set -- if set use this for num threads
      char *env_var;
      if ((env_var=getenv("OMP_NUM_THREADS"))!=NULL) {
         nthreads=atoi(env_var);
      }
      else if ((env_var=getenv("PTH_NUM_THREADS"))!=NULL) {
         nthreads=atoi(env_var);
      }
      else {
         nthreads=1; 
      }  
   }

   // dynamically allocate threads[nthreads] on heap fully C++ compliant manner
   pthread_t *threads = new pthread_t [nthreads]; 

   // dynamically allocate ids[nthreads] on heap fully C++ compliant manner
   int *ids = new int [nthreads];

   // initialize mutex variable -- this variable is used to ensure that
   // all couts are atomic meaning that they are not interrupted
   pthread_mutex_init(&MUTEX,NULL);

   /* create the threads */
   for (tid=0; tid<nthreads; tid++) {
      ids[tid]=tid;
      errcode=pthread_create(
              &threads[tid],// thread information structure
              NULL,         // thread attributes -- NULL means assume defaults 
              hello,        // function name that is to represent thread
              &ids[tid]);  // pthread created thread id for the created thread        
      // check for error during thread creation
      if (errcode) {
         cerr << "Pthread Creation Error: " << strerror(errcode) << endl; 
         exit(1);
      }
   }

   // print out number of threads
   pthread_mutex_lock(&MUTEX); // enter critical region
   cout << "Number of threads = " << nthreads << endl
       << "Program compiled using "
       #ifdef __PGI
          << "NVIDA HPC compiler" 
       #elif __INTEL_COMPILER 
          << "Intel compiler" 
       #elif __GNUC__
          << "GNU compiler" 
       #endif
       << endl << flush;

   pthread_mutex_unlock(&MUTEX); // exit critical region

   // wait for all threads to return before exiting main program (process)
   for (tid=0; tid<nthreads; tid++) {
      // wait for each thread to terminate 
      errcode=pthread_join(
              threads[tid], //thread scheduler id information of selected thread
              NULL);        //thread return value -- not used in this case
      if (errcode) {
         cerr << "Pthread Join Error: " << strerror(errcode) << endl; 
         exit(1);
      }
   }
   delete[] threads;
   delete[] ids; 
   return(0);
}
