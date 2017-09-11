#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

int main(int argc, char* argv[]){
	const int n = 10;	
	int a[n],b[n],c[n];
	srand(time(0));
	//Ingreso de datos a vectores
	for(int i=0;i<n;i++){
		a[i] = rand()%100;
		b[i] = rand()%100;
	}
	
	for(int i=0;i<n;i++){
		c[i] = a[i]+b[i];
		cout<<"C_"<<i<<": "<<c[i]<<endl;
	}
	/*PRAM
	
	for i:=1 to n pardo
		c[i]:= a[i] + b[i]
	endfor 
	
	*/
	
	return 0;
}
