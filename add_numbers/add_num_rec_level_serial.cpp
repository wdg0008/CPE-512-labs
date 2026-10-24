/*******************************************************************/
/* Summation of a Sequence of Numbers Program -- hybrid recursive  */
/* procedual version that recurses down to the specified level     */
/* then just adds in a linear manner the set of nums at the     */
/* specified level                                                 */
/* October 2026 -- B. Earl Wells -- University of Alabama          */
/*                                    in Huntsville                */
/*******************************************************************/
/*

To compile type:

   g++ add_num_rec_level_serial.cpp -o add_num_rec_level_serial

To execute:
   add_num_recursive_level [num of nums] [level of recursion]

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
void get_data_size_level(int argc,char *argv[],int *data_size,
                        int *level)
{
   string input = "";
   int size,lev;

   // ERROR if too many command line arguments
   if(argc > 3 || argc==2) {
      cout << "usage:  add_num_rec_level_serial [data size] [level]"
           << endl;
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
      while (1) {
         cout << "Enter the level number to stop recursion:" << endl;
         getline(cin, input);
         stringstream myStream(input);
         if (myStream >> lev) break;
         cout << "Invalid Input" << endl << endl;
      }
      *data_size=size;
      *level=lev;
   }
   // Three Command Line Argument case:
   // user supplied the number of nums on the command line.
   else {
      *data_size = atoi(argv[1]);
      *level = atoi(argv[2]);
   }
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
// the function recurses down to the specified level then the
// remaining nums in the list from start_index to end_index are
// added serially
double add_nums(int start_index, int end_index,
                   int current_level, int end_level) {
   double partial_sum;

   // if only one element return its value -- termination condition
   if (end_index==start_index) partial_sum=nums[start_index];
   else {
      cout << end_index << endl;
      if (current_level < end_level) {
         // divide list in half sum up contributions from each half
         int middle_index = start_index + (end_index-start_index+1)/2 +
                         (end_index-start_index+1)%2 -1;
         // calculate first half of list
         partial_sum =  add_nums(start_index,middle_index,
                                    current_level+1,end_level);
         // calculate second half of list
         partial_sum += add_nums(middle_index+1,end_index,
                                    current_level+1,end_level);
      }
      else {
         // sum up remaining nums
         partial_sum = 0;
         for (int i = start_index;i<=end_index;i++) {
            partial_sum += nums[i];
         }
      }
   }
   return partial_sum;
}

/*
MAIN ROUTINE: summation of nums in a list
*/
int main( int argc, char *argv[])
{
   double sum;
   int data_size,i,end_level;

   //get list size and end level number from command line or prompt
   //the user for input
   get_data_size_level(argc,argv,&data_size,&end_level);

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
   sum=add_nums(0,data_size-1,0,end_level);

   // stop recording the execution time
   TIMER_STOP;


   cout << "Sum of nums is " << setprecision(8) << sum << endl;
   cout << "time=" << setprecision(8) <<  TIMER_ELAPSED/1000000.0
        << " seconds" << endl;

   // reclaim dynamiclly allocated memory
   delete nums;
}
