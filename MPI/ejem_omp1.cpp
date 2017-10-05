#include<iostream>
#include<omp.h>

using namespace std;
#define CHUNKSIZE 100
#define N 10


int main(){
	omp_set_num_threads(4);
	int i,chunk;
	float a[N],b[N],c[N],d[N];
	
	for(i=0;i<N;i++){
		a[i] = i*1.5;
		b[i]=i+22.35;
	}
	chunk = CHUNKSIZE;
	
	#pragma omp parallel shared(a,b,c,chunk) private(i)
	{
		/*
		#pragma omp for schedule(dynamic,chunk) nowait
			for(i=0;i<N;i++)
				c[i]=a[i]+b[i];
		
		if(omp_get_thread_num()==0)
			cout<<omp_get_num_threads()<<endl;
		*/
		#pragma omp sections nowait
		{
			#pragma omp section
				for(i=0;i<N;i++)
					c[i]=a[i]+b[i];
			#pragma omp section
				for(i=0;i<N;i++)
					d[i] =a[i]*b[i];
		}
	}//end of parallel section
	cout<<"a[i]\tb[i]\tc[i]\td[i]"<<endl;
	for(i=0;i<N;i++)
		cout<<a[i]<<"\t"<<b[i]<<"\t"<<c[i]<<"\t"<<d[i]<<endl;
	return 0;
}
