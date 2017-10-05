#include<iostream>
#include<cstdlib>
#include<ctime>

#include<mpi.h>

using namespace std;

class Randomizer{
	private:
	int const multiplier = 1366;
	int const addend = 150889;
	int const pmod = 714025;
	int random_low,random_hi;
	long random_last;
	public:
	Randomizer(){	srand(time(0));}
	double nrand(){
		int r = rand();
		double d = r/(double)RAND_MAX;
		return d;
	}
	void seed(double low_in,double hi_in){
		//random hi and low setter
		if(low_in < hi_in){
			random_low = low_in;
			random_hi = hi_in;
		}else{
			random_low = hi_in;
			random_hi = low_in;
		}
		random_last = pmod/addend;
	}
	double drandom(){
		long random_next;
		double ret_val;
		random_next = (multiplier*random_last+addend)%pmod;
		random_last = random_next;
		ret_val = ((double)random_next/(double)pmod)*(random_hi-random_low)+random_low;
		return ret_val;
	}
};

int main(int argc, char *argv[]){

	double const pi_t = 3.141592653589793238462643;
	Randomizer ru;

	int me,numprocs;	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	int const max = 10000;
	ru.seed(-max,max);
	int dc = 0,ds =0;
	int num_trails=100000000;
	int resn,ress;
	int rounds = num_trails/numprocs;

	double x,y;

	double start = MPI_Wtime();

	for(int i=0; i<rounds ; i++){
		x=ru.drandom()/max;
		y=ru.drandom()/max;
		if(x*x + y*y<1){
			dc++;
		}
		else ds++;
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&dc,&resn,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(&ds,&ress,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	
	if(me==0){
		double t = MPI_Wtime()-start;
		double pi_d = 4*resn/(double)(ress+resn);
		cout<<"Error\t\tTiempo"<<endl;
		cout<< pi_t - pi_d <<"\t" << t<<endl;
	}
	
	MPI_Finalize();
	return 0;
}
