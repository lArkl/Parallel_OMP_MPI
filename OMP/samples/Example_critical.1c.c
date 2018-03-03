/*
* @@name:	critical.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
#include<stdio.h>
#include<omp.h>

int iRand(int iMin, int iMax) {
return  rand() % iMax + iMin;
}

//int dequeue(float *a, int n){return a[iRand(0,n)];};
int dequeue(float *a, int n){return a[1];};
void work(int i, float *a){
a[i]=a[i+1];
printf("thread: %d i: %d, a: %f\n",omp_get_thread_num(),i, a[i]);
};

void critical_example(float *x, float *y, int n)
{
  int ix_next, iy_next;

  #pragma omp parallel num_threads(4) shared(x, y) private(ix_next, iy_next)
  {
    #pragma omp critical (xaxis)
      ix_next = dequeue(x,n);
      work(ix_next, x);

    #pragma omp critical (yaxis)
      iy_next = dequeue(y,n);
      work(iy_next, y);
  }

}

int main(){
int i,n=10;
float x[n],y[n];

for(i=0;i<n;i++) {
	x[i]=i;
	y[i]=i+n;
	printf("x: %f, y: %f \n",x[i],y[i]);
}

critical_example(x,y,n);	

for(i=0;i<n;i++) {
//	x[i]=i;
//	y[i]=i+n;
//	printf("x: %f, y: %f \n",x[i],y[i]);
}
}
