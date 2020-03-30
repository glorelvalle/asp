/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Factorial 1.3
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"
#include <math.h>
#include <time.h>
#define _GNU_SOURCE
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define LONGITUD 20000
#define DEBUG 1

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


long long  factorialA(long numero){
	long long factorial = 1;
	int i;
	for(i = 1; i <= numero; i++){
		factorial = factorial * i;
		if(factorial > LONG_MAX){
			puts("nos fuimos a la verga\n");
			return factorial;
		}
	}
	return factorial;
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
		//printf("%s\n",tok);
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
	int *numerosFichero;
	FILE *fileWrite;
	int i;
	MPI_Status statusP,status;
	int nump, id,indiceI,indiceF;
	int reparto;
	int esclavo;
	char fileName[LONGITUD];
	MPI_File fh,ft;
	int elms;


	int numeroEsclavo;
	clock_t t, t_firstDist,t_end;
	double timeFact;
	long f[LONGITUD];

	char columna[200];

	int numRecieved;
	double timeRecieved;

	if(argc < 2){
		fprintf(stdout,"Necesario nombre del fichero");
		exit(EXIT_FAILURE);
	}

	// Inicializar MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nump);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);


	/*Maestro*/

	if(id == 0){

		if(DEBUG)
			puts("master");

		numerosFichero = FileLines(argv[1]);
		printf("num process, %d",nump);

		reparto = ceil(lines/nump);
		printf("reparto = %d\n",reparto);

		t_firstDist = clock();

		for(esclavo = 1; esclavo < nump; esclavo++)
			MPI_Send(&(numerosFichero[esclavo]),1,MPI_INT,esclavo,2,MPI_COMM_WORLD);

		t_firstDist = clock() - t_firstDist;

		sprintf(fileName,"Times_1_3_%s",argv[1]);

		MPI_File_open(MPI_COMM_SELF, fileName,MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);
		MPI_File_write(fh,"Factorial,Time\n",strlen("Factorial,Time\n"), MPI_CHAR,&status);
		sprintf(fileName,"Factorial_Total_%s",argv[1]);
		MPI_File_open(MPI_COMM_SELF, fileName,MPI_MODE_WRONLY |MPI_MODE_APPEND,MPI_INFO_NULL,&ft);

		indiceI = esclavo;
		t_end = clock();
		while(indiceI < lines){
			MPI_Recv(&numRecieved,1,MPI_INT,MPI_ANY_SOURCE,3,MPI_COMM_WORLD,&statusP);
			MPI_Recv(&timeRecieved,1,MPI_DOUBLE,MPI_ANY_SOURCE,4,MPI_COMM_WORLD,&statusP);

			sprintf(columna,"%d,%lf\n",numRecieved,timeRecieved);

			MPI_File_write(fh,columna,strlen(columna), MPI_CHAR,&status);
			MPI_Send(&(numerosFichero[indiceI]),1,MPI_INT,statusP.MPI_SOURCE,2,MPI_COMM_WORLD);

			indiceI++;


		}
		for(esclavo = 1; esclavo < nump; esclavo++){

			MPI_Recv(&numRecieved,1,MPI_INT,MPI_ANY_SOURCE,3,MPI_COMM_WORLD,&statusP);
			printf("num %d\n", statusP.MPI_SOURCE);
			MPI_Recv(&timeRecieved,1,MPI_DOUBLE,MPI_ANY_SOURCE,4,MPI_COMM_WORLD,&statusP);
			printf("fact %d\n", statusP.MPI_SOURCE);
			sprintf(columna,"%d,%lf\n",numRecieved,timeRecieved);
			MPI_File_write(fh,columna,strlen(columna), MPI_CHAR,&status);

		}
		t_end = clock() - t_end;
		sprintf(columna,"Paralelo_1_Dist_%d,%lf\n",nump,(double)(t_firstDist + t_end)/CLOCKS_PER_SEC);
		MPI_File_write(ft,columna,strlen(columna), MPI_CHAR,&status);
		indiceI = -1;
		for(esclavo=1; esclavo < nump; esclavo++)
			MPI_Send(&(indiceI),1,MPI_INT,esclavo,2,MPI_COMM_WORLD);

		MPI_File_close(&fh);
		MPI_File_close(&ft);
		free(numerosFichero);
	}
	else{
		while(true){
			MPI_Recv(&numeroEsclavo,1,MPI_INT,0,2,MPI_COMM_WORLD,&statusP);
			if(numeroEsclavo == -1)
				break;
			t = clock();
			factorial ( numeroEsclavo, f);
			t = clock() - t;
			timeFact = ((double)t)/CLOCKS_PER_SEC; // in seconds


			MPI_Ssend(&(numeroEsclavo),1,MPI_INT,0,3,MPI_COMM_WORLD);
			MPI_Ssend(&(timeFact),1,MPI_DOUBLE,0,4,MPI_COMM_WORLD);

		}
	}
	MPI_Finalize();

	return 0;
}
