/* --------------------------------------------------------------------------
* Práctica 1 de Arquitectura de Sistemas Paralelos
*
* Aproximacion de Pi por Monte Carlo, version serie
* (Ejercicio 1.2)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#define DEBUG 1
#define N 1000000000 /*1MG*/
#define ARG 3

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
  int rank, nump, n_puntos;
  int i, k, z;
  int radio;
  float distancia, pi;
  punto p;

  // Control errores init
  if (argc != ARG){
    if (rank == 0){
      fprintf(stderr, "Error args entrada: %s", argv[0]);
      return -1;
    }
  }
  radio = atoi(argv[1]);
  if (radio < 0){
    if (rank == 0){
      fprintf(stderr, "Error args: radio");
      return -1;
    }
  }
  n_puntos = atoi(argv[2]);
  if (n_puntos < 0){
    if (rank == 0){
      fprintf(stderr, "Error args: n_puntos");
      return -1;
    }
  }
  /* init seed*/
  srand(time(NULL));
  for (i = 0; i < n_puntos; i++){
    p.x = numero_aleatorio(radio);
    p.y = numero_aleatorio(radio);
    distancia = distancia_euclidea(p.x, p.y);
    printf("X: %f || Y: %f || Distancia: %f || (k: %d)\n", p.x, p.y, distancia, k);

    if (distancia < radio){
      k++;
    }
  }
  pi = (float) (k*4)/n_puntos;
  printf("Resultado de pi por Monte Carlo: %.10f\n", pi);

  return 0;
}
