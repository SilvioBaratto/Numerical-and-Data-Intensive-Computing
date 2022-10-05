/* ============================================================	*/
/* =							      = */
/* =	 NUMERICAL AND DATA-INTENSIVE COMPUTING (PART I)      = */
/* =							      = */
/* =		Miguel Angel Garcia (September 2012)	      = */
/* =							      = */
/* ============================================================	*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NUM_THREADS	2


void main()
{
	int pid, nthreads;
	long i, j=0, limit = -1;
	
	omp_set_num_threads( NUM_THREADS );  
	
	srand( (unsigned int) time(NULL) );   // Randomize pseudo-random number generator
     
     	// pid is wrongly declared as shared in the parallel directive
	
	#pragma omp parallel shared( pid ) private( limit, i, j ) 
	{  
	   pid = omp_get_thread_num();    // Each thread modifies the shared variable 
	   				  // on its own cache.					  
	   printf("Start thread # %d :: initial limit %ld\n", pid, limit );

	   // Dynamic workload (one thread finishes before the other).

	   limit = labs( 50000 + rand() * 100000 );  // limit is a pseudo-random number
	   for (i=0; i<limit; i++) j++;   
	   
	   printf("\tEnd thread # %d :: final limit %ld \n", pid, limit );

	   // After ending the parallel section, a thread flushes 
	   // its cached variables to main memory. The other caches 
	   // keeping copies of the same variables are automatically 
	   // refreshed with the flushed value, hence overwriting their 
	   // previous values.
	}

	printf("Last limit %ld\n", limit );
}
