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
#include <string.h>
#include <sys/time.h>
#define F "pi_performance.txt"

long n = 10485760;

int main (int argc, char **argv) {
  double h;
  double x;
  double sum;
  long i;
  double pi;
  FILE *fp;
  struct timeval inicio, fin;
  unsigned long long milisegundos;

  gettimeofday(&inicio, NULL);

  h = 1.0 / (double)n;

  for(i = 1; i <= n; i++){
    x = h * ((double)i - 0.5);
    sum += 4.0 / (1.0 + x*x);
  }
  pi = h * sum;

  gettimeofday(&fin, NULL);
  milisegundos = (fin.tv_sec - inicio.tv_sec) * 1000;
  milisegundos += (fin.tv_usec - inicio.tv_usec) / 1000;
  printf("\n%f segundos\n", milisegundos / 1000.0);
  fp = fopen(F,"a");
  fprintf(fp,"Secuencial,%lf\n",(double)milisegundos / 1000.0);
  fclose(fp);

  printf("\nPi por integracion numerica: %f\n", pi);
  return 0;
}
