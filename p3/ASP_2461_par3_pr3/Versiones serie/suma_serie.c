#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 1000000000
#define F "sumPerformance.txt"
int main(){
	char *datos;
	int i;
	clock_t tStart, tEnd; 
	unsigned long long suma=0;
	FILE *fp;

	datos = (char*)malloc(sizeof(char) * N);
	fp = fopen(F,"a");
	
	for(i = 0; i < N; i++)
		datos[i] = 2;
	
	tStart = clock();
	for(i = 0; i < N; i++)
		suma += datos[i];
	tEnd = clock();
	fprintf(fp,"Serie,%lf\n",(double)(tEnd-tStart)/CLOCKS_PER_SEC);	
	printf("%llu\n",suma);
	free(datos);
	fclose(fp);
}
