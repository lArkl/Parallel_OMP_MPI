#include <stdio.h>
#include <stdlib.h>

int main()
{
   int ic, i, n = 1000000;

   ic = 0;
   #pragma omp parallel for shared(ic,n) private(i)
   for (i=0; i<n; i++)
   {
//      #pragma omp critical
      #pragma omp atomic
         ic += 1;
   }
   printf("Contador = %d\n",ic);

   return(0);

}
