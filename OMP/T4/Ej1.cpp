#include<iostream>
#include<omp.h>
#include<assert.h>
using namespace std;

int main(void){
	int nthreads = 4;
	omp_set_num_threads(nthreads);
	int i=1,j=2;
	#pragma omp parallel private(i,j) shared(cout)
	{
		i = omp_get_thread_num();
		j = i*2;

		//assert(i==1 && j==2);
		cout<<"Thread: "<<i<<"\ti: "<<i<<"\tj: "<<j<<endl;
	}
	assert(i==1 && j==2);
	cout<<"Master: "<<"i: "<<i<<"\tj: "<<j<<endl;
	return 0;
}
