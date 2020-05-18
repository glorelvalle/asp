#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 1000000000
#define F "sumPerformance.txt"
int main(int argc, char **argv)
{

	int nthr,nproc,i,j,private_sum = 0;
	int arg;
	char *data,*partialSum;
	clock_t tStart, tEnd; 
	FILE *fp;
	unsigned long long sum= 0;
	
	data = (char*)malloc(sizeof(char) * N);
	
	for(i = 0; i < N; i++)
		data[i] = 2;
	
	fp = fopen(F,"a");
	nproc = omp_get_num_procs();
	printf("HAY %d cores disponibles\n",nproc);

	nthr = omp_get_max_threads();
	
	printf("Me han pedido que lance %d hilos\n",nthr);
	tStart = clock();
	#pragma omp parallel for reduction(+: sum) \
							private(j) shared(data)
	
		for(j = 0 ; j < N; j++)
			sum+= data[j];
	
	tEnd = clock();
	fprintf(fp,"OpenMp,%lf\n",(double)(tEnd-tStart)/CLOCKS_PER_SEC);
	printf("%llu\n",sum);	
	free(data);
	fclose(fp);
	return 0;
}
