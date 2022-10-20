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
	int pid, i;	
	long j, k=0;
	omp_lock_t  sem1;

	omp_set_num_threads( NUM_THREADS );  
	omp_init_lock( &sem1 );
   
   /**
	* In this program we have two threads that increase the value of k 
	*
	**/
  	
	#pragma omp parallel shared( sem1 ) private(i, j, pid ) firstprivate( k )
	{  
	   pid = omp_get_thread_num();    

	   for (i=0; i<10; i++)
	   {
       	   omp_set_lock( &sem1 );   // wait

      	   	   // Critical region: One thread at a time

			// Workload
			#pragma omp barrier
			for (j=0; j<1000; j++){
					if (j % 10 == 0){
						k++; 
					}    
			}

		   printf(" Thread # %d --> k %ld\n", pid, k );
           omp_unset_lock( &sem1 ); // signal

		   // Workload 2
		   for (j=0; j<1000; j++){
				if (j % 10 == 0){
					k++;
				}  
		   } 

	   }
	}
	
   	omp_destroy_lock( &sem1 );
}
