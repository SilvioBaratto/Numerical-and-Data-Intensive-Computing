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

#define NUM_THREADS 2

#define ITERATIONS 50
#define ITERATIONS_WORKLOAD 100000000

#define N 1000

char A[N][N], B[N][N];
int C1[N][N], C2[N][N];

struct timespec Diff_timespec(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec - start.tv_nsec) < 0)
	{
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	}
	else
	{
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}

void Static_Workload()
{
	long i, j = 0;
	double k = 0;

	for (i = 0; i < ITERATIONS_WORKLOAD; i++){
		if (i % 100 == 0){
			j++; // ALU workload
			k++; // FPU workload
		}
	}
	printf("End task %d :: j %ld k %f\n", omp_get_thread_num(), j, k);
}

void Dynamic_Workload(long iter)
{
	long i, j = 0, limit;
	double k = 0;

	limit = labs(10000 + rand() / 10 * iter); // limit is a pseudo-random number

	for (i = 0; i < limit; i++)
		if (i % 100 == 0) // ALU Workload
		{
			j++; // ALU workload
			k++; // FPU workload
		}
	printf("End task %d (iter %ld) :: j %ld k %f\n", omp_get_thread_num(), iter, j, k);
}

main()
{
	struct timespec ini, end, diff;
	long i;
	int tmp;

	omp_set_num_threads(NUM_THREADS); // Set maximum number of OpenMP threads

	clock_gettime(CLOCK_REALTIME, &ini);

	#pragma omp parallel for schedule(auto)
	for (i = 0; i < ITERATIONS; i++)
		Static_Workload(i);

	clock_gettime(CLOCK_REALTIME, &end);
	diff = Diff_timespec(ini, end);

	printf("Wall time = %ld ms\n", diff.tv_sec * 1000 + diff.tv_nsec / 1000000);
}
