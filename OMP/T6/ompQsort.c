#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

//./qsort 10

void swap(int *x, int *y){
  int temp = *x;
  *x = *y;
  *y = temp;
}

int separate(int *x, int low, int high){
  int i, pivot, last;
  pivot = x[low]; 
  swap(x+low, x+high);
  last = low;
  for(i = low; i<high; i++){
    if(x[i] <= pivot){
      swap(x+last, x+i);
      last += 1;
    }
  }
  swap(x+last, x+high);
  return last;
}

void qs(int *z, int zstart, int zend, int firstcall)
{
	int part;
	if(firstcall == 1) {
		#pragma omp task firstprivate(z,zend,zstart)
		qs(z,0,zend,0);
	} else {
		if(zstart<zend) {
			part = separate(z, zstart,zend);
			#pragma omp task firstprivate(z,zstart,part)
			qs(z, zstart, part-1,0);
			#pragma omp task firstprivate(z,zend,part)
			qs(z,part+1,zend,0);
		}
	}
}

int main(int argc, char **argv){
	int i,n,*w;
	if(argc<=1) 
		n=5;
	else  
		n = atoi(argv[1]);
	w = malloc(n*sizeof(int));
	for(i = 0; i<n; i++) 
		w[i] = 1+rand()%99;
	int thr;
	printf("Threads\tTime\n");
	printf("=================\n");			
	//for(thr = 2; thr<9; thr*=2 ){
		#pragma omp parallel shared(w,n) //num_threads(thr)
		{
			size_t start = omp_get_wtime();
			#pragma omp single// nowait
			qs(w,0,n-1,1);
			#pragma omp single
			{
				printf("%d\t%f\n",omp_get_num_threads(),omp_get_wtime()-start);
			}
		}
//	}
	printf("\n");
	if(n<21)
		for(i=0;i<n;i++) printf("%d\n",w[i]);
	return 0;
}
