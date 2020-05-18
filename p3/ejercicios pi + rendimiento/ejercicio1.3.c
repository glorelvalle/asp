/* --------------------------------------------------------------------------
* Práctica 3 de Arquitectura de Sistemas Paralelos
*
* Aproximacion de Pi por Monte Carlo, version OpenMP
* (Ejercicio 1.3)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <string.h>
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
  int rank, nump, n_puntos;
  int i, k, z;
  int radio;
  float distancia, pi;
  punto p;
  struct timeval tv;
	unsigned long long inicio;
  int numThreads = 8;

  omp_set_num_threads(numThreads);

  radio = 2;
  n_puntos = 100000;

	gettimeofday(&tv, NULL);
  inicio=tv.tv_sec*1000000+tv.tv_usec;
  /* init seed*/

  #pragma omp parallel
	{
    srand(time(NULL));

    #pragma omp for private(p) reduction(+:k) schedule(static)
      for (i = 0; i < n_puntos; i++){
        p.x = numero_aleatorio(radio);
        p.y = numero_aleatorio(radio);
        distancia = distancia_euclidea(p.x, p.y);
        //printf("X: %f || Y: %f || Distancia: %f || (k: %d)\n", p.x, p.y, distancia, k);

        if (distancia < radio){
          k++;
        }
      }
    pi = (float) (k*4)/n_puntos;
  }
	gettimeofday(&tv, NULL);
	printf("Tiempo empleado: %lf ms\n", (tv.tv_sec*1000000+tv.tv_usec - inicio)/1000.0);
  printf("Resultado de pi por Monte Carlo: %.10f\n", pi);

  return 0;
}
