/**************************************************************/
/* Histogram construction Program -- MPI version              */
/* September 2023 -- B. Earl Wells -- University of Alabama   */
/* (C MPI bindings)                  in Huntsville            */
/**************************************************************/
/*
This program illustrates the basic concepts of SPMD programming using
MPI.  The program represents a common example that is used often in
the CPE 412/512 text, creating a histogram of binned numbers.
The program is written in such a way that it is assumed that the
sequence of numbers is first read from a central source (in this case
a data file) by a single MPI process (the root process) and then
partitioned into equal groups with each part being distributed
(scattered) to the local memory domains of the other MPI
processes in the system. Each MPI process then locally computes its
own histogram of the data for the data in its own domain. This is
followed by a a coordinated communication/computation operation
where the local histogram information is transferred to the
root MPI process where it is combined into the final histogram
by using element-wise addition (i.e. a vector reduce operation).
The histogram output is then placed by the root process in a file
which is named histogram.txt

The following is a simplified version of the program which you will
be asked to augment in a number of ways in future homework
assignments. Use the dmc.asc.edu or the Jetson Cluster system for
these assignments.
Notes: This implementation utilizes a minimum set of MPI methods
       call that include Init, Finalize, Comm_size,
       Comm_rank, Send, and Recv.
       Abort is also used to illustrate its functionality.

  Compilation on dmc.asc.edu

   GNU Compiler
   mpic++ histogram_MPI.cpp -o histogram_MPI

To execute:

   GNU compiler
   use run_script_mpi histogram_MPI.sh

*/

#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <random>
#include <exception>
#include <mpi.h> /* MPI Prototype Header Files */

using namespace std;

/*
Function used by get_parameters routine to prompt the user to
interactively enter the next parameter and to return it to the
calling routine
*/
template <class T>
T enter_next_param(const std::string prompt) {
   T num;
   while (1) {
      string input = "";
      cout << prompt << endl;
      getline(cin, input);
      stringstream myStream(input);
      if (myStream >> num) break; // feels evil, how does this even work?
      cout << "Invalid Input" << endl << endl;
   }
   return num;
}

/*
Routine to retrieve the run time parameters which include
1) the number of numbers in the number list,
2) number of equally spaced bins to be used by the histogram,
3) maximum value in bin range, 4) minimum value in bin range.
If there is a 5th run time parameter then outputing of the numbers
array that was generated is suppressed
If no parameters are supplied on the command line, then the program
interactively prompts the user for the information.
*/
void get_parameters(int *list_size,int *num_bins,double *max_bin_val,
   double *min_bin_val,bool *num_list_out, int argc,char *argv[],
   int rank,int numprocs) {

   if(argc==1) {
      // One Command Line Argument Case:
      // case where user did not enter number of numbers on command line
      // In this case, only one of the MPI processes needs to communicate
      // directly with the user. Since there will always be a MPI process
      // with rank 0 this is the one that will perform the
      // human interface communication.
      if (rank==0) {
         *list_size=enter_next_param<int>("Enter number of numbers in list:");
         *num_bins=enter_next_param<int>("Enter number of bins in histogram:");
         *max_bin_val=enter_next_param<double>("Enter Upper Range of histogram:");
         *min_bin_val=enter_next_param<double>("Enter Lower Range of histogram:");
         char print_out=enter_next_param<char>("Suppress numbers array printing (y or n):");
         if (print_out=='y' || print_out=='Y') *num_list_out=false;
      }
      // since only the rank=0 MPI process is communicating with the
      // user, this process must send its value to all of the other MPI processes.
      // Uses built-in broadcast library function to send a scalar (count = 1)
      // Sent to all MPI processes since MPI_COMM_WORLD is universal
      MPI_Bcast(list_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast(num_bins, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast(max_bin_val, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(min_bin_val, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   }
   else if (argc==5 || argc==6) {
      // Two Command Line Argument case:
      // user supplied the number of numbers on the command line.
      // Each MPI process can retrieve it from there. No need to
      // broadcast it to the other processes because each have it at
      // run time.
      *list_size = atoi(argv[1]); // get list_size from command line
      *num_bins = atoi(argv[2]);  // get num_bins from command line
      *max_bin_val = atoi(argv[3]);  // get max_bin_val from command line
      *min_bin_val = atoi(argv[4]);  // get min_bin_val from command line
      if (argc==6) *num_list_out=false; // suppress output of numbers array
   }
   else {
      if (rank==0) {
         cout << "Usage: mpirun -np [num MPI processes] histogram_MPI [list size num_bins max_bin_val min_bin_val]" << endl;
      }
      MPI_Finalize(); // Terminate MPI
      exit(0); // Exit Program
   }
}

/*
Routine that fills a 1-D array with Random Data with values
based on a normal distibution that has a mean of MEAN and a
standard deviation of STDEV
This simulates in some way what might happen if there was a
single sequential data acquisition source that only one MPI process
could handle at a time
*/
#define MEAN  50.0f
#define STDEV 20.0f

void create_list(double *numbers,int list_size) {
   std::default_random_engine generator(123546);
   std::normal_distribution<double> distribution(MEAN,STDEV);

   for(int i=0;i<list_size;i++) {
      numbers[i]=distribution(generator);
   }
}

// Routine that outputs the numbers matrix to the screen
void print_matrix(double *numbers,int list_size)
{
   for(int i=0;i<list_size;i++) {
      cout << numbers[i] << endl;
   }
}

// MAIN ROUTINE: parallel histogram calculation
int main(int argc, char *argv[]) {

   int numprocs,rank;

   MPI_Init(&argc,&argv); // Initalize MPI environment
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs); //get total number of processes
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get process identity number

   //get run time parameters from the user
   // list_size == number of numbers to generate on root node
   // num_bins  == number of equally-spaced bins for the histogram
   // value range (max_val,min_val) that is being recorded in the histogram
   int list_size, num_bins;
   double max_val, min_val;
   bool print_flg=true; // default is to print numbers out to screen

   get_parameters(&list_size,&num_bins,&max_val,&min_val,&print_flg,
      argc,argv,rank,numprocs);
   // compute the range associated with each bin
   const double bin_range = (max_val-min_val)/num_bins;

   double *numbers = nullptr;
   int *histogram = nullptr;

   //  if root MPI Process (0) then
   if(rank==0) {

      // dynamically allocate from heap the histogram array on the root process
      try {
         histogram = new int[num_bins];

         numbers = new double[list_size];
      }
      catch (const exception& err) {
      cout << err.what() << " on Rank=0 MPI Process :(" << endl << flush;
      MPI_Abort(MPI_COMM_WORLD,1); // abort the MPI Environment
   }

      // initialize numbers array with random data
      create_list(numbers,list_size);

      // print the array if printing is enabled
      if (print_flg) {
         cout << "numbers matrix =" << endl;
         print_matrix(numbers,list_size);
         cout << endl;
      }
   }

   const int numbers_local_array_sz = list_size/numprocs+1;
   double *numbers_local = nullptr;

   int *histogram_local = nullptr;

   // dynamically allocate from heap the arrays on each of the MPI Processes
   try {
      numbers_local = new double[numbers_local_array_sz];
      histogram_local = new int[num_bins];
   }
   catch (const exception& err) {
      cout << err.what() << " on Rank=" << rank << " MPI Process :(" << endl << flush;
      MPI_Abort(MPI_COMM_WORLD,1); // abort the MPI Environment
   }

   // local list size for MPI process
   const int list_size_ideal = list_size / numprocs;
   const int list_size_rem   = list_size % numprocs;
   const int local_list_size = list_size_ideal + (rank < list_size_rem);

   // use fancy self-deleting arrays that default to nullptr
   unique_ptr<int[]> sendcounts;
   unique_ptr<int[]> offsets;

   // only needed in the root, so only allocate memory there
   if (rank == 0) {
      try {
         sendcounts = make_unique<int[]>(numprocs);
         offsets = make_unique<int[]>(numprocs);
      } catch (const exception& err) {
         cout << err.what() << " on Rank=0 MPI Process :(" << endl << flush;
         MPI_Abort(MPI_COMM_WORLD,1); // abort the MPI Environment
      }

      // base case is easy
      offsets[0] = 0; // no displacement
      sendcounts[0] = local_list_size; // if any remainder, it starts here

      // continue the induction chain
      for (int proc = 1; proc < numprocs; proc++) {
         sendcounts[proc] = list_size_ideal + (proc < list_size_rem);
         offsets[proc] = offsets[proc-1] + sendcounts[proc-1];
      }
   }

   // scatter the numbers array to all MPI processes in the system
   MPI_Scatterv(numbers, sendcounts.get(), offsets.get(), MPI_DOUBLE,
      numbers_local, local_list_size, MPI_DOUBLE, 0 , MPI_COMM_WORLD);

   // create local histogram of the set of numbers associated
   // with the current MPI process

   // clear out local histogram array, histogram_local
   for (int i=0;i<num_bins;i++) histogram_local[i]=0.0f;

   // compute the local histogram for the local set of numbers
   for (int i=0;i<local_list_size;i++) {
      double num;
      num = numbers_local[i];
      if (num>=min_val && num < max_val) {
         num -= min_val;
         histogram_local[(int) (num/bin_range)]++;
      }
   }
   // obtain final histogram on root MPI process by element-wise
   // summation of each of the local histograms present on each
   // MPI process
   MPI_Reduce(histogram_local, histogram, num_bins, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   // output sum from root MPI process
   if (rank==0) {
       cout << "Histogram Information" << endl;
       for (int i=0;i<num_bins;i++) {
          cout << setprecision(8) << min_val+bin_range/2+bin_range*i <<
             "  " << histogram[i] << endl;
       }
       cout << numprocs << " MPI Process OpenMPI Implementation"
            << endl << flush;
   }

   // reclaim dynamiclly allocated memory
   if (rank==0) {
      delete[] histogram;
      delete[] numbers;
   }
   delete[] histogram_local;
   delete[] numbers_local;

   // Terminate MPI Program -- perform necessary MPI housekeeping
   // clear out all buffers, remove handlers, etc.
   MPI_Finalize(); // Remove MPI Environment
   return 0;
}
