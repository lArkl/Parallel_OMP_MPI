#include<iostream>
#include<cstdlib>
#include<ctime>

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
	int const max = 10000;
	ru.seed(-max,max);
	int nc = 0,ns =0;
	int dc = 0,ds =0;
	double x,y;
	int num_trails=100;
	cout<<"Exp\tPi-Pn\t\tPi-Pd"<<endl;
	for(int exp=2; exp<=8; exp++){
		for(int i=0;i<num_trails;i++){
			x = ru.nrand();
			y = ru.nrand();
			if(x*x + y*y<1)
				nc++;
			else ns++;
			x = ru.drandom()/max;
			y = ru.drandom()/max;
			if(x*x + y*y<1)
				dc++;
			else ds++;
		}
		double pi_n = 4*nc/(double)(nc+ns);
		double pi_d = 4*dc/(double)(dc+ds);
		cout<< exp << "\t" << pi_t-pi_n << "\t" << pi_t-pi_d << endl;
		num_trails*=10;
	}
	return 0;
}
