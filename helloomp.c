#include<stdio.h>
#include <omp.h>
void main ()  {
	int nthreads =4, tid;
	omp_set_num_threads(nthreads);
	/* Fork a team of threads with each thread having a private tid variable */
	#pragma omp parallel private(tid)
	{
	/* Obtain and print thread id */
		tid = omp_get_thread_num();
		printf("Hello World from thread = %d\n", tid);
		printf("with %d threads\n",tid);
	/* Only master thread does this */
		if (tid == 0) {
			nthreads = omp_get_num_threads();
			printf("Number of threads = %d\n", nthreads);
		}
	}  /* All threads join master thread and terminate */
	printf("all done, with hopefully %d threads\n",nthreads);
}
