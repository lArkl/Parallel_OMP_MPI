#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

#define _PARALLEL
#define _SINGLE
#ifdef _PARALLEL
#include <mpi.h>
#endif

int main(int argc, char *argv[]){
	double res;
	#ifdef _PARALLEL
	int me, numprocs;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	if(me==0)
		cout<<"n\tPI\tError\t\tTime"<<endl;
	for(int n=100;n<1000000;n*=10){
		double start;
		double local=0;
		int rounds = n/numprocs;
		int extra = n%numprocs;
		int count = me;
		for(int i=0; i<rounds; i++){
			double delta = (count+0.5)/n;
			local += 4/(1+delta*delta);
			//cout<<me<<"\t"<<i<<"\t"<<local<<endl;
			//MPI_Barrier(MPI_COMM_WORLD);
			count+=numprocs;
		}
		start = MPI_Wtime();		
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Reduce(&local,&res,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);	
		if(me==0){
			//Si existe extra
			if( extra ){
				for(int i=0;i<extra;i++){
					double delta = (numprocs*rounds+i+0.5)/n;
					res += 4/(1+delta*delta);
				}
			}
			double time = (MPI_Wtime()-start)*1000;
			double pi = res/n;
			//cout<<time<<endl;
			cout<<n<<"\t"<<pi<<"\t"<< pi-3.141592653589793238462643<<"\t"<<time<<endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Finalize();
	#endif
	/*
	#ifdef _SINGLE
	cout<<"n\tPI"<<endl;
	for(n=100; n<=1000000; n*=10){
		res = 0;
		for(int i=1; i<=n; i++){
			double delta = (i-0.5)/n;
			sum += 4/(1+delta*delta);
		}
		cout<<n<<"\t"<<res/n<<endl;
	}
	#endif
	*/
	return 0;
}
