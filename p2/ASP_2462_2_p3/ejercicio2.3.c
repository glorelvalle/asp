/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Ejercicio 2.3: Indexed Derived Data Type
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
/*1 elem indextype*/
#define ELEMS 6
#define TAM_B 2
#define ARRAY 16

int main(int argc, char *argv[]){
  MPI_Datatype indextype; /*para extraer datos en bloques de distintos tamaños*/
  MPI_Status status;
  int i;
  int n_task;
  int rank, tag = 0;
  float a[ARRAY] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
  float rec[ELEMS];
  int blocklengths[TAM_B] = {4, 2};
  int displacements[TAM_B] = {5, 12};

  /* MPI*/
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_task);

  MPI_Type_indexed(TAM_B, blocklengths, displacements, MPI_FLOAT, &indextype);
  MPI_Type_commit(&indextype);

  if (rank == 0){
    printf("\nArray: ");
    for(i = 0; i < ARRAY; i++){
        printf("%.1f ", a[i]);
    }
    printf("\n");
    for(i = 0; i < n_task; i++){
      MPI_Send(a, 1, indextype, i, tag, MPI_COMM_WORLD);
    }
  }
  MPI_Recv(rec, ELEMS, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
  printf("\n-Elemento-\n");
  for(i = 0; i < ELEMS; i++){
    printf("N: %d, rec[%d] -> %.1f\n", rank, i, rec[i]);
  }
  printf("\n-----\n");

  MPI_Type_free(&indextype);
  MPI_Finalize();
  return 0;
}
