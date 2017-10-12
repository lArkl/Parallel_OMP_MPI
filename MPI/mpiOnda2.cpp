#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <mpi.h>

using namespace std;

#define MAXPOINTS 1000
#define MAXSTEPS 1000
#define MINPOINTS 20
#define PI 3.14159265

void init_param(void);
void init_line(void);
void update (void);
void printfinal (void);

int nsteps,                 	/* number of time steps */
    tpoints, 	     		/* total points along string */
    rcode,                  	/* generic return code */
	rounds;
	
double values[MAXPOINTS+2], 	/* values at time t */
       oldval[MAXPOINTS+2], 	/* values at time (t-dt) */
       newval[MAXPOINTS+2], 	/* values at time (t+dt) */
		//oldval2[MAXPOINTS+2], 	/* values at time (t-dt) */
			total[MAXPOINTS+2];
/***************************************************************************
 *	Obtains input values from user
 ***************************************************************************/
void init_param(void)
{
   char tchar[8];

   /* set number of points, number of iterations */
   tpoints = 0;
   nsteps = 0;
   while ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS)) {
      cout<<"Enter number of points along vibrating string ["<<MINPOINTS<<","<< MAXPOINTS<<"]: ";
      cin>>tchar;
      tpoints = atoi(tchar);
//      if ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS))
//         printf("Invalid. Please enter value between %d and %d\n", 
//                 MINPOINTS, MAXPOINTS);
      }
   while ((nsteps < 1) || (nsteps > MAXSTEPS)) {
      cout<<"Enter number of time steps [1-"<<MAXSTEPS<<"]: ";
      cin>>tchar;
      nsteps = atoi(tchar);
//      if ((nsteps < 1) || (nsteps > MAXSTEPS))
 //        printf("Invalid. Please enter value between 1 and %d\n", MAXSTEPS);
      }

	cout<<"Using points ="<<tpoints<<", steps = "<<nsteps<<endl;
}

/***************************************************************************
 *     Initialize points on line
 **************************************************************************/
void init_line(void)
{
	int i, j;
	double x, fac, k, tmp;

	/* Calculate initial values based on sine curve */
	fac = 2.0 * PI;
	k = 0.0; 
	tmp = tpoints - 1;
	for (j = 1; j <= tpoints; j++) {
		x = k/tmp;
		total[j] = sin (fac * x);
		k = k + 1.0;
	} 

	/* Initialize old values array */
	for (i = 1; i <= tpoints; i++) 
		oldval[i] = total[i];
}

/***************************************************************************
 *      Calculate new values using wave equation
 **************************************************************************/
double ifValues(int i){
	if(i!=-1 && i!=tpoints)
		return values[i];
	return 0;		
}


void do_math(int i)
{
   double dtime, c, dx, tau, sqtau;
	/*
   dtime = 0.3;
   dx = 1.0;
   */
   dtime = 0.002;
   dx = 1.0/tpoints;
   c = 1.0;
   tau = (c * dtime / dx);
   sqtau = tau * tau;
   
   newval[i] = (2.0 * values[i]) - oldval[i] 
               + (sqtau * (ifValues(i-1) - (2.0 * values[i]) + ifValues(i+1)));
}

/***************************************************************************
 *     Update all values along line a specified number of times
 **************************************************************************/


void update(int me, int numprocs)
{
   //MPI_Scatter(&oldval,rounds+2,MPI_DOUBLE, &oldval2,rounds,MPI_DOUBLE,0,MPI_COMM_WORLD);
	/* Update values for each time step */
   for (int i = 1; i<= nsteps; i++) {

		//MPI_Scatter(&total,rounds+2,MPI_DOUBLE, &values,rounds,MPI_DOUBLE,0,MPI_COMM_WORLD);
		char buffer[20];
		
		sprintf (buffer, "%04d.dat", i);
		
      int first = me*rounds;
		/* Exchange data with "left-hand" neighbor */
		if (me > 0) {
         MPI_Send(&values[first], 1, MPI_DOUBLE, me-1,0, MPI_COMM_WORLD);
         MPI_Recv(&values[first-1], 1, MPI_DOUBLE, me-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		/* Exchange data with "right-hand" neighbor */
		if (me< numprocs-1) {
         MPI_Send(&values[first+rounds-1], 1, MPI_DOUBLE, me+1, 1, MPI_COMM_WORLD);
         MPI_Recv(&values[first+rounds], 1, MPI_DOUBLE, me+1, 0,
                   MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }

		MPI_Barrier(MPI_COMM_WORLD);
		/* Update points along line for this time step */
      for (int j = first; j < first+rounds; j++) {
         /* global endpoints */
            do_math(j);
		}
      
      /* Update old values with new values */
      for (int j = first; j < first+rounds; j++) {
         oldval[j] = values[j];
         values[j] = newval[j];
	//		cout<<values[j]<<endl;
		}
		
		//newval
		MPI_Barrier(MPI_COMM_WORLD);
		//MPI_Gather(&values,rounds,MPI_DOUBLE, &total,rounds,MPI_DOUBLE,0,MPI_COMM_WORLD);
		
	//	cout<<"Step: "<<i<<endl;
		for(int j=0; j<numprocs ;j++){
			FILE *fp;
			fp = fopen(buffer, "a");
			if (fp==NULL){
				printf("Something failed during the file opening :'v \n");
				exit(0);	
			}
			if(me==j){		
				for( int k=first;k<first+rounds;k++){
					fprintf(fp, "%f, %f\n", k/(double)tpoints,values[k]);
		//			printf("%f,%f\n", k/(double)tpoints,values[k]);
				}
			}
			fclose(fp);
			MPI_Barrier(MPI_COMM_WORLD);
		}
		for(int j=0;j<numprocs;j++){
			if(me==0){
				FILE *fp;
				fp = fopen(buffer, "a");
				if (fp==NULL){
					printf("Something failed during the file opening :'v \n");
					exit(0);	
				}
				for( int k=0;k<tpoints;k++){
					fprintf(fp, "%f, %f\n", k/(double)tpoints,total[k]);
				}
			
				fclose(fp);
			}		
		}
	}
//	MPI_Gather(&oldval2,rounds+2,MPI_DOUBLE, &oldval,rounds+2,MPI_DOUBLE,0,MPI_COMM_WORLD);
		
}

/***************************************************************************
 *     Print final results
 **************************************************************************/
void printfinal()
{
	for (int i = 1; i <= tpoints; i++) {
		printf("%+6.4f ", total[i]);
	if (i%10 == 0)
		cout<<endl;
	}
}

/***************************************************************************
 *	Main program
 **************************************************************************/

int main(int argc, char *argv[])
{

	tpoints = 100;
	nsteps = 300;
	
	int me,numprocs;	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	rounds = tpoints/numprocs;
	double start = MPI_Wtime();
	if(me==0){
		init_line();
		cout<<"Updating all points for all time steps..."<<endl;
	}	
	MPI_Barrier(MPI_COMM_WORLD);
	update(me,numprocs);
	if(me==0){
		cout<<"Printing final results..."<<endl;
		printfinal();
		cout<<endl<<"Done."<<endl;
		double end = MPI_Wtime();
		cout<<"Procesos: "<<numprocs<<endl;
		cout<<"Tiempo: "<<end-start<<endl;
	}
	MPI_Finalize();
	return 0;
}
