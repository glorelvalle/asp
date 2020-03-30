/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Ejercicio 2.5: Cartesian Virtual Topology
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define PET 8
#define ARRIBA 0
#define ABAJO 1
#define IZQUD 2
#define DERCH 3
#define TAM 16
#define TAM_B 4
#define N_DIM 2

int main(int argc, char *argv[]){
  MPI_Request peticiones[PET];
  MPI_Status status[PET];
  MPI_Comm c_cartesiana;
  int i;
  int n_task;
  int rank, tag = 0;
  int salida, ranking_order;
  int buffer[TAM_B] = {MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL};
  int dimensiones[N_DIM] = {TAM_B, TAM_B};
  int periodos[N_DIM] = {0, 0};
  int vecinos[TAM_B], coordenadas[N_DIM];

  /* MPI*/
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &n_task);

  if (n_task == TAM){
    MPI_Cart_create(MPI_COMM_WORLD, N_DIM, dimensiones, periodos, ranking_order, &c_cartesiana);
    MPI_Comm_rank(c_cartesiana, &rank);
    MPI_Cart_coords(c_cartesiana, rank, N_DIM, coordenadas);
    MPI_Cart_shift(c_cartesiana, 0, 1, &vecinos[ARRIBA], &vecinos[ABAJO]);
    MPI_Cart_shift(c_cartesiana, 1, 1, &vecinos[IZQUD], &vecinos[DERCH]);
    salida = rank;

    for (i = 0; i < TAM_B; i++){
      MPI_Isend(&salida, 1, MPI_INT, vecinos[i], tag, MPI_COMM_WORLD, &peticiones[i]);
      MPI_Irecv(&buffer[i], 1, MPI_INT, vecinos[i], tag, MPI_COMM_WORLD, &peticiones[i+TAM_B]);
    }
    MPI_Waitall(PET, peticiones, status);
    printf("\nN: %d | Vecinos: (%d, %d, %d, %d) | Coordenadas: %d %d\n", rank, vecinos[ARRIBA], vecinos[ABAJO], vecinos[IZQUD], vecinos[DERCH], coordenadas[0], coordenadas[1]);
    printf("\tBuffer: (%d, %d, %d, %d)", buffer[ARRIBA], buffer[ABAJO], buffer[IZQUD], buffer[DERCH]);
  } else {
    printf("\nError en el n. de procesadores. Debes especificar %d procesadores\n", TAM);
  }
  MPI_Finalize();
  return 0;
}
