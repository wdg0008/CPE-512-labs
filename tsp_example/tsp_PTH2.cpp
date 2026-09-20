// Travelling Salesman Program -- pthread Version 2
// (base pthread version -- with a potential race condition)
// Restricted Distributed Access to Best City Tour shared data Version
//
// B. Earl Wells -- September 2025
//
// Assignment: Modify this code so that the use of thread-locks is reduced
//             by having each thread examine their own portion of the
//             search space and determine their own minimum cost city
//             tour candidate. Then, right before exiting, each thread 
//             should atomically overwrite the shared variables of 
//             best_city_visit_order and best_cost when their value of
//             best_cost (thread_best_cost) is less than the current 
//             value of best_cost. They should do this atomically in a
//             protected manner using a MUTEX lock
//
// Travelling Salesman Problem: "Given a specified number of "cities"
// along with the cost of travel between each pair of them, find the 
// cheapest way of visiting all the cities and returning to the first 
// city visited."
//
// For an n city tour this program examines all (n-1)! tours and 
// returns a tour with the least travel cost.
//
// compilation:
// g++ tsp_PTH2.cpp -o tsp_PTH2 -lpthread -Ofast
//
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <sys/time.h>

#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec = 0)
#define TIMER_START     gettimeofday(&tv1, (struct timezone*)0)
#define TIMER_ELAPSED   (double) (tv2.tv_usec-tv1.tv_usec)/1000000.0+(tv2.tv_sec-tv1.tv_sec)
#define TIMER_STOP      {gettimeofday(&tv2, (struct timezone*)0);timersub(&tv2,&tv1,&tv3);}
struct timeval tv1,tv2,tv3;

#define MX_CITIES 30       // maximum number of cities in tour
#define SEED (long) 178937 // random number seed
#define MAX_INT 0x7fffffff // maximum integer possible

// generate randomly a cost matrix that represents the cost of travel
// between each two cities. This generates a nonsymetric cost matrix
// which means the cost of travel between any two cities may be 
// depend on which city is the source and which is the destination
void fill_cost_matrix(int cost_matrix[][MX_CITIES],int num_cities) {
   int i,j;
   srand48(SEED);
   if (num_cities<=MX_CITIES) {
      for (i=0;i<num_cities;i++) {
         for (j=0;j<num_cities;j++) {
            if (i==j) cost_matrix[i][j]=0;
            else cost_matrix[i][j]= (int) (drand48()*100);
         }
      }
   }
   else {
      cout << "Error: Too many cities -- increase MX_CITIES parameter";
      cout << endl << " and recompile" << endl;
      exit(1);
   }
}

// This routine outputs to the screen the city cost matrix that has
// been generated 
void print_cost_matrix(int cost_matrix[][MX_CITIES],int num_cities) {
   int i,j;
   for (i=0;i<num_cities;i++) {
      for (j=0;j<num_cities;j++) {
         printf("%03d ",cost_matrix[i][j]); 
      }
      printf("\n");
   }
   printf("\n");
}

// This routine outputs the City visit order from left to right
// assuming that we are always starting our tour with City 0.
// Note: there is no loss of generality in this since any tour
// order can be rotated to make any of the cities the start and
// end point.
void print_city_visit_order(int city_order[],int num_cities) {
   for (int city=0;city<num_cities;city++) {
      cout <<"[City " << city_order[city] << "]->";
   }
   cout << "[City " << city_order[0] << "]" << endl; 
}

// routine that computes the cost of each tour where the tour is
// described in the city_order array that should have a valid 
// city order premutation of the n-1 remaining cities after the
// first city, City 0, is assumed as the starting point. The
// tour starts at City 0 and goes to the first city in the city_order
// array. The tour is then processed in the sequence dictated by
// the city_order permutation after which the tour proceeds from the
// last city in this permutation back to City 0.
// Note: there is no loss of generality in this since any tour
// order can be rotated to make any of the cities the start and
// end point.
int tour_cost(int num_cities,int *city_order,
              int cost_matrix[MX_CITIES][MX_CITIES]) {
   int prev_city,start_city,cost;
   start_city=city_order[0];
   cost = 0;
   prev_city=start_city;
   for (int city=1;city<num_cities;city++) {
      cost += cost_matrix[prev_city][city_order[city]];
      prev_city = city_order[city];
   }
   // return to starting city
   cost += cost_matrix[prev_city][start_city];
   return cost;
}

// Routine to save the current city_order permutation to another
// data structure so that it can be displayed later by the 
// print_city_visit_order routine
void save_order (int *city_order_sv,int num_cities,
                 int * city_order) {
   for (int city=0;city<num_cities;city++) {
      city_order_sv[city]=city_order[city];
   }
}

// routine to obtain the number of cities from the user
// via the command line or by prompting the user for input
int get_number_cities(int argc,char *argv[]) {
   int num_cities;
   if (argc==2) {
      num_cities = atoi(argv[1]);
   }
   else {
      if (argc==1) { 
         // input number of cities 
         cout << "Enter number of cities:" << endl;
         scanf("%d",&num_cities);
         cout << endl;
      }
      else {
         cout << "Usage: " << argv[0] << " [number of cities]"
            << endl;
         exit(1);
      }
   }
   if ((num_cities<2) || (num_cities>MX_CITIES)) {
      cout << "Error: Number of Cities too large or too small" << endl;
      exit(1);
   }
   return num_cities;
}

// Global Variables
int city_cost_matrix[MX_CITIES][MX_CITIES];
int num_cities;
// data structure that is used to store the best city visit order
int best_city_visit_order[MX_CITIES],best_cost=MAX_INT; //set to worst value poss.

// Declare pthread Mutext Lock Variable so it is visible in thread
// functions
//
//    Enter your code here
//          ¯\_(ツ)_/¯ 
//

int next_city(int city_start, bool *city_msk, int num_cities) {
   city_start++;
   // find the next unvisited city
   for (int city=city_start;city<num_cities;city++) {
      if (!city_msk[city]) {
         return city;
      }
   }
   return -1; // No more cities left at this level :(
}

// city tours generator
void city_tours_gen(int city_order_slot, int *city_visit_order,bool *city_msk,
        int cum_tour_cost) {
   //                   ^^^^
   //    Enter your code here
   //          ¯\_(ツ)_/¯ 
   //
   // To make the thread_best_city_visit_order and the thread_best_cost 
   // visible to the city_tours_gen() function, extend the parameter list to
   // include them. Pass them by reference as pointers
   // i.e.
   //     ..., *thread_best_city_visit_order, int *thread_best_cost)


   // If there are more city slots available, then city visit order permutation
   // is not complete. Systematically select from remaining index elements the
   // city that should occupy the current visit slot and call this routine 
   // in a recursive manner.
   int prev_city = city_visit_order[city_order_slot-1];
   if (city_order_slot<num_cities) {
      int city=0;
      // search for unvisited city
      while ((city=next_city(city,city_msk,num_cities)) >=0 ) { 
         
         // for each city that has not become a part of a city visit
         // order yet, place it next in the city visit order schedule
         // matrix         
         city_visit_order[city_order_slot]=city;

         // then add in the cost from the previous city to this city
         // to the total tour cost so far (up to this level) of the city
         // tour recursion tree 
         int city_tour_cost = cum_tour_cost+city_cost_matrix[prev_city][city];

         city_msk[city]=true;   // mask out this number so it cannot
                                // be used again at this level
         // then recursively call the city_tours_gen routine with the

         // city_order_slot being incremented by 1, and passing on the
         // current state of the city_msk[] arry, and the city_tour_cost
         // when this city is added to the tour.
         // Note that the first arguments is used to point to where the 
         // routine again with the slot number being where the next city
         // will be placed in the city visit order permutation 
         city_tours_gen(city_order_slot+1,city_visit_order,
            city_msk, city_tour_cost);
         //                        ^^^^
         //    Enter your code here                       
         //          ¯\_(ツ)_/¯
         //
         // Add thread_best_city_visit_order and thread_best_cost to the 
         // parameter list so that these thread-private variables which were
         // declared in this function will be visible inside the 
         // city_tours_gen() function. Pass the pointers to these variables. 
         // In this case both the thread_best_city_visit_order, and the 
         // thread_best_cost variables have been declared as a pointer in the 
         // parameter section so all that is needed is to list the variables 
         // in the proper order (i.e. the  & operator is NOT needed for the 
         // thread_best_cost case).  
         //  
         //  e.g.  add to end of the parameter list
         //   city_tours_gen(...,thread_best_city_visit_order,thread_best_cost);


         city_msk[city]=false;  // unmask (release) this city so it
                                // can be used again by other
                                // city visit order permutations
      }
   }
   // if there are no more slots available, then city visit order is complete
   else {
      // since city permutation is determined then determine the tour
      // cost and save the tour if it is better than the the best
      // tour so far

      // add loop back path back to City in slot 0 (City 0)  
      cum_tour_cost += city_cost_matrix[prev_city][0];

      // CODE MODIFICATION -- having each thread determine and 
      // record their own best city visit order tour.
      //
      // If current tour cost is less than the thread best cost so far
      // overwrite the current thread best cost and the thread best 
      // city visit order
      //
      // Since the thread_best_cost and the thread_best_visit_order
      // variables and array are thead-private, (local to each
      // thread) then you should NOT protect these variables with a
      // MUTEX lock/unlock. This would be unnecessary synchronization and
      // such synchronization could have a significant performance impact! 
      //
      //    Enter your code close to here
      //          ¯\_(ツ)_/¯ 
      //
      // Note: to access the value of the thread_best_cost you need to
      // use the de-reference operator, *,  since this is a passed-by-
      // reference scalar whose value needs to be returned to the
      // calling function. 
      // i.e. *thread_best_cost        
      if (cum_tour_cost<best_cost) {
         save_order (best_city_visit_order,num_cities,city_visit_order);
         best_cost = cum_tour_cost;
      }

      // uncomment to view all city tours
      // print_city_visit_order(city_visit_order,num_cities);
      // cout << "tour cost = " << cum_tour_cost << endl;
   }
}

void *thread_total(void * arg) {
   int tid=*(int *) arg;
   
   // Add thread-private variables thread_best_city_visit,
   // and thread_best_cost, which will allow the thread to
   // independently store the least-cost city visit order 
   // that was discovered by the thread

   // You can make these thread-private by declaring these
   // variables inside the thread 
    
   //   int thread_best_city_visit_order[MX_CITIES];
   //   int thread_best_cost=MAX_INT; //set to worst value poss.
   //
   //    Enter your code here
   //          ¯\_(ツ)_/¯
   //

   int city_visit_order[MX_CITIES];
   bool city_msk[MX_CITIES];

   // set city mask for City 0 
   // so it is not a selection candidate
   city_msk[0]=true;

   // place City 0 in the start of the city_visit_order (slot 0)
   city_visit_order[0]=0;

   // place City 1 in second slot of the city_visit_order (slot 1)
   city_visit_order[1]=tid;

   // clear out city mask array except for
   // except for City 0 
   for (int city=1;city<num_cities;city++) {
      if (city==tid) {city_msk[city]=true;}
      else {city_msk[city]=false;}
   }


   // explore all possible (n-2)! city visit orders by calling this 
   // recursive routine starting at level 2 of recursion tree 
   city_tours_gen(2,city_visit_order,city_msk,city_cost_matrix[0][tid]);
   //                                                               ^^^^
   //    Enter your code here                       
   //          ¯\_(ツ)_/¯
   //
   // Add thread_best_city_visit_order and thread_best_cost to the 
   // parameter list so that these thread-private variables which were
   // declared in this function will be visible inside the city_tours_gen()
   // function. Pass the pointers to these variables. The 
   // thread_best_city_visit_order is an array so it is automatically 
   // treated as a pointer, but the thread_best_cost is a scalar so you
   // must use the & operator to provide to pass to pointer to its location.
   // Note that the purpose of passing this as a pointer is to make this
   // a pass-by-reference to allow the value of thread_best_cost to be
   // returned to this function. 
   // (Same is true for the thread_best_city_visit_order array which is
   //  also passed by reference, but we do not have to use the & operator
   //  because C/C++ already assumes that a pointer to it is being passed)
   //  
   //  e.g.  add to end of the parameter list
   //    city_tours_gen(...,thread_best_city_visit_order, &thread_best_cost);


   // add code that will check your thread_best_cost against the 
   // shared variable best_cost and then use the save_order() 
   // function to copy the elements of the thread_best_city_visit_order
   //  array into the best_city_visit_order array and update the 
   //  best_cost with the thread_best_cost when thread_best_cost is
   //  less than best_cost.
   // Make the comparison and the copy operation atomic using a 
   // MUTEX lock.
   //
   //    Enter your code here
   //          ¯\_(ツ)_/¯
   //

   return arg;
}

void tour_search(void) {
   
   // dynamically allocate threads[num_cities] on heap
   pthread_t threads[num_cities]; 

   // dynamically allocate ids[num_cities] on heap 
   int ids[num_cities];

   // load logical id structure array -- start numbering at 1
   for (int tid=0; tid<num_cities; tid++) {
      ids[tid]=tid+1;
   }

   // initialize your globally declared MUTEX variable
   //
   //    Enter your code here
   //          ¯\_(ツ)_/¯ 
   //

   // Generate one thread per number of cities left at level 1
   // (in other words num_cities-1 threads -- so each thread processes
   //  one of the level 1 computations ) 
   // Thread Create SUBPHASE -- num_cities created
   //   [~(active points/num_cities) elements processed per thread]
   for (int tid=0; tid<num_cities-1; tid++) {
       int errcode=pthread_create(
               &threads[tid],// thread information structure
               NULL,         // thread attributes - NULL-- assume defaults
               thread_total, // function name that is to represent thread
               &ids[tid]);   // points to logical thread id of thread
       // check for error during thread creation
       if (errcode) {
           cerr << "Pthread Creation Error: " << strerror(errcode) << endl;
           exit(1);
       }
   }

   // Thread JOIN SUBPHASE
   // wait for all threads to return before exiting main program (process)
   for (int tid=0; tid<num_cities-1; tid++) {
       // wait for each thread to terminate
       int errcode=pthread_join(
               threads[tid], //pthread id information of selected thread
               NULL);        //thread return value -not used in this case
       if (errcode) {
           cerr << "Pthread Join Error: " << strerror(errcode) << endl;
           exit(1);
       }
   }

   // destroy your globally declared MUTEX variable
   //
   //    Enter your code here
   //          ¯\_(ツ)_/¯ 
   //
}

int main(int argc, char *argv[]) {

   // get number of cities from the user
   num_cities = get_number_cities(argc,argv);

   // fill city cost matrix
   fill_cost_matrix(city_cost_matrix,num_cities);
   
   // print city cost matrix
   cout << endl <<
      "____________________________________________________________________"
     << endl; 
   cout << "Number of Cities = " << num_cities <<
      endl << endl;
   cout << "Tour Cost Matrix" << endl;
   print_cost_matrix(city_cost_matrix,num_cities);

   // start timer
   TIMER_CLEAR;
   TIMER_START; 

   // search through all possible orderings of the
   // cities
   tour_search();

   // record end time
   TIMER_STOP;

   // print best city tour 
   cout << "Best City Tour:" << endl; 
   print_city_visit_order(best_city_visit_order,num_cities);

   cout << "Best Tour Cost = " <<  
      tour_cost(num_cities,best_city_visit_order,city_cost_matrix)
      << endl; 
   cout <<"Execution Time = " << TIMER_ELAPSED << " seconds" << endl;
    
}

   
