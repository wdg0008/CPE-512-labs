/*******************************************************************/
/* Summation of a Sequence of Numbers Program -- recursive version */
/* October 2026 -- B. Earl Wells -- University of Alabama          */
/*                                    in Huntsville                */
/*******************************************************************/
/*

To compile type:

   g++ add_num_recursive_serial.cpp -o add_num_recursive_serial 

To execute:
   add_num_recursive [num of numbers] 

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
   // case where user did not enter number of numbers on command line
   if (argc==1) {
      while (1) {
         cout << "Enter the number of numbers to be added:" << endl;
         getline(cin, input);
         stringstream myStream(input);
         if (myStream >> size) break;
         cout << "Invalid Input" << endl << endl;
      }
   }
   // Two Command Line Argument case:
   // user supplied the number of numbers on the command line.
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
double add_numbers(int start_index, int end_index) {
   double partial_sum;

   // if only one element return its value -- termination condition 
   if (end_index==start_index) partial_sum=numbers[start_index];

   // divide list in half sum up contributions from each half
   else {
      int middle_index = start_index + (end_index-start_index+1)/2 +
                         (end_index-start_index+1)%2 -1;
      // calculate first half of list
      partial_sum =  add_numbers(start_index,middle_index);
      // calculate second half of list
      partial_sum += add_numbers(middle_index+1,end_index);
   }
   return partial_sum;
}

/*
MAIN ROUTINE: summation of numbers in a list
*/
int main( int argc, char *argv[])
{
   double sum;
   int data_size,i;

   //get data size from command line or prompt
   //the user for input
   data_size=get_data_size(argc,argv);

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
   sum=add_numbers(0,data_size-1);

   // stop recording the execution time
   TIMER_STOP;


   cout << "Sum of numbers is " << setprecision(8) << sum << endl;
   cout << "time=" << setprecision(8) <<  TIMER_ELAPSED/1000000.0 
        << " seconds" << endl;

   // reclaim dynamiclly allocated memory
   delete numbers;
}
