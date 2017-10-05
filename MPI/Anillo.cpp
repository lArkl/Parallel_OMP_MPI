#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

#define _PARALLEL

#ifdef _PARALLEL
#include <mpi.h>
#endif

#define _SINGLE

int main(int argc, char *argv[]){

	#ifdef _PARALLEL
	int msg;
	int me,numprocs;	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	double start = MPI_Wtime();
	int local=0;
	for(int i=0;i<numprocs;i++){
		int next = (i==numprocs-1)? 0:i+1;
		if(me==i){
			local = 1000+2*i;
			MPI_Send(&local,1,MPI_INT,next,i,MPI_COMM_WORLD);
			//cout<<me<<"\t"<<next<<"\t"<<local<<endl;
		}
		else if (me==next){
			MPI_Recv(&local,1,MPI_INT,i,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	if(me==0){
		double time = (MPI_Wtime()-start)*1000;
		cout<<"Proc\tMsg\tT"<<endl;
		cout<<numprocs<<"\t"<<local<<"\t"<<time<<endl;
	}
	MPI_Finalize();
	#endif
	return 0;
}
