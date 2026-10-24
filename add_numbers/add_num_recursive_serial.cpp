/*******************************************************************/
/* Summation of a Sequence of Numbers Program -- recursive version */
/* October 2026 -- B. Earl Wells -- University of Alabama          */
/*                                    in Huntsville                */
/*******************************************************************/
/*

To compile type:

   g++ add_num_recursive_serial.cpp -o add_num_recursive_serial

To execute:
   add_num_recursive [num of nums]

*/

#include <iostream>
#include <iomanip>
#include <sstream>

#include "../randcompat.h"
#include "../ctimer_utils.h"

using namespace std;

#define SEED 2397           /* random number seed */
#define MAX_VALUE  100.0    /* maximum value of any number in list */
#define MIN_VALUE  0.0      /* minimum value of any number in list */

/*
Routine to retrieve the data size of the nums array from the
command line or get this number by prompting the user for the
information.
*/
int get_data_size(int argc,char *argv[])
{
   string input = "";
   int size;

   // ERROR if too many command line arguments
   if(argc > 2) {
      cout << "usage:  add_num_recursive [data size]" << endl;
      exit(1); // Exit Program
   }
   // One Command Line Argument Case:
   // case where user did not enter number of nums on command line
   if (argc==1) {
      while (1) {
         cout << "Enter the number of nums to be added:" << endl;
         getline(cin, input);
         stringstream myStream(input);
         if (myStream >> size) break;
         cout << "Invalid Input" << endl << endl;
      }
   }
   // Two Command Line Argument case:
   // user supplied the number of nums on the command line.
   else {
      size = atoi(argv[1]);
   }
   return size;
}

/*
Routine that fills the number matrix with Random Data with values
between MIN_VALUE and MAX_VALUE
This simulates in some way what might happen if there was a
single sequential data acquisition source such as a single file
*/
void fill_matrix(double *nums,int data_size)
{
   int i;
   srand48(SEED);
   for(i=0;i<data_size;i++) {
      nums[i]=drand48()*(MAX_VALUE-MIN_VALUE) + MIN_VALUE;
      //to verify may want to initialize the nums array with a pattern
      //that has a known answer such as the sum of nums from 0 to N-1
      // The result of that summation is (N+1)*N/2!!
      nums[i]=i; // to do so uncomment this line
   }
}

/*
Routine that outputs the nums matrix to the screen
*/
void print_matrix(double *nums,int data_size)
{
   int i;
   for(i=0;i<data_size;i++) {
      cout << nums[i] << endl;
   }
}

double *nums;

// recursive function that returns the sum of the elements in the
// list between the start_index value and the end_index
// value. The nums arry is considered to be global to the main
// program and each invocation of the add_nums() function.
double add_nums(int start_index, int end_index) {
   double partial_sum;

   // if only one element return its value -- termination condition
   if (end_index==start_index) partial_sum=nums[start_index];

   // divide list in half sum up contributions from each half
   else {
      int middle_index = start_index + (end_index-start_index+1)/2 +
                         (end_index-start_index+1)%2 -1;
      // calculate first half of list
      partial_sum =  add_nums(start_index,middle_index);
      // calculate second half of list
      partial_sum += add_nums(middle_index+1,end_index);
   }
   return partial_sum;
}

/*
MAIN ROUTINE: summation of nums in a list
*/
int main( int argc, char *argv[])
{
   double sum;
   int data_size,i;

   //get data size from command line or prompt
   //the user for input
   data_size=get_data_size(argc,argv);

   // dynamically allocate from heap the nums array
   nums = new (nothrow) double[data_size];
   if (nums==0) { // check for null pointer
      cout << "Memory Allocation Error" << endl;
      exit(1); // exit the program
   }

   // initialize nums matrix with random data
   fill_matrix(nums,data_size);

   // and print the nums matrix
   cout << "nums matrix =" << endl;
   print_matrix(nums,data_size);
   cout << endl;

   // start recording the execution time
   // in this case only record actual summation time
   TIMER_CLEAR;
   TIMER_START;

   // sum up elements in the nums array
   sum=add_nums(0,data_size-1);

   // stop recording the execution time
   TIMER_STOP;


   cout << "Sum of nums is " << setprecision(8) << sum << endl;
   cout << "time=" << setprecision(8) <<  TIMER_ELAPSED/1000000.0
        << " seconds" << endl;

   // reclaim dynamiclly allocated memory
   delete nums;
}
