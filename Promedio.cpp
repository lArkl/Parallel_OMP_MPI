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
	srand(time(0));

	#ifdef _PARALLEL
	int me,numprocs;	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	#endif

	const int max = 1000;
	int global[max];
	#ifdef _PARALLEL
	int total[numprocs];
	int rounds = max/numprocs;
	int extra = max % numprocs;
	#endif
	double start;
	//Asignamos los valores
	#ifdef _PARALLEL
	if(me==0){
		start = MPI_Wtime();
		for(int i=0;i<max;i++)
			global[i] = rand()%100;
	}
	int local[rounds];
	#endif
	/*
	#ifdef _SINGLE
	for(int i=0;i<max;i++)
		global[i] = rand()%100;
	#endif
	*/
	double avg;
	int sum = 0;
	
	#ifdef _PARALLEL
	MPI_Scatter(&global,rounds,MPI_INT, &local,rounds,MPI_INT,0,MPI_COMM_WORLD);	
	for(int i=0;i<rounds;i++){
		sum += local[i];
	}
	MPI_Barrier(MPI_COMM_WORLD);
	
	//MPI_Reduce(&sum,&res,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Gather(&sum,1,MPI_INT, &total,1,MPI_INT,0,MPI_COMM_WORLD);
	if(me==0){
		sum = 0;
		for(int i=0;i<numprocs;i++){
			sum += total[i];
		}		
		//Calculamos el extra
		if(extra)
			for(int i=0;i<extra;i++)
				sum += global[rounds*numprocs+i];
		
		avg = sum/max;
		cout<<"AVG\tTime"<<endl;
		double time = (MPI_Wtime()-start)*10000;
		cout<< avg <<"\t"<<time<<endl;
	}
	MPI_Finalize();
	#endif	
	/*
	#ifdef _SINGLE
	for(int i=0;i<1000;i++){
		sum+=global[i];
	}
	cout<<"AVG: "<< avg << endl;
	#endif
	*/
}
