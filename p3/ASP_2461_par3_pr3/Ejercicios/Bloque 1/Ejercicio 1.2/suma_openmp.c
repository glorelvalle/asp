/* --------------------------------------------------------------------------
* Práctica 3 de Arquitectura de Sistemas Paralelos
*
* Suma N enteros. OpenMP.
* (Ejercicio 1.2)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 100

int main(int argc, char **argv)
{

	int nthr,nproc,i;
	int arg;
	char *data,*partialSum;
	unsigned long long suma= 0;

	data = (char*)malloc(sizeof(char) * N);

	for(i = 0; i < N; i++)
		data[i] = 2;



	nproc = omp_get_num_procs();
	printf("HAY %d cores disponibles\n",nproc);

	//arg = atoi(argv[1]);
	//omp_set_num_threads(arg);
	nthr = omp_get_max_threads();
	partialSum = (char*)malloc(sizeof(char)*nthr);
	printf("Me han pedido que lance %d hilos\n",nthr);

	#pragma omp parallel shared(data)
	{
		int private_sum = 0,j, tid;
		tid = omp_get_thread_num();
		partialSum[tid] = 0;

		printf("hola,soy el hilo %d de %d\n -> private_sum: %d\n",tid,nthr,private_sum);
		//#pragma omp for
			for(j = tid ; j < N; j+=nthr){

				private_sum+= data[j];
			}
		printf("priv sum :%d -> tid:%d\n",private_sum,tid);
		partialSum[tid] = private_sum;
	}
	for(i = 0; i < nthr; i++)
	{
		printf("partical sum: %d\n",partialSum[i]);
		suma +=partialSum[i];
	}
	printf("%llu\n",suma);
	free(data);
	free(partialSum);
	return 0;
}
