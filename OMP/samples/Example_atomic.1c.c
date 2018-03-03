/*
* @@name:	atomic.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include<stdio.h>
#include<omp.h>

float work1(int i)
{
  return 1.0 * i;
}

float work2(int i)
{
   return 2.0 * i;
}

void atomic_example(float *x, float *y, int *index, int n)
{
  int i;

  #pragma omp parallel for shared(x, y, index, n)
    for (i=0; i<n; i++) {
      #pragma omp atomic update
      x[index[i]] += work1(i);
      y[i] += work2(i);
     }
}

float atomic_read(float *p, float value)
{
	   // int value;
	    /* Guarantee that the entire value of *p is read atomically. No part of
	     *  * *p can change during the read operation.
	     *   */
#pragma omp atomic read
	     value = *p;
	     return value;
}

void atomic_write(float *p, float value)
{
	/* Guarantee that value is stored atomically into *p. No part of *p can
	 * change
	 *  * until after the entire write operation is completed.
	 *   */
#pragma omp atomic write
	    *p = value;
}

int main()
{
  int n=10;
  float x[n];
  float y[10*n];
  int index[10*n];
  int i;
 float value=333;

  for (i = 0; i < 10*n; i++) {
    index[i] = i % n;
    y[i]=0.0;
  }
  for (i = 0; i < n; i++)
    x[i] = 0.0;

  atomic_example(x, y, index, 10*n);
  value=atomic_read(x,value);
  atomic_write(y, value);

#pragma omp single
  for (i=0; i<n; i++) {
printf("i: %d, x: %f, y: %f value: %f\n",i,x[i],y[i],value);
  }
  return 0;
}
