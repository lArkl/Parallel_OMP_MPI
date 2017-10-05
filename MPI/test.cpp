#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include <time.h>

using namespace std;

static long MULTIPLIER = 1366;
static long ADDEND = 150889;
static long PMOD = 714025;
static double pi = 3.141592653589793238462643;

double random_low,random_hi;
long random_last;

double drandom()
{
	long random_next;
	double ret_val;

	// compute an integer number from zero to mod
	random_next = (MULTIPLIER * random_last + ADDEND) % PMOD;
	random_last = random_next;

	//shift into preset range
	ret_val = ((double)random_next/(double)PMOD) * (random_hi - random_low) + random_low;	
	return ret_val;
}

void seed(double low_in,double hi_in)
{
	if(low_in < hi_in)
	{
		random_low = low_in;
		random_hi = hi_in;
	}
	else
	{
		random_low = hi_in;
		random_hi = low_in;
	}

	random_last = PMOD/ADDEND;
}
int main(int argc,char *argv[])
{
	long i; 
	double x,y,test,pi1;
	double r = 1.0; // radio del circulo. Lado del cuadrado es 2*r
	time_t  t;
	seed(-1.0,1.0);
	//srand(time(&t));
	long num_trials=100000000;
	double timeA,timeB;
	int numproc,myid;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	timeA = MPI_Wtime();
	long Ncirc = 0;
	long sum_local=0;
	MPI_Bcast(&num_trials,1,MPI_LONG,0,MPI_COMM_WORLD);

	for(i=myid;i<num_trials;i+=numproc)
	{
		x = drandom();
		y = drandom();
		if(x*x + y*y <= r*r)
			sum_local++;
	}	
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Reduce(&sum_local,&Ncirc,1,MPI_LONG,MPI_SUM,0,MPI_COMM_WORLD);
	if(myid == 0)
		pi1 = 4.0 * ((double)Ncirc/(double)num_trials);
	timeB = MPI_Wtime();
	if(myid == 0){
		cout<<endl<<num_trials<<" trials, pi (teorico) is "<<pi<<endl;
		cout<<endl<<num_trials<<" trials, pi (experimental) is "<<pi1<<endl;
		cout<<endl<<"El tiempo es de: "<<(timeB-timeA)<<endl;
	}

	 MPI_Finalize();
	return 0;
}
