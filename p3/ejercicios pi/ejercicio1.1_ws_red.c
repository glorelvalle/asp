/* --------------------------------------------------------------------------
* Práctica 1 de Arquitectura de Sistemas Paralelos
*
* Cálculo de pi por integracion numerica. OpenMP Work Sharing + reduction.
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
  long i;

  h = 1.0 / (double)n;
  omp_set_num_threads(numThreads);

  double x;

  #pragma omp parallel for reduction(+: sum) \
          private(i,x)

    for(i = 1; i <= n; i++){
      x = h * ((double)i - 0.5);
      sum += 4.0 / (1.0 + x*x);
    }

  pi = h * sum;
  printf("\nPi por integracion numerica (Version Work Sharing + reduction): %f\n", pi);
  return 0;
}
