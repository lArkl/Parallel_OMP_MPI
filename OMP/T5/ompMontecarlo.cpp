#include<iostream>
#include<omp.h>

using namespace std;

int main(void){
	int nthreads = 4;
	omp_set_num_threads(nthreads);
	int const max = 1000000;
	int i, thd, circle, square;
	double x,y;
	#pragma omp parallel private(i,x,y,thd) shared(circle,square,cout)
	{
		thd = omp_get_thread_num();
		size_t time = omp_get_wtime();
		circle = 0;
		square = 0;
		#pragma omp barrier
		#pragma omp for reduction(+:circle) reduction(+:square)
			for(i = 0;i<max;i++)
			{
				x = rand()/(double)RAND_MAX;
				y = rand()/(double)RAND_MAX;
				if(x*x+y*y < 1)
					circle++;
				else
					square++;
			}
		#pragma omp single
		{
			cout<<"Pi: "<<circle*4.0/(circle+square)<<"\tTime: "<<omp_get_wtime()-time<<endl;		
		}
	}
	return 0;
}
