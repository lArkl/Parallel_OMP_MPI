/*
* @@name:	barrier_regions.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include<stdio.h>
#include<omp.h>

void work(int n) {printf("thread: %d n: %d \n",omp_get_thread_num(),n);}

void sub3(int n)
{
  work(n);
  #pragma omp barrier
  work(n);
}

void sub2(int k)
{
  #pragma omp parallel num_threads(4) shared(k) 
	{  sub3(k);
      printf("sub2, thread: %d k: %d \n",omp_get_thread_num(),k);
	}
}

void sub1(int n)
{
  int i;
  #pragma omp parallel num_threads(4) private(i) shared(n) 
  {
    #pragma omp for
    for (i=0; i<n; i++){
      sub2(i);
      printf("sub1, thread: %d n: %d \n",omp_get_thread_num(),n);
    }
  }
}

int main()
{
  sub1(2);
//  sub2(2);
//  sub3(2);

  return 0;
}
