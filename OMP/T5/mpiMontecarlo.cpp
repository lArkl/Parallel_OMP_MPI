#include<iostream>
#include<cstdlib>
#include<ctime>

#include<mpi.h>

using namespace std;

int main(int argc, char *argv[]){

	double const pi_t = 3.141592653589793238462643;

	int me,numprocs;	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	int dc = 0,ds =0;
	int num_trails=1000000;
	int resn,ress;
	int rounds = num_trails/numprocs;

	double x,y;

	double start = MPI_Wtime();

	for(int i=0; i<rounds ; i++){
		x=rand()/(double)RAND_MAX;
		y=rand()/(double)RAND_MAX;
		if(x*x + y*y<1){
			dc++;
		}
		else ds++;
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&dc,&resn,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(&ds,&ress,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	
	if(me==0){
		double pi_d = 4.0*resn/(double)(ress+resn);
		cout<< "Pi: "<<pi_d<<"\tMPI Time: " << MPI_Wtime()-start<<endl;
	}
	
	MPI_Finalize();
	return 0;
}
