#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 1

#define ITERATIONS 50
#define ITERATIONS_WORKLOAD 10

#define N 1000

void Static_Worload(){
	int i = 0;
    printf("printing %d\n", i);
}

int main(){
    int i = 100;
    Static_Worload("prova", i);
}