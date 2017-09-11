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
	//Suma
	int sum = 0;
	for(int i=0;i<n;i++){
		sum = sum + x[i];
	}
	cout << "Suma: "<<sum<<endl;
	return 0;
	
	/* PRAM
	Tareas  t := n/p 
	for k:=0 to t
		for i:=1 to p pardo
			if i+k*p <= n then
				c[i+k*p] := x[i+k*p]
			endif
		endfor
	endfor
	//La cantidad de elementos del array se divide en 2 en cada iteración
	//consideramos m como la actualización
	m := n/2
	for h := 1 to lg(n)
		t := m/p
		for k:=0 to t
			for i:=1 to p pardo
				if i+k*p < 2*m then
					c[i+k*p] := c[2*(i+k*p)-1] + c[2*(i+k*p)]
				endif
		 	endfor
	 	endfor
	 	m:=m/2
	endfor
	sum := c[1]
	* */
	
	/* Grafo
		 P1  | P2  | P3  | P4  || P1	| 		P2		 | 	P3	   | 		P4		|
		c1	c2	c3	c4	c5	c6	c7	c8	c9	c10	c11	c12	c13	c14	c15	c16
		 \ /	 \ /	 \ /	 \ /	 \ /	 	|	_/			/ ____/		/____/	
h=1	 c1	  c2	  c3	  c4	  c5		c6				c7			c8
		 	\	 /			\ 	  /		\ 	 	/				 \   	___/		
h=2		  c1			  c2				c3						c4
			   \__	____/					 \____	_________/
h=3		   	c1									c2
			   	 \_________		____________/
h=4		   	 				c1

T(n) = O(h) = O(lg(n))
P(n) = O(p)
W(n) = O(plg(n))

El algoritmo secuencial es T(n) = O(n) > O(lg(n))
=> El tiempo de ejecución del algoritmo secuencial es mayor al del algoritmo paralelo. Por tanto, el algoritmo paralelo es óptimo respecto del secuencial.

	*/
	
}
