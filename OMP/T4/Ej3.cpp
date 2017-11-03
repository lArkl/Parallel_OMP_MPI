#include <iostream>
#include<omp.h>
using namespace std;
int main () {
	int a, i;
	#pragma omp parallel
	{
		a = 0;
		#pragma omp for reduction(+:a)
			for (i = 0; i < 10; i++) {
				a += i;
			}
	}
	cout<<"La suma es: "<<a<<endl;
}
