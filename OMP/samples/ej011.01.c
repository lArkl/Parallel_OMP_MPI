#include <stdio.h>
#include <stdlib.h>

int main()
{
int TID = -1;

#pragma omp parallel private(TID)
#pragma omp critical
   {
	TID = omp_get_thread_num();
      	printf("Hilo %d ejecuta region en paralelo externa\n",
        TID);

     #pragma omp parallel num_threads(3)
     {
        printf(" TID: %d, Hilo %d ejecuta region en paralelo interna\n",
               TID,omp_get_thread_num());
     }  /*-- region en paralelo interna --*/
   }  /*-- region en paralelo externa --*/

   return(0);
}


