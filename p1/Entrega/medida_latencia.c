/* --------------------------------------------------------------------------
 * Práctica 1 de Arquitectura de Sistemas Paralelos
 * Code:   medida_latencia.c
 * Lab:    MPI Point-to-Point Communication
 * Tiempo de transferencia: t = latencia + ax
 * x = # de bytes transferidos por segundo
 * a = tiempo que se tarda en enviar un byte una vez iniciada la comunicacion
 *
 * Michael Alexander Fajardo y Gloria del Valle
 * -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "mpi.h"

#define REPS 1000

int main(int argc, char **argv){

		double t_ini, t_end;						/* tiempos de inicio y de fin           */
		double latencia = 0;						/* medida de la latencia                */
		double t_repeticion;						/* tiempo por repeticion                */
  	char *mensaje=NULL;             /* mensaje que vamos a mandar           */
  	char count;                     /* un byte                              */
  	int rank,                       /* rank of task in communicator         */
      	len_mss,                    /* message length specified by user     */
      	i, j;    										/* contadores                           */
  	int tag = 1;										/* message tags                         */
  	MPI_Status status;	            /* status of communication              */
		int n;													/* tam mensaje                          */

		if(argc != 2){
			printf("Error en el tamaño de mensaje\n");
			exit(0);
		}
		n = atoi(argv[1]);

    /* inicio MPI*/
  	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  	/* maestro*/
  	if (rank == 0){
  		for (i = 0; i < REPS; i++){
  			/* medida del tiempo empleado en enviar y recibir 1 byte*/
  			t_ini = MPI_Wtime();
  			MPI_Send(&count, 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
  			MPI_Recv(&count, 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
  			t_end = MPI_Wtime();
  			/* calculo de la latencia*/
  			latencia += (double)((t_end - t_ini))/2;
  			printf("Latencia en la iteracion %d: %f\n", i, latencia);
  		}
  		/* latencia media*/
  		latencia = (double)(latencia/REPS);
  		printf("El valor de la latencia media es %.7f\n", latencia);

  		/* paso de potencias de 2*/
  		for (i = 0; i <= n; i++){
  			len_mss = pow(2, i);
  			mensaje = (char*)malloc(sizeof(char)*len_mss);
  			if (mensaje == NULL){
  				printf("Maestro: Error en malloc\n");
  				MPI_Finalize();
      		return -1;
  			}
  			for(j = 0; j < 10; j++){
  				/* medida del tiempo empleado en enviar y recibir el mensaje*/
	  			t_ini = MPI_Wtime();
	  			MPI_Send(mensaje, len_mss, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
	  			MPI_Recv(mensaje, len_mss, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
	  			t_end = MPI_Wtime();
	  			/* calculo de la latencia*/
	  			t_repeticion += (double)((t_end - t_ini))/2;
  			}
  			/* latencia media*/
	  		t_repeticion = (double)(t_repeticion/10);
	  		printf("T. repeticion: %.5f\n", t_repeticion);
	  		free(mensaje);
  		}
  	} /* esclavo*/
  	else if (rank == 1){
  		for (i = 0; i < REPS; i++){
  			MPI_Recv(&count, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
  			MPI_Send(&count, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
  		}
  		for (i = 0; i <= n; i++){
				len_mss = pow(2, i);
				mensaje = (char*)malloc(sizeof(char)*len_mss);
				if (mensaje == NULL){
					printf("Maestro: Error en malloc\n");
					MPI_Finalize();
	  			return -1;
				}
				for (j = 0; j < 10; j++){
					MPI_Recv(mensaje, len_mss, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
					MPI_Send(mensaje, len_mss, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
				}
				free(mensaje);
			}

  	} else {
			printf("\n");
		}
  	MPI_Finalize();
  	return 0;
}
