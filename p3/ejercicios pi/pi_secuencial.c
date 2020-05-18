/* --------------------------------------------------------------------------
* Práctica 1 de Arquitectura de Sistemas Paralelos
*
* Cálculo de pi por integracion numerica. Programa secuencial.
* (Ejercicio 1.1)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long n = 10485760;

int main (int argc, char **argv) {
  double h;
  double x;
  double sum;
  long i;
  double pi;

  h = 1.0 / (double)n;

  for(i = 1; i <= n; i++){
    x = h * ((double)i - 0.5);
    sum += 4.0 / (1.0 + x*x);
  }
  pi = h * sum;
  printf("\nPi por integracion numerica: %f\n", pi);
  return 0;
}
