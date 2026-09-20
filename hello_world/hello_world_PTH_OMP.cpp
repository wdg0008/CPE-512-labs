/*
  Hybrid Pthreads/OpenMP Example - Hello World - C/C++ Version
File: hello_world_PTH_OMP.cpp
Compilation on the ASA ASAX System 
Note1: normally you would not every combine the two but this was created
       to illustrate how the OpenMP constructs generate threads in the
       linux OS environment.

       Set to generate pthreads -- uncomment omp related statements
       to allow for a hybrid implementation where the number of pthreads
       that are generated being set to the value expressed by environment 
       variable PTH_NUM_THREADS+1 and the number of OpenMP generated threads
       being set to the value of the environment varible OMP_NUM_THREADS.
Note2: two while(1) infinite loop statements are comment out. If they are
       uncommented the executable will cause all threads that are generated
       to persist in a CPU intensive busy wait state. This allows their 
       existance to be monitored by the linux top command with specific
       thread ids being accessiable through the
           top -H -b -p <process id> 
       variant of the top command. Note that the process id and the thread id
       of the initial thread of the process at least in linux are identical.  

     To compile this program using the GNU Compiler use both the -lpthread
     and -fopenmp flags.

     g++ hello_world_PTH_OMP.cpp -o hello_world_PTH_OMP -lpthread -fopenmp

*/

using namespace std;
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // needed for pthreads
//#include <omp.h>     // needed if OpenMP API is used
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

pthread_mutex_t MUTEX; // globaly defined lock variable used to insure
                       // uninterrupted print operations from each thread

// This is the worker thread code
void *hello(void * arg) {
   int myid=*(int *) arg;
   pid_t ostid = syscall(SYS_gettid);

   // enter critical region
   pthread_mutex_lock(&MUTEX);

   cout << "Hello World, from PThread " << myid << " (" << ostid << ")" << endl;
   // exit critical region
   pthread_mutex_unlock(&MUTEX);
   // while(1); // Infinite loop
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

   pid_t ostid = syscall(SYS_gettid); //gettid();
   cout << endl;
   cout << "Hello World from OSthread = " << ostid << endl << flush; 
   // get total number of threads to be launched
   // if there is a command line parameter use it to set number of threads
   if (argc>1) {
       nthreads=atoi(argv[1]); // get number of threads from command line 
   }
           // if there is no command line parameter check to see if
   else {  // OMP_NUM_THREADS is set -- if set use this for num threads
      char *env_var;
      if ((env_var=getenv("PTH_NUM_THREADS"))!=NULL) {
         nthreads=atoi(env_var);
      }
      else if ((env_var=getenv("OMP_NUM_THREADS"))!=NULL) {
         nthreads=atoi(env_var);
      }
   }

   // dynamically allocate threads[nthreads] on heap fully C++ compliant manner
   pthread_t *threads = new pthread_t [nthreads]; 

   // dynamically allocate ids[nthreads] on heap fully C++ compliant manner
   int *ids = new int [nthreads];

   // initialize mutex variable -- this variable is used to insure that
   // all couts are automic meaning that they are not interrupted
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

/* // Uncomment to incorporate the OpenMP component after
   // the ptheads threads have been generated

   // Fork a team of threads giving them their own
   // copy of private variable, tid, thread id
   int omp_nthreads;
   #pragma omp parallel
   {
      int tid; // private (local) variable thread id, tid
               // declared within the basic block
      pid_t ostid = syscall(SYS_gettid); //gettid();
      // Obtain thread number
      tid = omp_get_thread_num();

      #pragma omp master
      #pragma omp critical
      {
         cout << "Hello World from OMPthread = " << tid << 
             " (OSthread=" << ostid << ")" 
              << endl <<flush;
      }
      // print out Hello World Message -- do this as a noninterruptable op
      #pragma omp critical
      {
         cout << "Hello World from OMPthread = " << tid << 
             " (OSthread=" << ostid << ")" 
              << endl <<flush;
      }
      // Thread 0 determines number of threads in group and stores the
      // result in the shared variable, nthreads
      if (tid == 0) {
         omp_nthreads = omp_get_num_threads();
      }
      // while(1); // Infinite loop
   }  // All OMP threads join master thread and disband
*/
   // wait for all pthreads to return before exiting main thread 
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

   // print out number of threads
   pthread_mutex_lock(&MUTEX); // enter critical region
   cout << "Number of Pthreads = " << nthreads << endl;
//   cout << "Number of OpenMP threads = " << omp_nthreads << endl;
   cout << "Program compiled using "
       #ifdef __PGI
          << "NVIDA HPC compiler" 
       #elif __INTEL_COMPILER 
          << "Intel compiler" 
       #elif __GNUC__
          << "GNU compiler" 
       #endif
       << endl << flush;

   pthread_mutex_unlock(&MUTEX); // exit critical region
   delete threads;
   delete ids; 
   return(0);
}

