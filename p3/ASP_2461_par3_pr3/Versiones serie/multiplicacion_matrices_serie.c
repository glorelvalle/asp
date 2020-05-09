#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define F "mulPerformance.txt"

int main(int argc ,char **argv){
	int tamMatriz=0,i=0,j=0,k=0;
	int **matrizA,**matrizB,**matrizC;
	clock_t tStart, tEnd; 
	FILE *fp;
	
	printf("Introduzca tamMatriz:\n");
	scanf("%d",&tamMatriz);

	matrizA = (int**)malloc(sizeof(int*)*tamMatriz);
	if(matrizA == NULL){
		puts("a");
		return -1;
	}
	matrizB = (int**)malloc(sizeof(int*)*tamMatriz);
	if(matrizB == NULL){
		puts("b");
		return -1;
	}
	matrizC = (int**)malloc(sizeof(int*)*tamMatriz);
	if(matrizC == NULL){
		puts("c");
		return -1;
	}
	fp = fopen(F,"a");
	for(i = 0; i < tamMatriz; i++){
		matrizA[i] = (int*)malloc(sizeof(int)*tamMatriz);
		matrizB[i] = (int*)malloc(sizeof(int)*tamMatriz);
		matrizC[i] = (int*)malloc(sizeof(int)*tamMatriz);
	}
	
	for(i = 0; i < tamMatriz; i++){
		for(j = 0; j < tamMatriz; j++){
			matrizA[i][j] = 1;
			if(i == j)
				matrizB[i][j] = 1;
			else
				matrizB[i][j] = 0;
		}
	}
	
	tStart = clock();
	for(i = 0; i < tamMatriz; i++){
		for (j = 0; j < tamMatriz; j++){
			for(k = 0; k < tamMatriz; k++){
				matrizC[i][j] += matrizA[i][k] * matrizB[j][k];
			}
		
		}
	}
	tEnd = clock();
	fprintf(fp,"serie,%lf\n",(double)(tEnd-tStart)/CLOCKS_PER_SEC);
	/*	
	for(i = 0; i < tamMatriz; i++){
		for(j = 0; j < tamMatriz; j++){
			printf("matrizC[%d][%d] = %d\n",i,j,matrizC[i][j]);
		}
	}*/
	for(i = 0 ; i < tamMatriz; i++){
		free(matrizA[i]);
		free(matrizB[i]);
		free(matrizC[i]);
	}
	free(matrizA);
	free(matrizB);
	free(matrizC);
	fclose(fp);
	return EXIT_SUCCESS;
}
