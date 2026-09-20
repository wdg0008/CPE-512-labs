// 1-D temperature Example  -- Coarser-grain pthread Implementation
//                             (nthreads < active_threads)
//                             Cyclic Distribution of Active points
//                             Two Thread Generation Phases per Iteration
// g++ heat_1d_PTH2.cpp -o heat_1d_PTH2 -lpthread
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

/* copied from mpbench */
#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   (double) (tv2.tv_usec-tv1.tv_usec)/1000000.0+(tv2.tv_sec-tv1.tv_sec)
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;

#define LEFT_TEMP 100  // temperature at lower/left boundary
#define RIGHT_TEMP -40 // temperature at upper/right boundary
#define MID_TEMP    0 // initial temperature at the other points

// routine to initialize the temperature vector and the temperature at the
// boundary
void init_temp(double *temp, int active_points) {

    // Set leftmost boundary condition
    temp[0]=LEFT_TEMP;

    // set rightmost boundary condition on last process
    temp[active_points+1]=RIGHT_TEMP;

    // set initial conditions on all non boundary or ghost points
    for (int i=1;i<=active_points;i++) {temp[i]=MID_TEMP;};

}
// Shared pointer variables and other variables
double *temp,*temp_buf;
int nthreads,active_points;

//  new set of elements -- put in temp_buf array
//  process every nthread elements per thread
void *thread1(void * arg) {
    int start=*(int *) arg;
    for (int j=start; j<=active_points; j += nthreads) {
       temp_buf[j]=0.5*(temp[j-1]+temp[j+1]);
    }
    pthread_exit(0);
}

// copy element from temp_buf array to temp array
//  copy every nthread elements per thread
void *thread2(void * arg) {
    int start=*(int *) arg;
    for (int j=start; j<=active_points; j += nthreads) {
       temp[j]=temp_buf[j];
    }
    pthread_exit(0);
}

void compute_temp(double * temp, int active_points, int num_iterations) {

    // allocate local storage for temp buffer 
    temp_buf = new double [active_points + 2];

    // dynamically allocate threads[nthreads] on heap
    pthread_t *threads = new pthread_t [nthreads]; 

    // dynamically allocate ids[nthreads] on heap 
    int *ids = new int [nthreads];

    // load logical id structure array -- start numbering at 1
    for (int tid=0; tid<nthreads; tid++) {
       ids[tid]=tid+1;
    }
    for (int i=0;i<num_iterations;i++) {
        // average points -- do not update boundary conditions 
        // -- store in separate temp_buf array

        // COMPUTE NEW ELEMENT PHASE
        // Thread Create SUBPHASE -- nthreads created 
        //   [~(active points/nthreads) elements processed per thread]
        for (int tid=0; tid<nthreads; tid++) {
            int errcode=pthread_create(
                    &threads[tid],// thread information structure
                    NULL,         // thread attributes - NULL-- assume defaults 
                    thread1,      // function name that is to represent thread
                    &ids[tid]);   // points to logical thread id of thread        
            // check for error during thread creation
            if (errcode) {
                cerr << "Pthread Creation Error: " << strerror(errcode) << endl; 
                exit(1);
            }
        }
        // Thread JOIN SUBPHASE
        // wait for all threads to return before exiting main program (process)
        for (int tid=0; tid<nthreads; tid++) {
            // wait for each thread to terminate 
            int errcode=pthread_join(
                    threads[tid], //pthread id information of selected thread
                    NULL);        //thread return value -not used in this case
            if (errcode) {
                cerr << "Pthread Join Error: " << strerror(errcode) << endl; 
                exit(1);
            }
        }

        // COPY NEW ELEMENTS PHASE
        // Thread Create SUBPHASE -- nthreads created 
        //   [~(active points/nthreads) elements copied per thread]
        for (int tid=0; tid<nthreads; tid++) {
            int errcode=pthread_create(
                    &threads[tid],// thread information structure
                    NULL,         // thread attributes - NULL-- assume defaults 
                    thread2,      // function name that is to represent thread
                    &ids[tid]);   // points to logical thread id of thread        
            // check for error during thread creation
            if (errcode) {
                cerr << "Pthread Creation Error: " << strerror(errcode) << endl; 
                exit(1);
            }
        }
        // Thread JOIN SUBPHASE
        // wait for all threads to return before exiting main program (process)
        for (int tid=0; tid<nthreads; tid++) {
            // wait for each thread to terminate 
            int errcode=pthread_join(
                    threads[tid], //pthread id information of selected thread
                    NULL);        //thread return value -not used in this case
            if (errcode) {
                cerr << "Pthread Join Error: " << strerror(errcode) << endl; 
                exit(1);
            }
        }

    }
    delete temp_buf;
}

// routine to display temperature values at each point including the 
// boundary points
void print_temp(double * temp, int active_points, int num_iterations) {
    // print header and the left-most boundary point
    cout << "Temperature at Iteration " << num_iterations << endl;

    // Print out all temperature point, including boundary points
    for (int i=0;i<active_points+2;i++) {
        // print point index and temp of each point
        cout << setw(5) << i << " " << setw(5) << temp[i] << endl;
    }

} 
// Routine that performs a simple 64 integer checksum
// of the binary contents of the final temp array
// This is used to perform a quick comparison of the
// results to insure that modifications to the original
// program did not effect the accuracy of the computation
unsigned long long int checksum(double *temp,int active_points) {
    void *ptr;
    unsigned long long int sum = 0;
    for (int i=0;i<active_points;i++) {
        ptr=(void *) &temp[i];
        sum += *(unsigned long long int *) ptr;
    }
    return sum;
}
void print_usage_instructions(char *command) {
    cout << "Usage: " << command << 
        " [Number Active Data Points] [Number Iterations] " << 
        "[Number Threads] <x>" << endl <<
        "   where optional argument x = " << endl <<
        "       H suppress output -- print active_points,runtime and" << endl <<
        "                            checksum in human readable format" << endl <<
        "       S suppress output -- print checkSum" << endl <<
        "       C suppress output -- print active_points,runtime" << endl <<
        "                            in CSV format" << endl <<
        "       G suppress output -- print active_points,runtime" << endl <<
        "                            in gnuplot format" << endl;

        exit(1);
}

int main (int argc, char *argv[]){

    if (argc!=4 && argc!=5) {
        print_usage_instructions(argv[0]);
    }

    // get total number of points not counting boundary points 
    active_points = atoi(argv[1]);

    // get total number of iterations to run simulation
    int num_iterations = atoi(argv[2]);

    // get total number of worker threads
    nthreads = atoi(argv[3]);

    // dynamically allocate memory to store temp array
    temp = new double [active_points+2]; 

    init_temp(temp,active_points); // initialize temperature matrix

    // Start recording the execution time
    TIMER_CLEAR;
    TIMER_START;

    compute_temp(temp,active_points,num_iterations); // compute new temps

    //  stop recording the execution time
    TIMER_STOP;

    // print out the results if there is no suppress output argument
    if (argc==4) {
        print_temp(temp,active_points,num_iterations); // print out the results
        // print time in normal human readable format
        cout << "Execution Time = " << TIMER_ELAPSED << " Seconds"
             << endl;
        cout << "64 bit Checksum = " << checksum(temp,active_points) << endl;
    }
    else {
        // print time in gnuplot format
        if (*argv[4]=='G') {
            cout << active_points << " " << TIMER_ELAPSED << endl;
        }
        // print time in CSV format 
        else if (*argv[4]=='C') {
            cout << active_points << "," << TIMER_ELAPSED << endl;
        }
        // print 64 bit checkSum
        else if (*argv[4]=='S') {
            cout << "64 bit Checksum = " << checksum(temp,active_points) << endl;
        }
        else if (*argv[4]=='H') {
           // print time and Checksum in normal human readable format
           cout << "Number of active data points =" << active_points << endl;
           cout << "Execution Time = " << TIMER_ELAPSED << " Seconds"
                << endl;
            cout << "64 bit Checksum = " << checksum(temp,active_points) << endl;
        }
    }

    delete temp;

}
