#include <iostream>
#include <cstdlib>
#include<ctime>
#include <mpi.h>
#include<omp.h>

using namespace std;
const int G=1;	

class Body{
	public:
	float pos[3];
	float vel[3];
	float m;
	Body(){
		m = rand()/(float)RAND_MAX;
		for(int j=0;j<3;j++){
			pos[j]=rand()/(float)RAND_MAX;
			vel[j]=rand()/(float)RAND_MAX;
		}	
	}
	void update(float dt,float a_x,float a_y,float a_z){
		vel[0]+=a_x*dt;
		pos[0]+=vel[0]*dt;
		vel[1]+=a_y*dt;
		pos[1]+=vel[1]*dt;
		vel[2]+=a_z*dt;
		pos[2]+=vel[2]*dt;
	}
};
/*	
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

*/

int main(int argc, char *argv[]){
	const float dt=0.001;
	
	
	int me,numprocs;	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	srand(me*time(0));
	float max = 1/dt;
	int N=1024;
	int rounds = N/numprocs;
	float start = MPI_Wtime();
	Body bodies[rounds];
	MPI_Barrier(MPI_COMM_WORLD);
	for(int t=0;t<max;t++){
		//force
		for(int proc=0;proc<numprocs;proc++){
			for(int i=0;i<rounds;i++){
				float a_x=0;
				float a_y=0;
				float a_z=0;
				float my_rx = bodies[i].pos[0];
				float my_ry = bodies[i].pos[1];
				float my_rz = bodies[i].pos[2];
				MPI_Barrier(MPI_COMM_WORLD);
				MPI_Bcast(&my_rx, 1,MPI_FLOAT,proc,MPI_COMM_WORLD);
				MPI_Bcast(&my_ry, 1,MPI_FLOAT,proc,MPI_COMM_WORLD);
				MPI_Bcast(&my_rz, 1,MPI_FLOAT,proc,MPI_COMM_WORLD);
				#pragma omp parallel num_threads(2) default(shared)
				{
					#pragma omp for
					for(int j=0;j<rounds;j++){
							if(j!=i){
								float dx = bodies[j].pos[0]-my_rx;
								float dy = bodies[j].pos[1]-my_rx;
								float dz = bodies[j].pos[2]-my_rz;
								a_x += G*bodies[j].m/(dx*dx);
								a_y += G*bodies[j].m/(dy*dy);
								a_z += G*bodies[j].m/(dz*dz);
							}
					}
					//#pragma omp barrier
				}
				MPI_Barrier(MPI_COMM_WORLD);
				float sumx,sumy,sumz;
				MPI_Reduce(&sumx,&a_x,1,MPI_FLOAT,MPI_SUM,proc,MPI_COMM_WORLD);
				MPI_Reduce(&sumy,&a_y,1,MPI_FLOAT,MPI_SUM,proc,MPI_COMM_WORLD);
				MPI_Reduce(&sumz,&a_z,1,MPI_FLOAT,MPI_SUM,proc,MPI_COMM_WORLD);
				if(me==proc){
					a_x = sumx;
					a_y = sumy;
					a_z = sumz;
					bodies[i].update(dt,a_x,a_y,a_z);
				}MPI_Barrier(MPI_COMM_WORLD);
			}
		}
	}
	if(me==0){
		cout<<"Hybrid Time: "<<MPI_Wtime()-start<<endl;
	}

	MPI_Finalize();
	return 0;
}
