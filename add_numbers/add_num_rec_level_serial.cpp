/*******************************************************************/
/* Summation of a Sequence of Numbers Program -- hybrid recursive  */
/* procedual version that recurses down to the specified level     */
/* then just adds in a linear manner the set of numbers at the     */
/* specified level                                                 */
/* October 2026 -- B. Earl Wells -- University of Alabama          */
/*                                    in Huntsville                */
/*******************************************************************/
/*

To compile type:

   g++ add_num_rec_level_serial.cpp -o add_num_rec_level_serial

To execute:
   add_num_recursive_level [num of numbers] [level of recursion] 

*/  

using namespace std;
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <sys/time.h>

#define SEED 2397           /* random number seed */
#define MAX_VALUE  100.0    /* maximum value of any number in list */
#define MIN_VALUE  0.0      /* minimum value of any number in list */

// timer macros  
#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   ((tv2.tv_usec-tv1.tv_usec)+((tv2.tv_sec-tv1.tv_sec)*1000000))
#define TIMER_STOP      gettimeofday(&tv2, (struct timezone*)0)
struct timeval tv1,tv2;

/*
Routine to retrieve the data size of the numbers array from the 
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
   // case where user did not enter number of numbers on command line
   if (argc==1) {
      while (1) {
         cout << "Enter the number of numbers to be added:" << endl;
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
   // user supplied the number of numbers on the command line.
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
void fill_matrix(double *numbers,int data_size)
{
   int i;
   srand48(SEED);
   for(i=0;i<data_size;i++) {
      numbers[i]=drand48()*(MAX_VALUE-MIN_VALUE) + MIN_VALUE;
      //to verify may want to initialize the numbers array with a pattern
      //that has a known answer such as the sum of numbers from 0 to N-1
      // The result of that summation is (N+1)*N/2!!
      numbers[i]=i; // to do so uncomment this line
   }
}

/*
Routine that outputs the numbers matrix to the screen 
*/
void print_matrix(double *numbers,int data_size)
{
   int i;
   for(i=0;i<data_size;i++) {
      cout << numbers[i] << endl;
   }
}

double *numbers;

// recursive function that returns the sum of the elements in the
// list between the start_index value and the end_index
// value. The numbers arry is considered to be global to the main 
// program and each invocation of the add_numbers() function.
// the function recurses down to the specified level then the
// remaining numbers in the list from start_index to end_index are
// added serially
double add_numbers(int start_index, int end_index,
                   int current_level, int end_level) {
   double partial_sum;

   // if only one element return its value -- termination condition 
   if (end_index==start_index) partial_sum=numbers[start_index];
   else {
      cout << end_index << endl;
      if (current_level < end_level) {
         // divide list in half sum up contributions from each half
         int middle_index = start_index + (end_index-start_index+1)/2 +
                         (end_index-start_index+1)%2 -1;
         // calculate first half of list
         partial_sum =  add_numbers(start_index,middle_index,
                                    current_level+1,end_level);
         // calculate second half of list
         partial_sum += add_numbers(middle_index+1,end_index,
                                    current_level+1,end_level);
      }
      else {
         // sum up remaining numbers
         partial_sum = 0;
         for (int i = start_index;i<=end_index;i++) {
            partial_sum += numbers[i];
         }
      }
   }
   return partial_sum;
}

/*
MAIN ROUTINE: summation of numbers in a list
*/
int main( int argc, char *argv[])
{
   double sum;
   int data_size,i,end_level;

   //get list size and end level number from command line or prompt
   //the user for input
   get_data_size_level(argc,argv,&data_size,&end_level);

   // dynamically allocate from heap the numbers array
   numbers = new (nothrow) double[data_size];
   if (numbers==0) { // check for null pointer
      cout << "Memory Allocation Error" << endl;
      exit(1); // exit the program
   }
 
   // initialize numbers matrix with random data
   fill_matrix(numbers,data_size);

   // and print the numbers matrix
   cout << "numbers matrix =" << endl; 
   print_matrix(numbers,data_size);
   cout << endl;

   // start recording the execution time
   // in this case only record actual summation time
   TIMER_CLEAR;
   TIMER_START;

   // sum up elements in the numbers array
   sum=add_numbers(0,data_size-1,0,end_level);

   // stop recording the execution time
   TIMER_STOP;


   cout << "Sum of numbers is " << setprecision(8) << sum << endl;
   cout << "time=" << setprecision(8) <<  TIMER_ELAPSED/1000000.0 
        << " seconds" << endl;

   // reclaim dynamiclly allocated memory
   delete numbers;
}
