/* --------------------------------------------------------------------------
* Práctica 1 de Arquitectura de Sistemas Paralelos
*
* multiplicacion_nxn
* (Ejercicio 1.3)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define DEBUG 1
#define M 1
#define M2 2
#define M3 3
#define M4 4
#define M5 5


int main(int argc, char **argv){
	int tamMatriz,i,j,k;

	//srand(time(NULL));   // Initialization, should only be called once.
	int random;


	MPI_Status status;
	int nump, id,indiceI,indiceF;
	int reparto;

	// Inicializar MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nump); /*determina el número de procesos que están actualmente asociados al comunicador*/
	MPI_Comm_rank(MPI_COMM_WORLD, &id); /*determina el rango (identificador) del proceso que lo llama dentro del comunicador seleccionado*/

	/*Maestro*/
	if(id == 0){
		int esclavo;

		if(DEBUG)
			puts("maestro");

		puts("\n");
		puts("Introduzca tam matriz:");
		scanf("%d",&tamMatriz);


		reparto = ceil(tamMatriz/nump);

		int matriz[tamMatriz*tamMatriz], matriz2[tamMatriz*tamMatriz];
		int matrizB[tamMatriz*tamMatriz];
		for(i = 0; i< tamMatriz;i++){
			for(j = 0; j<tamMatriz;j++){
				matriz[tamMatriz*i + j] = 1 ;
				matriz2[tamMatriz*i + j] = 0;
				if(i == j)
					matrizB[tamMatriz*i + j] = 1;
				else
					matrizB[tamMatriz*i + j] = 0;
			}
		}

		for(esclavo = 1,indiceI=0;esclavo<nump;esclavo+=1,indiceI+=reparto){
			MPI_Send(&tamMatriz,1,MPI_INT,esclavo,M,MPI_COMM_WORLD);
			MPI_Send(&(matriz[tamMatriz*indiceI]),reparto*tamMatriz,MPI_INT,esclavo,M4,MPI_COMM_WORLD);
			MPI_Send(&(matrizB[0]),tamMatriz*tamMatriz,MPI_INT,esclavo,M5,MPI_COMM_WORLD);

		}
		if((reparto * nump) < tamMatriz){
			printf("asdfasdfasdf\n");
			for(;indiceI<tamMatriz;indiceI++){
				for(j = 0; j < tamMatriz; j++){
					for(k = 0; k < tamMatriz; k++)
						matriz2[tamMatriz*indiceI + j] += matriz[tamMatriz*indiceI + k] * matrizB[tamMatriz*j + k];

				}
			}
		}

		int matrizRes[reparto*tamMatriz];

		for(esclavo = 1,indiceI=0;esclavo<nump;esclavo+=1,indiceI+=reparto){
			MPI_Recv(matrizRes,reparto*tamMatriz,MPI_INT,esclavo,2,MPI_COMM_WORLD,&status);
			for(i = indiceI,k = 0; i < indiceI+reparto;i+=1,k+=1){
				for(j=0;j<tamMatriz;j++)
					matriz2[tamMatriz*i + j] = matrizRes[tamMatriz*k + j];
			}
		}

		for(i = 0; i< tamMatriz; i++){
			for(j = 0; j < tamMatriz; j++)
				printf("matriz[%d][%d] = %d\n",i,j,matriz2[tamMatriz*i + j]);
		}

	}else{//Esclavo

		if(DEBUG)
			printf("esclavo");
		int repartoEsclavo,tamMatrizAux;

		MPI_Recv(&tamMatrizAux,1,MPI_INT,0,M,MPI_COMM_WORLD,&status);
		repartoEsclavo = ceil(tamMatrizAux/nump);

		int matrizIndice[repartoEsclavo*tamMatrizAux],matrizEsclavo[tamMatrizAux*tamMatrizAux];
		int matrizR[repartoEsclavo*tamMatrizAux];


		MPI_Recv(matrizIndice,repartoEsclavo*tamMatrizAux,MPI_INT,0,M4,MPI_COMM_WORLD,&status);
		MPI_Recv(matrizEsclavo,tamMatrizAux*tamMatrizAux,MPI_INT,0,M5,MPI_COMM_WORLD,&status);


		for(i = 0; i< repartoEsclavo;i++){
			for(j = 0; j<tamMatrizAux;j++)
				matrizR[tamMatrizAux*i + j] = 0;
		}

		for(i=0;i < repartoEsclavo; i++){
			for(j = 0; j < tamMatrizAux; j++){
				for(k = 0; k < tamMatrizAux; k++)
					matrizR[tamMatrizAux * i + j] += matrizIndice[tamMatrizAux*i + k] * matrizEsclavo[tamMatrizAux* j+ k];

			}
		}

		MPI_Send(&(matrizR[0]),repartoEsclavo*tamMatrizAux,MPI_INT,0,2,MPI_COMM_WORLD);
	}
	  // Terminamos MPI
	MPI_Finalize();
	return 0;
}
