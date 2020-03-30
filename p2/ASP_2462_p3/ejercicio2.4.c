/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Ejercicio 2.4: Struct Derived Data Type
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define ELEMS 10
#define TAM_B 2

typedef struct{
  float x, y, z, velocity;
  int n, type;
} Particle;

int main(int argc, char *argv[]){
  MPI_Datatype oldtypes[TAM_B] = {MPI_FLOAT, MPI_INT}; /*para representar una partícula y distribuir vector de ellas a todos los procs*/
  MPI_Datatype particletype;
  MPI_Status status;
  MPI_Aint extent, offsets[TAM_B];
  Particle particles[ELEMS], rec[ELEMS];
  int i;
  int n_task;
  int rank, tag = 0;
  int blockcounts[TAM_B] = {4, 2};

  /* MPI*/
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_task);

  MPI_Type_extent(MPI_FLOAT, &extent);
  offsets[0] = 0;
  offsets[1] = 4*extent;
  MPI_Type_struct(TAM_B, blockcounts, offsets, oldtypes, &particletype);
  MPI_Type_commit(&particletype);

  if (rank == 0){
    for(i = 0; i < ELEMS; i++){
        particles[i].x = 1.0;
        particles[i].y = 2.0;
        particles[i].z = 3.0;
        particles[i].velocity = 0.5;
        particles[i].n = i;
        particles[i].type = i + 10;
    }
    for(i = 0; i < n_task; i++){
      MPI_Send(particles, ELEMS, particletype, i, tag, MPI_COMM_WORLD);
    }
  }
  MPI_Recv(rec, ELEMS, particletype, 0, tag, MPI_COMM_WORLD, &status);
  printf("\n-Elemento-\n");
  for(i = 0; i < ELEMS; i++){
    printf("N: %d, rec[%d] -> %.1f %.1f %.1f %.1f %d %d\n", rank, i, rec[i].x, rec[i].y, rec[i].z, rec[i].velocity, rec[i].n, rec[i].type);
  }
  printf("\n-----\n");

  MPI_Type_free(&particletype);
  MPI_Finalize();
  return 0;
}
