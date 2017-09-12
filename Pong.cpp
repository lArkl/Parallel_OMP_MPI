#include<iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include<math.h>
using namespace std;

#define _PARALLEL

#ifdef _PARALLEL
#include <mpi.h>
#endif

#define _SINGLE

void printVector(vector<float> vector){
	for(int i=0;i<vector.size();i++)
		cout<<vector[i]<<endl;
}


int main(int argc, char *argv[]){
	vector<float> global;
	global.push_back(rand()%100/7);
	int max = pow(2,18);
	
	/*
	#ifdef _SINGLE
	time_t start,end;
	time(&start);
	for(int i=1; i <= max;i*=2){
		global.resize(i);
	}
	//printVector(global);
	time(&end);
	cout<<"T\tDim"<<endl;
	double elapse = (end-start)*100000;
	cout<<elapse<<"\t"<<global.size()<<endl;
	#endif
	*/
	
	#ifdef _PARALLEL
	int me,numprocs;	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	double start = MPI_Wtime();
	int A,B;
	int count=0;
	if(me==0)
		cout<<"A\tB\tT"<<endl;
	for(int i=1;i<=max;i*=2){
		A = count%2;
		B = 1-A;
		count++;
		if(me == A){
			//global.resize(5);
			MPI_Send(&global[0],global.size(),MPI_FLOAT,B,A,MPI_COMM_WORLD);
		}else 
			if (me == B){
				MPI_Recv(&global[0],global.size(),MPI_FLOAT,A,A,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			}
		if(me==0){
			double time = (MPI_Wtime()-start)*1000;
			cout<<A<<"\t"<<B<<"\t"<<time<<endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Finalize();
	#endif
	return 0;
}
