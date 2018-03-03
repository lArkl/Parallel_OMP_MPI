#include <iostream>
#include <cstdlib>
#include<omp.h>

using namespace std;
const int N=1024;
const int G=1;	

class NBody{
	public:
	float pos[3][N];
	float vel[3][N];
	float m[N];
	float dt;
	NBody(float dif){
		dt=dif;
		//#pragma omp for
		for(int i=0;i<N;i++){
			m[i]=rand()/(float)RAND_MAX;
			for(int j=0;j<3;j++){
				pos[j][i]=rand()/(float)RAND_MAX;
				vel[j][i]=rand()/(float)RAND_MAX;
			}
		}
		//#pragma omp barrier
	}
	//void force(int n, float post[][],  ,float vel[][], float m[], float dt){
	void force(){
		#pragma omp for
		for(int i=0;i<N;i++){
			float a_x=0;
			float a_y=0;
			float a_z=0;
			float my_rx = pos[0][i];
			float my_ry = pos[1][i];
			float my_rz = pos[2][i];
			for(int j=0;j<N;j++){
				if(j!=i){
					float dx = pos[0][j]-my_rx;
					float dy = pos[1][j]-my_rx;
					float dz = pos[2][j]-my_rz;
					a_x += G*m[j]/(dx*dx);
					a_y += G*m[j]/(dy*dy);
					a_z += G*m[j]/(dz*dz);
				}
			}
			//update
			vel[0][i]+=a_x*dt;
			pos[0][i]+=vel[0][i]*dt;
			vel[1][i]+=a_y*dt;
			pos[1][i]+=vel[1][i]*dt;
			vel[2][i]+=a_z*dt;
			pos[2][i]+=vel[2][i]*dt;
		}
		#pragma omp barrier
	}
	
	void integr(){
		float max = 1/dt;
		for(int t=0;t<max;t++){
			force();
		}
	}
};

int main(int argc, char *arv[]){
	const float dt=0.001;
	int nthreads = 4;
	omp_set_num_threads(nthreads);
	NBody galaxy(dt);
	#pragma omp parallel firstprivate(dt) shared(galaxy)
	{
		size_t start = omp_get_wtime();
		galaxy.integr();
		#pragma omp single
		{
			cout<<"OMP Time: "<<omp_get_wtime()-start<<endl;
		}
		
	}
	return 0;
}
