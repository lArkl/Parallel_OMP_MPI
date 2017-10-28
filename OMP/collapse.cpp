#include <stdio.h>
int main()
{
int j, k, jlast=0, klast=0;
#pragma omp parallel
{
#pragma omp for collapse(2) lastprivate(jlast, klast)
	for (k=1; k<=2; k++)
		for (j=1; j<=3; j++)
		{
			jlast=j;
			klast=k;
		}
#pragma omp single
	printf("%d %d\n", klast, jlast);
}

}


