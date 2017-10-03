#include<iostream>

#ifdef _OPENMP
	#include<omp.h>
#else
	#define omp_get_thread_num() 0
#endif

using namespace std;


int main(){
	
	int n=9;
	int i,a,b[n];
#ifdef _OPENMP
	omp_set_num_threads(4);
#endif
	
	for(i=0;i<n;i++){
		b[i]=-1;
	}
	#pragma omp parallel shared(a,b) private(i)
	{
		#pragma omp single
		{
			a = 10;
			cout<<"constructor single ejecutado por el hilo "<<omp_get_thread_num()<<endl;
		}
		#pragma omp for
		for(i=0;i<n;i++)
			b[i] = a;
	}//end of parallel section
	
	cout<<"Fin de la region en paralelo: "<<endl;
	for(i=0;i<n;i++)
		cout<<"b["<<i<<"]"<<"="<<b[i]<<endl;
	return 0;
}
