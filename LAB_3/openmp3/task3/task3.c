/* ============================================================	*/
/* =							      = */
/* =	 NUMERICAL AND DATA-INTENSIVE COMPUTING (PART I)      = */
/* =							      = */
/* =		Miguel Angel Garcia (October 2015)	      = */
/* =							      = */
/* ============================================================	*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NUM_THREADS	1

#define N	4000

long double A[N][N], B[N][N], C[N][N], D[N][N];
long double A2[N][N], B2[N][N], C2[N][N], D2[N][N];


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

void Ini()
{
	int i, j;
		
	for (i=0; i<N; i++)
	   for (j=0; j<N; j++)
	   {
	   	A[i][j] = A2[i][j] = rand();
		B[i][j] = B2[i][j] = rand();
		C[i][j] = C2[i][j] = rand();
		D[i][j] = D2[i][j] = rand();
	   }
	   	
}

void LoopPar( long double A[N][N], long double B[N][N], long double C[N][N], long double D[N][N] )
{





}


void Loop( long double A[N][N], long double B[N][N], long double C[N][N], long double D[N][N] )
{
	int i, j;
	
	for (i=0; i<N; i++)
	for (j=0; j<N; j++)
	{
		
	    // S1
		if (i+1 < N && j+1 < N)
		   if (i-3 >= 0 && j+1 < N)
			C[i+1][j+1] = D[i-3][j+1] * 3;  
			
	    // S2
	    	if (i-1 >= 0)
		   if (j-1 >= 0)
			D[i-1][j] = D[i][j] * B[i][j-1] - 2; 
	    // S3
		if (i+1 < N && j-1 >= 0)
		   if (i+1 < N && i-1 >= 0)
			B[i+1][j-1] = A[i+1][j] - A[i-1][j]; 

	     // S4
		if (i-1 >= 0 && j-1 >= 0)
			A[i][j] = D[i-1][j-1] * 3;  

	}
}



char Equal( long double A[N][N], long double B[N][N], long double C[N][N], long double D[N][N],
	    long double A2[N][N], long double B2[N][N], long double C2[N][N], long double D2[N][N] )
{
	int i, j;
	
	for (i=0; i<N; i++)
	for (j=0; j<N; j++)
	{
	   	if (fabs(A[i][j] - A2[i][j]) > 0.00001) return 0;
	   	if (fabs(B[i][j] - B2[i][j]) > 0.00001) return 0;
	   	if (fabs(C[i][j] - C2[i][j]) > 0.00001) return 0;
	   	if (fabs(D[i][j] - D2[i][j]) > 0.00001) return 0;
	}
		
	return 1;
}


main()
{
	struct timespec ini, end, diff;
	int tmp;
	
	
	Ini();
	omp_set_num_threads( NUM_THREADS );  // Set maximum number of OpenMP threads
	
	clock_gettime( CLOCK_REALTIME, &ini );
	
	   LoopPar( A2, B2, C2, D2 );
		
	clock_gettime( CLOCK_REALTIME, &end );
	diff = Diff_timespec( ini, end );

	printf("Parallel wall time = %ld ms\n",  diff.tv_sec * 1000 + diff.tv_nsec/1000000 );			

	clock_gettime( CLOCK_REALTIME, &ini );
	
	   Loop( A, B, C, D );
	
	clock_gettime( CLOCK_REALTIME, &end );
	diff = Diff_timespec( ini, end );

	printf("Sequential wall time = %ld ms\n",  diff.tv_sec * 1000 + diff.tv_nsec/1000000 );	
	
	printf("Equal %d \n", Equal( A, B, C, D, A2, B2, C2, D2 ) );
		

}

