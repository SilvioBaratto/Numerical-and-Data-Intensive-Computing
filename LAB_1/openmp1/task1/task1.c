/* ============================================================	*/
/* =							      = */
/* =	 NUMERICAL AND DATA-INTENSIVE COMPUTING (PART I)      = */
/* =							      = */
/* =		Miguel Angel Garcia (September 2012)	      = */
/* =							      = */
/* ============================================================	*/

#include <time.h>
#include <stdio.h>
#include <omp.h>

#define NUM_THREADS	1
#define ITERATIONS	2000000000 / NUM_THREADS


struct timespec Diff_timespec(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void Workload()
{
	long i, j=0;
	double k=0; 
	
	for (i=0; i<ITERATIONS; i++)
	   if (i % 100 == 0) // ALU Workload
	   { 
	   	j++;	// ALU workload
		k++;    // FPU workload
	   }
	printf("End task %d :: j %ld k %f\n", omp_get_thread_num(), j, k );
}

void main()
{
	int pid, nthreads;
	clock_t ini1, end1;
	struct timespec ini2, end2, diff;

	ini1 = clock();
	clock_gettime( CLOCK_REALTIME, &ini2 );
	
	omp_set_num_threads( NUM_THREADS );  // Set maximum number of OpenMP threads
  
	nthreads = omp_get_num_threads();    // Get current number of OpenMP threads
	printf("--> %d threads before omp parallel.\n", nthreads);
    
	#pragma omp parallel private( pid )   // Every thread has its own instance (copy) of variable pid
	{
	   pid = omp_get_thread_num();   // Get identifier of current OpenMP thread
	   printf("Thread # %d out of %d threads\n", pid, omp_get_num_threads() );
	   
	   Workload();
	}
	
	end1 = clock();
	clock_gettime( CLOCK_REALTIME, &end2 );
	diff = Diff_timespec( ini2, end2 );
		
	printf("CPU time = %.0f ms \n",  (float) (end1-ini1)/CLOCKS_PER_SEC*1000  );
	printf("Wall time = %ld ms\n",  diff.tv_sec * 1000 + diff.tv_nsec/1000000 );
}
