/* ============================================================	*/
/* =							      = */
/* =	 NUMERICAL AND DATA-INTENSIVE COMPUTING (PART I)      = */
/* =							      = */
/* =		Miguel Angel Garcia (October 2012)	      = */
/* =							      = */
/* ============================================================	*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS	1

#define N	1000

char A[N][N], B[N][N];
int C1[N][N], C2[N][N];


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
	   	A[i][j] = rand();
		B[i][j] = rand();
	   }
	   	
}

void Mul1Par( char A[N][N], char B[N][N], int C[N][N] )
{
	int i, j, k;
	
	for (i=0; i<N; i++)
	   for (j=0; j<N; j++)
	   {
	   	C[i][j] = 0;
		
	   	for (k=0; k<N; k++)
	   		C[i][j] += A[i][k] * B[k][j];
	   }
}


void Mul1( char A[N][N], char B[N][N], int C[N][N] )
{
	int i, j, k;
	
	for (i=0; i<N; i++)
	   for (j=0; j<N; j++)
	   {
	   	C[i][j] = 0;
		
	   	for (k=0; k<N; k++)
	   		C[i][j] += A[i][k] * B[k][j];
	   }
}

char Equal( int c1[N][N], int c2[N][N] )
{
	int i, j;
	
	for (i=0; i<N; i++)
	   for (j=0; j<N; j++)
	   	if (c1[i][j] != c2[i][j]) return 0;
		
	return 1;
}


main()
{
	struct timespec ini, end, diff;

	int tmp;
	
	
	Ini();
	omp_set_num_threads( NUM_THREADS );  // Set maximum number of OpenMP threads

	
	clock_gettime( CLOCK_REALTIME, &ini );
	
	Mul1Par( A, B, C1 );
		
	clock_gettime( CLOCK_REALTIME, &end );
	diff = Diff_timespec( ini, end );
		

	
	Mul1( A, B, C2 );
	printf("Equal %d \n", Equal( C1, C2 ) );
		
	printf("Wall time = %ld ms\n",  diff.tv_sec * 1000 + diff.tv_nsec/1000000 );	
}


