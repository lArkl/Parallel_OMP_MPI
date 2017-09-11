/*  MPI_Reduce 
 */


#include <iostream>
using namespace std;

#define  DATA_SIZE 1
//#define _SINGLE
#define _PARALLEL
#ifdef _PARALLEL
#include <mpi.h>
#endif

int main(int argc, char *argv[]) {
    	int   me, numprocs,res;
	int data;
#ifdef _PARALLEL
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    //    MPI_Barrier(MPI_COMM_WORLD);
	if(me==0)
		data=5;
#endif

#ifdef _SINGLE
	       	data = 5;
#endif
#ifdef _PARALLEL
        MPI_Bcast(&data,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Reduce(&data, &res, DATA_SIZE, MPI_INT, MPI_SUM,
	                   0, MPI_COMM_WORLD);
	if (me==0) {
	cout<<"Proceso: "<<me<<", resultado: "<<res<<endl;
        }
    MPI_Finalize();
#endif

#ifdef _SINGLE
cout<<"Resultado: "<<data<<endl;
#endif
}
    
