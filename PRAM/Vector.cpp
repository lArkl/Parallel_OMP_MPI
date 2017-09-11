#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

int main(int argc, char* argv[]){
	const int n = 10;	
	int a[n],b[n],c[n];
	srand(time(0));
	int s[n],y[n];
	if(n==1)
		s[0]=x[0];
		return 0;
	for(int i=0; i<n/2; i++){
		y[i]=x[2*i]+x[2*i+1];
	}
	for(int i=1; i<n; i++){
		if(i%2!=0)
			s[i]=y[(i+1)/2];
		else
			s[i]=y[i/2]+x[i];
	}

	
	
	
	return 0;
}
