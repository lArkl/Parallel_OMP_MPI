#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

int main(int argc, char* argv[]){
	const int n = 10;	
	int x[n];
	//Generacion de datos del array
	srand(time(0));
	for(int i=0;i<n;i++){
		x[i] = rand()%100;
	}
	//Prefijos
	int c[n];
	for(int i=0;i<n;i++){
		if(i==0)
			c[i]=x[i];
		else
			c[i] = c[i-1]+x[i];
		cout<<"C_"<<i<<": "<<c[i]<<endl;
	}
	//T(n) = O(n)
	return 0;

/* PRAM
	
	for i:=1 to n pardo
		c[i] := x[i]
	endfor
	
	//j es el salto entre prefijos
	j := 1
	t[1] := c[1]
	while j < n do
		for i:=1 to n-j pardo
			t[i+j] := c[i+j] + c[i]
		endfor
		for i:=1 to n-j pardo
			c[i+j] := t[i+j]
		endfor
		j := j*2
	end while	
	
	T(n) = lg(n)
	P(n) = O(n)
	W(n) = O(nlg(n))
	
	El algoritmo secuencial es T(n) = O(n) > O(lg(n))
=> El tiempo de ejecución del algoritmo secuencial es mayor al del algoritmo paralelo. Por tanto, el algoritmo paralelo es óptimo respecto del secuencial.	
	* */	
}
