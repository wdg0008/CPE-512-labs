/*
B. Earl Wells -- UAH  
Prime Number Generation using a distributed multiple strike out scheme where 
for each new prime number that is found then subsequent multiples of that
prime number are struck out masked out for consideration as primes 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MX_NUMS  6000000 // maximum numbers to be considered 

using namespace std;
#include <iostream>
int main( int argc, char *argv[])
{
   char numbers[MX_NUMS];
   long long int next_prime;
 
   // determine maximum prime multiple that needs to be examined
   long long int mx_prime_mult = 1+(int) sqrt((double) MX_NUMS); 

   // reset number cross ref list where 1 means number may be prime
   for (int i=0;i<MX_NUMS;i++) numbers[i]=1;

   // first node begins the crossing out process starting
   // with the prime number 2  
   next_prime=2; // give it first prime number      
   do {
      if (next_prime<=mx_prime_mult) {
         // remove all multiples of the prime number
         for (int i=next_prime;i<MX_NUMS;i += next_prime) {
            numbers[i]=0;   
         }
      }

      cout << "prime=" << next_prime << endl; 
      // find next prime number (number not struck out) in list
      while ((++next_prime)<mx_prime_mult && !numbers[next_prime]);

   } while (next_prime<mx_prime_mult);

   cout << flush; // flush output buffer

}
