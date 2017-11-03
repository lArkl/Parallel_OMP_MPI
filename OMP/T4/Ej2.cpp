#include<iostream>
#include<omp.h>

using namespace std;

void printArray(int *a){
	for(int i=0;i<100;i++)
		cout<<a[i]<<" ";
	cout<<endl;
}


int main(void){
	int nthreads = 4;
	omp_set_num_threads(nthreads);
	const int n = 100, chunk = 5;
	int array[n];
	int i,tid,least=1000;
	for(i=0;i<n;i++){
		array[i] = rand()%n;
	}
	cout<<"Thread\tIndex\tNumber"<<endl;
	cout<<"======================"<<endl;
	#pragma omp parallel shared(array,i) private(tid)
	{
		tid = omp_get_thread_num();
		#pragma omp for schedule(static, chunk) ordered
			for(i=0; i<n; i++){
				#pragma omp ordered
				cout<<tid<<"\t"<<i<<"\t"<< array[i]<<endl; 
			}
	}
	return 0;
}
