/* ============================================================	*/
/* =							      = */
/* =	 NUMERICAL AND DATA-INTENSIVE COMPUTING (PART I)      = */
/* =							      = */
/* =		Miguel Angel Garcia (September 2012)	      = */
/* =							      = */
/* ============================================================	*/

#include <time.h>
#include <stdio.h>

#define ITERATIONS	100
#define N	2000

char array[N][N];

void fun()
{
	int i, j;
	
	for (i=0; i<N; i++)
	   for (j=0; j<N; j++)
	   	array[j][i]=0; // switched rows and columns
	
	for (i=0; i<N; i++)
	   for (j=0; j<N; j++)
	   	array[j][i]++;	// switched rows and columns

	/* just debug

	for (i = 0; i <	N; i++)
	{
		for(j = 0; j < N; j++)
		{
			printf("%d     ", array[i][j]);
		}
		printf("\n");
	}
	*/
}

main()
{

	clock_t ini, end;

	int tmp;
	
	ini = clock();
	
	for (tmp=0; tmp<ITERATIONS; tmp++) fun();
		
	end = clock();
		
	printf("CPU = %f ms \n",  (float) (end-ini)/CLOCKS_PER_SEC/ITERATIONS*1000  );
}
