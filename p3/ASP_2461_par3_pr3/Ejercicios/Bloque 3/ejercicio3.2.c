/* --------------------------------------------------------------------------
* Práctica 3 de Arquitectura de Sistemas Paralelos
*
* Aproximacion de Pi por Monte Carlo, version MPI + OpenMP
* (Ejercicio 3.2)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "mpi.h"
#include <omp.h>
#define F "monte_carlo_performance.txt"

typedef struct{
  float x;
  float y;
}punto;

float numero_aleatorio(float n){
  if (n < 0 || n > RAND_MAX){
    return -1;
  }
  return (float)(rand()/(RAND_MAX/n));
}
float distancia_euclidea(float x, float y){
  if (x < 0 || y < 0){
    return -1;
  }
  return sqrt(pow(x, 2) + pow(y, 2));
}

int main (int argc, char **argv) {

  MPI_Status status;
  int rank, nump, n_puntos;
  int i, k;
  int z = 0;
  int radio;
  float distancia, pi;
  punto p;
  int numThreads = 8;
  FILE *fp;
  struct timeval inicio, fin;
  unsigned long long milisegundos;

  omp_set_num_threads(numThreads);

  // Inicializar MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nump); /*determina el número de procesos que están actualmente asociados al comunicador*/
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); /*determina el rango (identificador) del proceso que lo llama dentro del comunicador seleccionado*/

  // Control errores init
  radio = atoi(argv[1]);
  if (radio < 0){
    if (rank == 0){
      printf("Error args: radio");
      MPI_Finalize();
      return -1;
    }
  }
  n_puntos = atoi(argv[2]);
  if (n_puntos < 0){
    if (rank == 0){
      printf("Error args: n_puntos");
      MPI_Finalize();
      return -1;
    }
  }

  gettimeofday(&inicio, NULL);

  // Esclavo
  if (rank != 0){
  // OpenMP
  //#pragma omp parallel
	//{
    srand(time(NULL)-rank*2);
    // firstprivate(p, distancia, i) reduction(+: k) num_threads(numThreads)
    //#pragma omp parallel
    #pragma omp parallel firstprivate(p, distancia, i) reduction(+:k) num_threads(numThreads)
      for (i = 0; i < n_puntos; i++){
        p.x = numero_aleatorio(radio);
        p.y = numero_aleatorio(radio);
        distancia = distancia_euclidea(p.x, p.y);
        printf("X: %f || Y: %f || Distancia: %f || (k: %d)\n", p.x, p.y, distancia, k);
        if (distancia < radio){
          //#pragma omp critical
            k++;
        }

    }
  }
  //}

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&k, &z, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  // Maestro
  if(rank == 0){
    k = 0;
  }
  printf("Version en paralelo\n");


  if(rank == 0) {
    pi = (float) (z*4)/((nump-1)*n_puntos);
    printf("Resultado de pi por Monte Carlo: %.10f\n", pi);
  }
  gettimeofday(&fin, NULL);
  milisegundos = (fin.tv_sec - inicio.tv_sec) * 1000;
  milisegundos += (fin.tv_usec - inicio.tv_usec) / 1000;
  printf("\n%f segundos\n", milisegundos / 1000.0);
  fp = fopen(F,"a");
  fprintf(fp,"MonteCarlo(4),%lf\n",(double)milisegundos / 1000.0);
  fclose(fp);

  // Terminamos MPI
  MPI_Finalize();

  return 0;
}
