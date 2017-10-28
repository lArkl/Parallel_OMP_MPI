#include <iostream>
using namespace std;

#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
#endif

void funcionA()
{
#pragma omp critical
	cout<<"Esta seccion es ejecutada en la funcionA por el hilo "<<omp_get_thread_num()<<endl;
}
void funcionB()
{
#pragma omp critical
cout<<"Esta seccion es ejecutada en la funcionB por el hilo "<<omp_get_thread_num()<<endl;
}

int main()
{
#ifdef _OPENMP
   omp_set_num_threads(4);
#endif

#pragma omp parallel 
{
   #pragma omp sections
   {
      #pragma omp section
        funcionA();

      #pragma omp section
        funcionB();
   } /*-- Fin del bloque section --*/

} /*-- Fin del bloque section --*/

}


