#include <iostream>
#include <cstdlib>

using namespace std;
const int N=100;
const int G=1;	

class NBody{
	public:
	float pos[3][N];
	float vel[3][N];
	float m[N];
	float dt;
	NBody(float dif){
		dt=dif;
		for(int i=0;i<N;i++){
			m[i]=rand()/(float)RAND_MAX;
			for(int j=0;j<3;j++){
				pos[j][i]=rand()/(float)RAND_MAX;
				vel[j][i]=rand()/(float)RAND_MAX;
			}
		}
	}
	//void force(int n, float post[][],  ,float vel[][], float m[], float dt){
	void force(){
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
	}
	
	void integr(){
		double start = clock();
		float max = 1/dt;
		for(int t=0;t<max;t++){
			force();
		//	cout<<t<<endl;
		}
		double end = clock();
		double cpuTime=(end-start)/(CLOCKS_PER_SEC);
		cout<<cpuTime<<endl;
	}
};

int main(int argc, char *arv[]){
	const float dt=0.001;
	NBody galaxy(dt);
	galaxy.integr();
	return 0;
}
