/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Factorial 1.2
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
	MPI_Status status;
	int nump, id,indiceI,indiceF;
	int reparto;
	int esclavo;
	char fileName[LONGITUD];
	MPI_File fh,ft;
	int elms;

	int j,*numerosEsclavo,repartoEsclavo;
	clock_t t,t_dist,t_master = 0,t_reciev;
	double *timesFact;
	long f[LONGITUD];
	double timeTaken;

	char columna[200];

	int *numRecieved;
	double *timesRecieved;





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
			puts("Maestro\n");

		numerosFichero = FileLines(argv[1]);

		reparto = ceil(lines/nump);
		printf("reparto = %d",reparto);
		t_dist = clock();
		for(esclavo = 1,indiceI = 0; esclavo < nump; esclavo+=1,indiceI+=reparto)
			MPI_Send(&(numerosFichero[indiceI]),reparto,MPI_INT,esclavo,2,MPI_COMM_WORLD);

		t_dist = clock() - t_dist;

		numRecieved = (int*)malloc(reparto* sizeof(int));
		timesRecieved = (double*)malloc(reparto*sizeof(double));
		sprintf(fileName,"Times_1_2_%s",argv[1]);

		MPI_File_open(MPI_COMM_SELF, fileName,MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);
		MPI_File_write(fh,"Factorial,Time\n",strlen("Factorial,Time\n"), MPI_CHAR,&status);
		sprintf(fileName,"Factorial_Total_%s",argv[1]);
		MPI_File_open(MPI_COMM_SELF, fileName,MPI_MODE_WRONLY |MPI_MODE_APPEND,MPI_INFO_NULL,&ft);

		if(indiceI < (lines -1) ){

			t_master = clock();
			for(;indiceI < lines; indiceI++){
				t = clock();
				factorial ( numerosFichero[indiceI], f);
				t = clock() - t;
				timeTaken = ((double)t)/CLOCKS_PER_SEC;
				sprintf(columna,"%d,%lf\n",numerosFichero[indiceI],timeTaken);
				MPI_File_write(fh,columna,strlen(columna), MPI_CHAR,&status);
			}
			t_master = clock() - t_master;
		}

		t_reciev = clock();
		for(esclavo = 1 ; esclavo < nump; esclavo++){
			MPI_Recv(numRecieved,reparto,MPI_INT,esclavo,3,MPI_COMM_WORLD,&status);
			MPI_Get_elements( &status, MPI_INT, &elms );
			if(elms != reparto){
				printf("Error, recived less elements than sould be it \n");
				printf("elements %d, reparto %d\n",elms,reparto );
				MPI_Finalize();
				return -1;
			}else{
				printf("elements %d, reparto %d\n",elms,reparto );
			}
			MPI_Recv(timesRecieved,reparto,MPI_DOUBLE,esclavo,4,MPI_COMM_WORLD,&status);
			MPI_Get_elements( &status, MPI_DOUBLE, &elms );
			if(elms != reparto){
				printf("Error, recived less elements than sould be it\n");
				MPI_Finalize();
				return -1;
			}else{
				printf("elements %d, reparto %d\n",elms,reparto );
			}
			for(i = 0; i < reparto; i++){
				sprintf(columna,"%d,%lf\n",numRecieved[i],timesRecieved[i]);
				MPI_File_write(fh,columna,strlen(columna), MPI_CHAR,&status);
			}
		}
		t_reciev = clock() - t_reciev;
		sprintf(columna,"Paralelo_%d,%lf\n",nump,(double)(t_dist + t_master + t_reciev)/CLOCKS_PER_SEC);
		MPI_File_write(ft,columna,strlen(columna), MPI_CHAR,&status);
		MPI_File_close(&fh);
		MPI_File_close(&ft);
		free(numRecieved);
		free(timesRecieved);
		free(numerosFichero);
	}
	else{

		//Obtenemos primero el tamño del mensaje que nos van a enviar
		MPI_Probe(0, 2, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &repartoEsclavo);

		numerosEsclavo = (int*)malloc(repartoEsclavo*sizeof(int));
		timesFact = (double*)malloc(repartoEsclavo*sizeof(double));

		MPI_Recv(numerosEsclavo,repartoEsclavo,MPI_INT,0,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		for(j = 0;j < repartoEsclavo;j++){
			t = clock();
			factorial ( numerosEsclavo[j], f);
			t = clock() - t;
			timeTaken = ((double)t)/CLOCKS_PER_SEC;
			timesFact[j] = timeTaken;
		}

		MPI_Ssend(&(numerosEsclavo[0]),repartoEsclavo,MPI_INT,0,3,MPI_COMM_WORLD);
		MPI_Ssend(&(timesFact[0]),repartoEsclavo,MPI_DOUBLE,0,4,MPI_COMM_WORLD);
		free(numerosEsclavo);
		free(timesFact);
	}
	MPI_Finalize();
	return 0;
}
