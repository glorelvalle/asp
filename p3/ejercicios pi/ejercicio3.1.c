/* --------------------------------------------------------------------------
* Práctica 3 de Arquitectura de Sistemas Paralelos
*
* Cálculo de pi por integración numérica. MPI + OpenMP (híbrido).
* (Ejercicio 1.1)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main (int argc, char **argv) {
  MPI_Status status;
  int rank, numprocs;
  double h;
  double x;
  double sum;
  long i;
  double pi, mypi;
  long n;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  n = 1000000000;
  h = 1.0 / (double)n;
  sum = 0.0;

  #pragma omp parallel for default(shared) \
          private(i,x) reduction(+: sum)
    for(i = rank + 1; i <= n; i += numprocs){
      x = h * ((double)i - 0.5);
      sum += 4.0 / (1.0 + x*x);
    }
  mypi = h * sum;
  MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Finalize();
  printf("\nPi por integracion numerica (Version MPI + OpenMP): %f\n", pi);
}
