#include <iostream>
#include<omp.h>
using namespace std;
int main () {
	int a, i;
	#pragma omp parallel private(i) shared(cout,a)
	{
		#pragma omp master
		a = 0;
		#pragma omp barrier
		#pragma omp for reduction(+:a)
			for (i = 0; i < 10; i++) {
				a += i;
			}
		#pragma omp single
		cout<<"La suma es: "<<a<<endl;
	}
}
