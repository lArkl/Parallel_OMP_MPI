#include<iostream>
#include<cstdlib>
#include<ctime>
#include<mpi.h>
#include<omp.h>
using namespace std;



int main(int argc, char *argv[]){
	double res;
	int me, numprocs;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	for(int thr = 2;thr<7;thr+=2)
	{	
		if(me==0)
			cout<<"Threads: "<<thr<<"\nn\tError\t\tTime"<<endl;
		for(int n=10000;n<=1000000;n*=10){
			double start;
			double local;
			int rounds = n/numprocs;
			int extra = n%numprocs;
			int sum;
			#pragma omp parallel shared(local,sum) num_threads(thr)
			{
				sum = 0;
				int initial = rounds*me;
				int end = rounds*(me+1);
				local = 0;
				#pragma omp barrier
				#pragma omp for reduction(+:local)
				for(int i=initial; i<end; i++){
					double delta = (i+0.5)/n;
					local += 4/(1+delta*delta);
				}
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
				cout<<n<<"\t"<< pi-3.141592653589793238462643<<"\t"<<time<<endl;
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	/* Codigo secuencial
	cout<<"n\tPI"<<endl;
	double start = clock();
	for(int n=10000; n<=1000000; n*=10){
		double sum = 0.0;
		for(int i=0; i<=n; i++){
			double delta = (i-0.5)/n;
			sum += 4/(1+delta*delta);
		}
		double end = clock();
		cout<<n<<"\t"<<sum/n<<"\ttime: "<<(end-start)/CLOCKS_PER_SEC<<endl;
	}
	*/
	return 0;
}
