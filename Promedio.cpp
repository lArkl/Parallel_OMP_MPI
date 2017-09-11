#include<iostream>
#include<cstdlib>
#include<ctime>
#include<mpi.h>

using namespace std;

int main(int argc, char *argv[]){
	srand(time(0));
	/*
	int array[1000];
	for(int i=0;i<1000;i++){
		array[i]=rand()%100;
		//cout<<array[i]<<endl;
	}
	double sum;
	for(int i=0;i<1000;i++){
		sum+=array[i];
	}
	double avg = sum/1000;
	cout<<"Promedio: "<<avg<<endl;
	*/
	int me,numprocs;
	double res;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);100;
	const int max = 1000;
	int global[max];
	int total[numprocs];
	int rounds;
	rounds = max/numprocs;
	int extra = max % numprocs;
	double start;
	//Asignamos los valores
	if(me==0){
		start = MPI_Wtime();
		for(int i=0;i<max;i++)
			global[i] = rand()%100;
	}
	
	int local[rounds];
	double avg;
	
	MPI_Scatter(&global,rounds,MPI_INT, &local,rounds,MPI_INT,0,MPI_COMM_WORLD);
	
	int sum = 0;
	for(int i=0;i<rounds;i++){
		sum += local[i];
	}
	if(me != numprocs -1) avg = sum/rounds;
	MPI_Barrier(MPI_COMM_WORLD);
	
	//Calculamos el extra
	if(me == numprocs -1){
		for(int i=0;i<extra;i++){
			sum += global[rounds*numprocs+i];
		}
		avg = sum/(rounds+extra);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	
	//MPI_Reduce(&sum,&res,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Gather(&sum,1,MPI_INT, &total,1,MPI_INT,0,MPI_COMM_WORLD);
	
	if(me==0){
		for(int i=1;i<numprocs;i++){
			sum += total[i];
			//cout<<total[i]<<endl;
		}
		avg = sum/max;
		double time = (MPI_Wtime()-start)*10000;
		cout<< avg <<"\t"<<time<<endl;
	}
	MPI_Finalize();
}
