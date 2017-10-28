#include <omp.h>
#include <iostream>
using namespace std;

int main ()  
{
int   i, n, chunk;
float a[100], b[100], result;
/* Some initializations */
n = 100;
chunk = 10;
result = 0.0;
for (i=0; i < n; i++)
	  {
	    a[i] = i * 1.0;
            b[i] = i * 2.0;
       }
#pragma omp parallel for private(i)  shared(cout) schedule(static,chunk) reduction(+:result)
{
	for (i=0; i < n; i++)
	    result = result + (a[i] * b[i]);    

#pragma omp single
	cout<<"Final result= "<<result<<endl;
}

}

