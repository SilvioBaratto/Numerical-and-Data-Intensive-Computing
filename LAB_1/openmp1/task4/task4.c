/* ============================================================	*/
/* =							      = */
/* =	 NUMERICAL AND DATA-INTENSIVE COMPUTING (PART I)      = */
/* =							      = */
/* =		Miguel Angel Garcia (September 2017)	      = */
/* =							      = */
/* ============================================================	*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NUM_THREADS	2


void main()
{
	long long i;
	long double k=0;
	omp_lock_t  sem1;

	omp_set_num_threads( NUM_THREADS );  
	omp_init_lock( &sem1 );

  	
	#pragma omp parallel for reduction(+:k)
	for (i=0; i<100000000; i++)
	{
       	// omp_set_lock( &sem1 );   // wait

		// Critical region: One thread at a time	
		
		k++;

        // omp_unset_lock( &sem1 ); // signal
	}
	
   	omp_destroy_lock( &sem1 );
	
	printf("k %Lf\n", k );
}
