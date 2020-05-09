/* --------------------------------------------------------------------------
* Práctica 1 de Arquitectura de Sistemas Paralelos
*
* Aproximacion de Pi por Monte Carlo, version paralelo
* (Ejercicio 1.2)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

#define DEBUG 1
#define N 1000000000 /*1MG*/
#define ARG 2

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

  // MPI
  MPI_Status status; /*estructura para guardar información sobre operaciones de recepción de mensajes*/
  int rank, nump, n_puntos;
  int i, k;
  int z = 0;
  int radio;
  float distancia, pi;
  punto p;
  struct timeval tv;
	unsigned long long inicio;

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

  gettimeofday(&tv, NULL);
  inicio=tv.tv_sec*1000000+tv.tv_usec;

  // Esclavo
  if (rank != 0)  {
    srand(time(NULL)-rank*2);
    for (i = 0; i < n_puntos; i++){
      p.x = numero_aleatorio(radio);
      p.y = numero_aleatorio(radio);
      distancia = distancia_euclidea(p.x, p.y);
      //printf("X: %f || Y: %f || Distancia: %f || (k: %d)\n", p.x, p.y, distancia, k);
      if (distancia < radio){
        k++;
      }
    }
  }
  // Maestro
  if(rank == 0){
    k = 0;
  }
  printf("Version en paralelo\n");
  MPI_Reduce(&k, &z, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank == 0) {
    pi = (float) (z*4)/((nump-1)*n_puntos);
    printf("Resultado de pi por Monte Carlo: %.10f\n", pi);
  }
  gettimeofday(&tv, NULL);
	printf("Tiempo empleado: %lf ms\n", (tv.tv_sec*1000000+tv.tv_usec - inicio)/1000.0);
  // Terminamos MPI
  MPI_Finalize();

  return 0;
}
