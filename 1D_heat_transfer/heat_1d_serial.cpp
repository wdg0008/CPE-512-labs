// 1-D temperature Example  -- Single-Threaded Program
// compilation
// g++ heat_1d_serial.cpp -o heat_1d_serial 
using namespace std;
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sys/time.h>

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

void compute_temp(double * temp, int active_points, int num_iterations) {

    // allocate local storage for temp buffer 
    double *temp_buf = new double [active_points + 2];

    for (int i=0;i<num_iterations;i++) {
        {
            // average points -- do not update boundary conditions 
            // -- store in separate temp_buf array
            for (int j=1;j<=active_points;j++) {
                temp_buf[j]=0.5*(temp[j-1]+temp[j+1]);
            }
            // set current temperatures to new tempertures for next iteration
            for (int j=1;j<=active_points;j++) {
                temp[j]=temp_buf[j];
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
        "<x>" << endl <<
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

    if (argc!=3 && argc!=4) {
        print_usage_instructions(argv[0]);
    }

    // get total number of points not counting boundary points 
    int active_points = atoi(argv[1]);

    // get total number of iterations to run simulation
    int num_iterations = atoi(argv[2]);

    // dynamically allocate memory to store temp array
    double *temp = new double [active_points+2]; 

    init_temp(temp,active_points); // initialize temperature matrix

    // Start recording the execution time
    TIMER_CLEAR;
    TIMER_START;

    compute_temp(temp,active_points,num_iterations); // compute new temps

    //  stop recording the execution time
    TIMER_STOP;

    // print out the results if there is no suppress output argument
    if (argc==3) {
        print_temp(temp,active_points,num_iterations); // print out the results
        // print time in normal human readable format
        cout << "Execution Time = " << TIMER_ELAPSED << " Seconds"
             << endl;
        cout << "64 bit Checksum = " << checksum(temp,active_points) << endl;
    }
    else {
        // print time in gnuplot format
        if (*argv[3]=='G') {
            cout << active_points << " " << TIMER_ELAPSED << endl;
        }
        // print time in CSV format 
        else if (*argv[3]=='C') {
            cout << active_points << "," << TIMER_ELAPSED << endl;
        }
        // print 64 bit checkSum
        else if (*argv[3]=='S') {
            cout << "64 bit Checksum = " << checksum(temp,active_points) << endl;
        }
        else if (*argv[3]=='H') {
           // print time and Checksum in normal human readable format
           cout << "Number of active data points =" << active_points << endl;
           cout << "Execution Time = " << TIMER_ELAPSED << " Seconds"
                << endl;
            cout << "64 bit Checksum = " << checksum(temp,active_points) << endl;
        }
    }

    delete temp;

    return (0);
}
