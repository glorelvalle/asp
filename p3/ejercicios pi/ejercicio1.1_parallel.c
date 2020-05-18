/* --------------------------------------------------------------------------
* Práctica 1 de Arquitectura de Sistemas Paralelos
*
* Cálculo de pi por integracion numerica. OpenMP Parallel.
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
  double sum[100];
  double pi;
  int i;

  h = 1.0 / (double)n;
  omp_set_num_threads(numThreads);

  #pragma omp parallel
  {
    double x;
    double priv_sum;
    int i, tid;
    tid = omp_get_thread_num();

    for(i = tid + 1; i <= n; i += numThreads){
      x = h * ((double)i - 0.5);
      priv_sum += 4.0 / (1.0 + x*x);
    }
    sum[tid] = priv_sum;
  }

  pi = 0.0;
  for (i = 0; i < numThreads; i++){
    pi += sum[i];
  }
  pi = h * pi;
  printf("\nPi por integracion numerica (Version parallel): %f\n", pi);
  return 0;
}
