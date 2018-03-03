#include <assert.h>
#include <stdio.h>
#include <omp.h>

int A[2][2] = {1, 2, 3, 4};

void f(int n, int B[n][n], int C[])
{
	int D[2][2] = {1, 2, 3, 4};
	int E[n][n];
	assert(n >= 2);
	E[1][1] = 5;
#pragma omp parallel firstprivate(B, C, D, E)
	{
#pragma omp single
		{
		printf("size of B: %d\n",sizeof(B));
		printf("size of C: %d\n",sizeof(C));
		printf("size of D: %d\n",sizeof(D));
		printf("size of E: %d\n",sizeof(E));
		}
/*
		assert(sizeof(B) == sizeof(int (*)[n]));
		assert(sizeof(C) == sizeof(int*));
		assert(sizeof(D) == 4 * sizeof(int));
		assert(sizeof(E) == n * n * sizeof(int));
*/
		/* Private B and C have values of original B and C. */
#pragma omp single
		{
		printf("A_11: %d B_11: %d C_3: %d\n",A[1][1],B[1][1],C[3]);
		printf("D_11: %d E_11: %d\n",D[1][1],E[1][1]);
		}
/*		assert(&B[1][1] == &A[1][1]);
		assert(&C[3] == &A[1][1]);
		assert(D[1][1] == 4);
		assert(E[1][1] == 4);
*/	} 
}

int main() {
	 f(4, A, A[0]);
	 return 0;
}

