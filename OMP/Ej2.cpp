#include<iostream>
#include<omp.h>

using namespace std;

void printArray(int *a){
	for(int i=0;i<100;i++)
		cout<<a[i]<<"\t";
	cout<<endl;
}


int main(void){
	int nthreads = 4;
	omp_set_num_threads(nthreads);
	int n = 100, chunk = 5;
	int array[n];
	int i,j,least=1000;
	for(i=0;i<n;i++){
		array[i] = rand()%n;
	}
	printArray(array);
	#pragma omp parallel for shared(array,i,j) ordered schedule(static, chunk)
		for(i=1;i<n;i++){
			j = i;
			#pragma omp critical
			while(j>0 && array[j-1]>array[j]){
				int aux = array[j];
				array[j] = array[j-1];
				array[j-1] = aux;
				j -= 1;
			}
		}
	cout<<"Master: "<<endl;
	printArray(array);
	return 0;
}
