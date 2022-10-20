/* ============================================================	*/
/* =							      = */
/* =	 NUMERICAL AND DATA-INTENSIVE COMPUTING (PART I)      = */
/* =							      = */
/* =		Miguel Angel Garcia (September 2012)	      = */
/* =							      = */
/* ============================================================	*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define N	1000

char A[N][N], B[N][N];
int C1[N][N], C2[N][N];

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


void Mul2( char A[N][N], char B[N][N], int C[N][N] )
{

	int i, j, k;
	
	for (i=0; i<N; i++)
	   for (j=0; j<N; j++)
	   {
	   	C[j][i] = 0;
		
	   	for (k=0; k<N; k++)
	   		C[j][i] += A[j][k] * B[k][i];
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
	clock_t start_mul1, end_mul1, start_mul2, end_mul2;
	int tmp;
	
	Ini();
	
	start_mul1 = clock();
	
	Mul2( A, B, C1 );
		
	end_mul1 = clock();

	// Mul1(A, B, C2);

	printf("Equal %d \n", Equal( C1, C2 ) );
		
	printf("CPU = %f ms Mul2 \n",  (float) (end_mul1-start_mul1)/CLOCKS_PER_SEC*1000);
}
