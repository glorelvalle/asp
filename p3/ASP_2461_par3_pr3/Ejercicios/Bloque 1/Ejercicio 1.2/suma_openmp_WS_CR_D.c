/* --------------------------------------------------------------------------
* Práctica 3 de Arquitectura de Sistemas Paralelos
*
* Suma N enteros. OpenMP: WS + rep.dinamico.
* (Ejercicio 1.2)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define N 1000000000
#define F "sumPerformance.txt"
int main(int argc, char **argv)
{

	int nthr,nproc,i,j,private_sum = 0;
	int arg,reparto;
	char *data,*partialSum;
	clock_t tStart, tEnd;
	FILE *fp;
	unsigned long long sum= 0;

	data = (char*)malloc(sizeof(char) * N);

	for(i = 0; i < N; i++)
		data[i] = 2;


	nproc = omp_get_num_procs();
	printf("HAY %d cores disponibles\n",nproc);
	fp = fopen(F,"a");

	nthr = omp_get_max_threads();

	printf("Me han pedido que lance %d hilos\n",nthr);
	reparto = ceil(N/(4*nproc));
	tStart = clock();

	#pragma omp parallel for reduction(+: sum) \
							private(j) shared(data) schedule(dynamic,reparto)

		for(j = 0 ; j < N; j++)
			sum+= data[j];

	tEnd = clock();


	printf("%llu\n",sum);
	fprintf(fp,"OpenMp_DS,%lf\n",(double)(tEnd-tStart)/CLOCKS_PER_SEC);
	free(data);
	fclose(fp);
	return 0;
}
