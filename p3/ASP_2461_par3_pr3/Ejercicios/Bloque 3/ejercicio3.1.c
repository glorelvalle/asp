/* --------------------------------------------------------------------------
* Práctica 3 de Arquitectura de Sistemas Paralelos
*
* Suma N enteros. MPI + OpenMP.
* (Ejercicio 3.1)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define N 1000000000
#define F "sumPerformance.txt"

int main(int argc, char **argv)
{

	int nthr,nproc,i,j,private_sum = 0, elementsProc = 0;
	int nump, id;
	int arg;
	FILE *fp;
	char *data,*partialSum;
	clock_t tStart, tEnd;
	unsigned long long sum= 0,threadSum = 0;

		// Inicializar MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nump); /*determina el número de procesos que están actualmente asociados al comunicador*/
	MPI_Comm_rank(MPI_COMM_WORLD, &id); /*determina el rango (identificador) del proceso que lo llama dentro del comunicador seleccionado*/

	if(id == 0)
	{
		data = (char*)malloc(sizeof(char) * N);
		for(i = 0; i < N; i++)
			data[i] = 2;
		fp = fopen(F,"a");
	}

	/*
	arg = atoi(argv[1]);
	omp_set_num_threads(arg);
	*/
	nthr = omp_get_max_threads();

	elementsProc = ceil(N/nump);
	partialSum = (char*)malloc(sizeof(char)*elementsProc);
	/*
	MPI_Scatter(
	    void* send_data,
	    int send_count,
	    MPI_Datatype send_datatype,
	    void* recv_data,
	    int recv_count,
	    MPI_Datatype recv_datatype,
	    int root,
	    MPI_Comm communicator)
	*/
	printf("elementsProc :%d\n",elementsProc);
	MPI_Scatter(
		data,
		elementsProc,
		MPI_CHAR,
		partialSum,
		elementsProc,
		MPI_CHAR,
		0,
		MPI_COMM_WORLD);

	printf("Me han pedido que lance %d hilos\n",nthr);

	tStart = clock();
	#pragma omp parallel for reduction(+: threadSum) \
							private(j) shared(partialSum)

		for(j = 0 ; j < elementsProc; j++)
			threadSum+= partialSum[j];


	MPI_Reduce(&threadSum,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	tEnd = clock();
	if(id == 0)	{
		printf("N ->%d\nSuma->%llu\n,Time->%lf\n",N,sum,(double)(tEnd -tStart)/CLOCKS_PER_SEC);
		fprintf(fp,"MpiOpenMp,%lf",(double)(tEnd-tStart)/CLOCKS_PER_SEC);
		free(data);
		fclose(fp);
	}
	free(partialSum);
	MPI_Finalize();
	return 0;
}
