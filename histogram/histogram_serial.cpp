/**************************************************************/
/* Histogram construction Program -- Serial version           */
/* September 2023 -- B. Earl Wells -- University of Alabama   */
/* (C++ Program)                     in Huntsville            */
/**************************************************************/
/*
The program represents the sequential version of a histogram 
program, that is used used often in the CPE 412/512 text. 
The program creates a histogram of an array of numbers which 
are generated peudo randomly in a manner that has a normal 
distribution with a mean MEAN and standard deviation given by 
the symbolic constants expressed below:
*/

#define MEAN  50.0f
#define STDEV 20.0f

/*
Compilation

   GNU Compiler
   g++ histogram_serial.cpp -o histogram_serial

To execute:
   
   GNU compiler
   run_script histogram_serial.sh

*/  

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <random>
#include <exception>

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
      if (myStream >> num) break;
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
   double *min_bin_val,bool *num_list_out, int argc,char *argv[]) {

   if(argc==1) {
      // One Command Line Argument Case:
      // case where user did not enter number of numbers on command line
      // then the program prompts the user.
      *list_size=enter_next_param<int>("Enter number of numbers in list:");
      *num_bins=enter_next_param<int>("Enter number of bins in histogram:");
      *max_bin_val=enter_next_param<double>("Enter Upper Range of histogram:");
      *min_bin_val=enter_next_param<double>("Enter Lower Range of histogram:");
      char print_out=enter_next_param<char>("Suppress numbers array printing (y or n):");
      if (print_out=='y' || print_out=='Y') *num_list_out=false; 
   }
   else if (argc==5 || argc==6) {
      // Two Command Line Argument case:
      // user supplied the number of numbers on the command line.
      *list_size = atoi(argv[1]); // get list_size from command line
      *num_bins = atoi(argv[2]);  // get num_bins from command line
      *max_bin_val = atoi(argv[3]);  // get max_bin_val from command line
      *min_bin_val = atoi(argv[4]);  // get min_bin_val from command line
      if (argc==6) *num_list_out=false; // suppress output of numbers array
   }
   else {
      cout << "Usage: histogram [list size num_bins max_bin_val min_bin_val]" << endl;
      exit(1); // Exit Program 
   }
}

/*
Routine that fills a 1-D array with Random Data with values
based on a normal distibution that has a mean of MEAN and a
standard deviation of STDEV
*/
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

   //get run time parameters from the user
   // list_size == number of numbers to generate on root node
   // num_bins  == number of equally-spaced bins for the histogram
   // value range (max_val,min_val) that is being recorded in the histogram
   int list_size, num_bins;
   double max_val, min_val;
   bool print_flg=true; // default is to print numbers out to screen

   get_parameters(&list_size,&num_bins,&max_val,&min_val,&print_flg,
      argc,argv);
   // compute the range associated with each bin
   const double bin_range = (max_val-min_val)/num_bins;

   double *numbers;
   int *histogram;

   // dynamically allocate from heap the histogram array on the root process
   try {
      histogram = new int[num_bins];
   }
   catch (exception& err) { 
      cout << err.what() << " :(" << endl << flush;
   }

   // dynamically allocate from heap the numbers array on the root process
   try {
      numbers = new double[list_size];
   }
   catch (exception& err) { 
      cout << err.what() << " :(" << endl << flush;
   }

   // initialize numbers array with random data
   create_list(numbers,list_size);

   // print the array if printing is enabled
   if (print_flg) { 
      cout << "numbers matrix =" << endl; 
      print_matrix(numbers,list_size);
      cout << endl;
   }

   // create histogram of the set of numbers in the 
   // numbers array 

   // clear out histogram array, histogram
   for (int i=0;i<num_bins;i++) histogram[i]=0.0f;
 
   // compute the histogram 
   for (int i=0;i<list_size;i++) {
      double num;
      num = numbers[i];
      if (num>=min_val && num < max_val) {
         num -= min_val;
         histogram[(int) (num/bin_range)]++;
      }
   }
   cout << "Histogram Information" << endl;
   for (int i=0;i<num_bins;i++) {
      cout << setprecision(8) << min_val+bin_range/2+bin_range*i <<
         "  " << histogram[i] << endl <<flush;
   }

   // reclaim dynamiclly allocated memory
   delete histogram;
   delete numbers;

}

