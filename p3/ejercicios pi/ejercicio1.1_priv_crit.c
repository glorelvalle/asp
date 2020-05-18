/* --------------------------------------------------------------------------
* Práctica 1 de Arquitectura de Sistemas Paralelos
*
* Cálculo de pi por integracion numerica. OpenMP private + critical.
* (Ejercicio 1.1)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

long n = 10485760;
int numThreads = 8;

int main (int argc, char **argv) {
  double h;
  double sum;
  double pi;
  double x;
  int i;

  h = 1.0 / (double)n;
  omp_set_num_threads(numThreads);

  #pragma omp parallel private(x, sum)
  {
    int id;
    id = omp_get_thread_num();
    sum = 0.0;

    for(i = id + 1; i <= n; i += numThreads){
      x = h * ((double)i - 0.5);
      sum += 4.0 / (1.0 + x*x);
    }
    #pragma omp critical
      pi += sum;
  }
  pi = h * pi;
  printf("\nPi por integracion numerica (Version private + critical): %f\n", pi);
  return 0;
}
