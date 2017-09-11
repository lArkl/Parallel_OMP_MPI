#include<iostream>
using namespace std;

#define _PARALLEL

#ifdef _PARALLEL
#include <mpi.h>
#endif

int main(int argc, char *argv[]){
	/*int n;
	cout<<"n"<<"\t"<<"Pi"<<endl;
	cout<<"==============="<<endl;
	double sum;
	for(n=100; n<=1000000; n*=10){
		sum = 0;
		for(int i=1; i<=n; i++){
		//	cout<<sum<<endl;	
			double delta = (i-0.5)/n;
			sum += 4/(1+delta*delta);
		}
		double pi = sum/n;
		cout<<n<<"\t"<<pi<<endl;
	}*/
	int me, numprocs;
	double res;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	int n=100;
	double local=0;
	int rounds = n/numprocs;
	int count = me;
	for(int i=0; i<rounds; i++){
		double delta = (count+0.5)/n;
		local = 4/(1+delta*delta);
		cout<<me<<"\t"<<i<<"\t"<<local<<endl;
		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Reduce(&local,&res,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
		count+=numprocs;
	}
	MPI_Barrier(MPI_COMM_WORLD);
		
	if(me==0)
		cout<<"Suma total: "<<res/n<<endl;
	
	MPI_Finalize();
	return 0;
}
