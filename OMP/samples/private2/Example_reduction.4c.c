#include <stdio.h>
#include <omp.h>

int main (void)
{
  int a=0, i;

  #pragma omp parallel  private(i) 
  {
	a=0;
    #pragma omp for reduction(+:a)
    for (i = 0; i < 10; i++) {
        a += i;
	 printf ("valor de %d\n", a);
    }

    #pragma omp single
    printf ("La suma es %d\n", a);
  }
  return 0;
}
