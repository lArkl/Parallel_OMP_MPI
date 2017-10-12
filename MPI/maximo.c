#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"


int main(int argc, char *argv[]){

	int i,size,rank;
	int A[10];
	MPI_Init( &argc, &argv );
   	MPI_Comm_size( MPI_COMM_WORLD, &size );
   	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	for(i=0;i<10;i++){
		A[i]=rand()%100;
//		printf("A[%d]= %d\n",i,A[i]);
	}
	int local_max=A[0];
	
	for(i=0;i<10;i++){
		if(A[i]>local_max){
			local_max=A[i];
		}
	}

	int global_max;
	MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0,MPI_COMM_WORLD);
	
	if (rank == 0) {
	for(i=0;i<10;i++)
		printf("A[%d]= %d\n",i,A[i]);
  	printf("Maximo = %d\n", global_max);
	}

        MPI_Finalize();

}






