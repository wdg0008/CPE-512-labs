// 2-D temperature Example  -- Single-Threaded Program
/*
To compile on the Jetson Cluster
   GNU Compiler
      g++ heat_2d_serial.cpp -o heat_2d_serial -Ofast

To execute on the Jetson Cluster 
   GNU Compiler
      run_script heat_2d_serial.sh
      where heat_2d_serial.sh is a script file that contains
         #!/bin/bash
         ./heat_2d_serial 10 100 
         # execute a 10 x 10 point 2d-heat transfer problem 
         # for 10 iterations 
*/

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <omp.h>
#include <memory>
using namespace std;

#define TIMER_CLEAR  (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   (double) (tv2.tv_usec- tv1.tv_usec)/1000000.0+(tv2.tv_sec-tv1.tv_sec)
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;

template <typename T>
class MatrixView {
public:
    MatrixView(T* data, int columns)
        : data_(data), columns_(columns) {}

    T& operator()(int row, int col) {
        return data_[row * columns_ + col];
    }

    const T& operator()(int row, int col) const {
        return data_[row * columns_ + col];
    }

private:
    T* data_;
    int columns_;
};


// Global Constants
const int ROOM_TEMP=20;       // temperature everywhere except the fireplace
const int FIREPLACE_TEMP=100; // temperature at upper/right boundary

int n;                // number of non boundary condition rows in problem 
int num_iterations;   // number of successive iterations before terminating

int total_rows;       // total number of rows including 
                      // including boundary condition rows

int total_cols;       // total number of columns including
                      // boundary condition columns

unique_ptr<double[]> temp;     // shared temperature, row-ordered storage
unique_ptr<double[]> temp_buf; // next iteration, row-ordered storage

// routine to initialize the temperature vector and the temperature at the
// boundary
void init_temp(MatrixView<double>& temp_view) {
    const int fireplace_start = 0.3 * (double) n;
    const int fireplace_end = 0.7 * (double) n;

    // Set leftmost boundary condition on process 0
    for (int row=0;row < total_rows; row++) {
        for (int col=0;col<total_cols;col++) {
            if (row == 0) {
                if (col<=fireplace_start || col > fireplace_end) {
                    temp_view(row, col) = ROOM_TEMP;
                } else {
                    temp_view(row, col) = FIREPLACE_TEMP;
                }
            } else {
                temp_view(row, col) = ROOM_TEMP;
            }
        }
    }
}
void compute_temp(MatrixView<double>& temp_view,
                  MatrixView<double>& temp_buf_view) {
    #pragma omp parallel
    {
        for (int i = 0; i < num_iterations; i++) {
            // magically divide the loop indices evenly w/ remainder
            #pragma omp for schedule(static)
            for (int row = 1; row <= n; row++) {
                for (int col = 1; col <= n; col++) {
                    temp_buf_view(row, col) = 0.25
                        * (temp_view(row - 1, col)
                        +  temp_view(row + 1, col)
                        +  temp_view(row, col - 1)
                        +  temp_view(row, col + 1));
                }
            }

            // OMP provides contiguous, balanced blocks with spread remainder
            #pragma omp for schedule(static)
            for (int row = 1; row <= n; row++) {
                for (int col = 1; col <= n; col++) {
                    temp_view(row, col) = temp_buf_view(row, col);
                }
            }
        }
    }
}
// routine to display temperature values at each point including the 
// boundary points
void print_temp(const MatrixView<double>& temp_view) {
    cout << "Temperature Matrix Including Boundary Points" << endl;
    for (int row=0;row<total_rows;row++) {
        for (int col=0;col<total_cols;col++) {
            cout << setw(5) << temp_view(row, col) << " ";
        }
        cout << endl << flush;
    }
}
// Routine that performs a simple 64 integer checksum
// of the binary contents of the final Temp array
// This is used to perform a quick comparison of the
// results to insure that modifications to the original
// program did not affect the accuracy of the computation
unsigned long long int checksum(const MatrixView<double>& temp_view) {
    void *ptr;
    unsigned long long int sum = 0;
    for (int row=0;row<total_rows;row++) {
        for (int col=0;col<total_cols;col++) {
            ptr=(void *) &temp_view(row, col);
            sum += *(unsigned long long int *) ptr;
        }
    }
    return sum;
} 

void print_usage_instructions(char *command) {
    cout << "Usage: " << command << 
        " [Dim n = num row/cols square matrices] [Number Iterations] " << 
        "<x>" << endl <<
        "   where optional argument x = " << endl <<
        "       H suppress output -- print n,runtime and" << endl <<
        "                            checksum in human readable format" << endl <<
        "       S suppress output -- print checkSum" << endl <<
        "       C suppress output -- print n,runtime" << endl <<
        "                            in CSV format" << endl <<
        "       G suppress output -- print n,runtime" << endl <<
        "                            in gnuplot format" << endl;

        exit(1);
}

int main (int argc, char *argv[]){

    if (argc!=3 && argc!=4) {
        print_usage_instructions(argv[0]);
    }

    // get total number of points not counting boundary points
    // from first command line argument 
    // Warning No Error Checking 
    n = atoi(argv[1]);

    // get total number of iterations to run simulation
    // Warning No Error Checking
    num_iterations = atoi(argv[2]);

    // set total columns plus boundary points
    total_rows = n+2; // total rows plus boundary points

    // set total columns plus boundary points
    total_cols = n+2; // total columns plus boundary points

    // dynamically allocate shared memory to
    // temp and temp_buf arrays
    temp = make_unique<double[]>(total_rows * total_cols);
    temp_buf = make_unique<double[]>(total_rows * total_cols);

    MatrixView<double> temp_view(temp.get(), total_cols);
    MatrixView<double> temp_buf_view(temp_buf.get(), total_cols);

    // initialize temperature matrix
    init_temp(temp_view);
    // begin timer
    TIMER_CLEAR;
    TIMER_START;

    // compute new temps
    compute_temp(temp_view, temp_buf_view);
    // stop timer
    TIMER_STOP;

    // print out the results if there is no suppress output argument
    if (argc==3) {
        print_temp(temp_view); // print out the results
        cout << "Execution Time = " << TIMER_ELAPSED << " Seconds"
             << endl;
        cout << "64 bit Checksum = " << checksum(temp_view) << endl;
    }
    // if there exists a 4th argument, then suppress the output
    else {
        // print time in gnuplot format
        if (*argv[3]=='G') {
            cout << n << " " << TIMER_ELAPSED << endl;
        }
        // print time in CSV format 
        else if (*argv[3]=='C') {
            cout << n << "," << TIMER_ELAPSED << endl;
        }
        // print 64 bit checkSum
        else if (*argv[3]=='S') {
            cout << "64 bit Checksum = " << checksum(temp_view) << endl;
        }
        else if (*argv[3]=='H') {
           // print time and Checksum in normal human readable format
           cout << "Number of active data points =" << n << endl;
           cout << "Execution Time = " << TIMER_ELAPSED << " Seconds"
                << endl;
            cout << "64 bit Checksum = " << checksum(temp_view) << endl;
        }
    }

}
