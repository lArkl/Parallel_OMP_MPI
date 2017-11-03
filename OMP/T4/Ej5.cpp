#include <iostream>
#include<omp.h>
using namespace std;


int main () {
	int a, i;
	int least = 100;
	int most = 0;
	const int n = 10; 
	int array[n];
	for(i=0; i<n; i++){
		array[i] = 1+rand()%50;
		//cout<<array[i]<<endl;
	}
	#pragma omp parallel private(i,a) shared(cout,least,most)
	{
		#pragma omp for reduction(min:least) reduction(max:most)
			for(i=0;i<n;i++)
			{
				if(least>array[i])
					least = array[i];
				if(most < array[i] )
					most = array[i];
			}
		#pragma omp single
		{
			cout<<"El menor es: "<<least<<endl;
			cout<<"El mayor es: "<<most<<endl;
		}
	}
}
