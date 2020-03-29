/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Ejercicio 2.1: Contiguous Derived Data Type
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define TAM 4

int main(int argc, char *argv[]){
  MPI_Datatype rowtype; /*para la fila diferente a cada uno de los procs*/
  MPI_Status status;
  int i, j;
  int count = TAM, n_task;
  int rank, tag = 0;
  float a[TAM][TAM] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
  float rec[TAM];

  /* MPI*/
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_task);
  MPI_Type_contiguous(count, MPI_FLOAT, &rowtype);
  MPI_Type_commit(&rowtype);

  if (n_task == TAM){
    if (rank == 0){
      printf("\n\t-----------Matriz-----------\n");
      for(i = 0; i < count; i++){
        for(j = 0; j < count; j++){
          printf("\t%.1f ", a[i][j]);
        }
        printf("\n");
      }
      printf("\n\t----------------------------\n");
      for(i = 0; i < n_task; i++){
        MPI_Send(&a[i][0], 1, rowtype, i, tag, MPI_COMM_WORLD);
      }
    }
    MPI_Recv(rec, count, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
    printf("\n-Fila-\n");
    for(i = 0; i < count; i++){
      printf("N: %d, rec[%d] -> %.1f\n", rank, i, rec[i]);
    }
    printf("\n-----\n");
  } else {
    printf("\nError en el n. de procesadores. Debes especificar %d procesadores\n", count);
  }
  /* Liberamos datatype*/
  MPI_Type_free(&rowtype);
  MPI_Finalize();
  return 0;
}
