# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include "mpi.h"

#define MAXN 12


void main(int argc, char *argv[]){
	int rank;
	int size;
	MPI_Init ( &argc, &argv );
	MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
	MPI_Comm_size ( MPI_COMM_WORLD, &size );
	MPI_Status status;

	int i,j,epoch;
	
	double x[MAXN][MAXN];
	int jmax = MAXN/size;
	double xlocal[MAXN][jmax+2];
	for(epoch=0;epoch<1000;epoch++){
		for(i=0;i<MAXN;i++){
    	  for(j=0;j<jmax+2;j++){
    	    xlocal[i][j]=1+rand()%9;
    	  }
    	}
 		if ( 0 < rank ){
			MPI_Send ( xlocal[1], MAXN, MPI_DOUBLE, rank-1, 1, MPI_COMM_WORLD );
		}
  		if ( rank < size-1 ){
			MPI_Recv ( xlocal[jmax+1], 1,  MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD, &status );
  		}
	
		if ( rank < size-1 ){
			MPI_Send ( xlocal[jmax], 1, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD );
		} 
		if ( 0 < rank ){
			MPI_Recv ( xlocal[0], 1, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD, &status );
		}  
    
    
		for(i=1;i<MAXN-1;i++){
			for(j=1;j<jmax-1;j++){
				xlocal[i][j] = 
				0.25*(x[i][j+1]+x[i][j-1]+x[i+1][j]+x[i-1][j]);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
  }
  
  MPI_Finalize ( );
}
