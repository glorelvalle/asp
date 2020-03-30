/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Factorial 1
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#define _GNU_SOURCE
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define LONGITUD 20000

int lines = 0;

void mult(long fact[],int j)
{
 int i,ll;
 long x;

 ll=0;
 for(i=0;i<=(LONGITUD-1);i++)
 {
  x=fact[i]*j+ll;
  fact[i]=x%10000;
  ll=x/10000;
 }
}


void factorial(int num,long fact[])
{
 int j;

	for(j=0;j<LONGITUD;j++)
		fact[j]=0L;

	fact[0]=1L;

	for(j=1;j<=num;j++)
		mult(fact,j);
}

void imprime_fact ( long fact[] )
{
	int p, j;

	p = 1;
	for ( j = LONGITUD-1; j>=0; j-- )
		if ( fact[j] || !p )
		{

			fprintf(stdout,"%04ld",fact[j]);
			p = 0;
		}
	fprintf(stdout,"\n");
}
int* FileLines(char*nombreFichero){
	char * line = NULL,*tok;
    size_t len = 0;
    int read;
	int *numeros;
    FILE *fp;
	int i =0;
	fp = fopen(nombreFichero,"r");
	if(fp == NULL){
		perror("Error al abrir el fichero");
		exit(EXIT_FAILURE);
	}
	while ((read = getline(&line, &len, fp)) != -1) {
        lines++;
    }
    fclose(fp);
	numeros = (int*)malloc(sizeof(int)*lines);
	fp = fopen(nombreFichero,"r");
	if(fp == NULL){
		perror("Error al abrir el fichero por segunda vez");
		exit(EXIT_FAILURE);
	}
    while ((read = getline(&line, &len, fp)) != -1) {
        tok = strtok(line,"\n");
		numeros[i]  = atoi(tok);
		i++;
    }
	fclose(fp);
	if (line)
        free(line);
    return numeros;
}

int main(int argc,char**argv)
{
	int i,*fileNumbers;
	int esclavo;
	FILE *fp,*ft;
	double time_taken;
	clock_t t,total;
	long f[LONGITUD];
	char fileName[200];
	if(argc < 2){
		fprintf(stdout,"Necesario nombre del fichero");
		exit(EXIT_FAILURE);
	}
	sprintf(fileName,"Factorial_Serie_%s",argv[1]);
	fp = fopen(fileName,"w");
	if(!fp){
		perror("Error fichero: Factorial_Serie.txt");
		exit(EXIT_FAILURE);
	}
	sprintf(fileName,"Factorial_Total_%s",argv[1]);
	ft = fopen(fileName,"a");

	if(!ft){
		perror("Error fichero: Factorial_Total.txt");
		exit(EXIT_FAILURE);
	}
	fprintf(fp,"Factorial,Time\n");
	fprintf(ft,"Type,Total\n");
	total = clock();
	fileNumbers = FileLines(argv[1]);
	for ( i = 0; i<lines; i++ )
	{
		t = clock();
		factorial ( fileNumbers[i], f );
		t = clock() - t;
		time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
		fprintf(fp,"%d,%lf\n",i,time_taken);
		//imprime_fact ( f );
	}

	fprintf(ft,"serie,%lf\n",(double)(clock() -total)/CLOCKS_PER_SEC);
	fclose(fp);
	return 0;
}
