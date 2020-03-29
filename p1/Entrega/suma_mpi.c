/* --------------------------------------------------------------------------
* Práctica 2 de Arquitectura de Sistemas Paralelos
*
* Suma de los k primeros numeros (Ejercicio 1.1)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

#define DEBUG 1
#define N 1000000000 /*1MG*/
#define TAM 4

int main (int argc, char **argv) {
  unsigned long long indice;
  unsigned long long suma;
  char *datos;
  struct timeval inicio, fin;
  unsigned long long milisegundos;

  // MPI
  MPI_Status status; /*estructura para guardar información sobre operaciones de recepción de mensajes*/
  int nump, id; /*numero de procesos y id de proceso*/
  int tam; /*tamaño del reparto*/

  // Inicializar MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nump); /*determina el número de procesos que están actualmente asociados al comunicador*/
  MPI_Comm_rank(MPI_COMM_WORLD, &id); /*determina el rango (identificador) del proceso que lo llama dentro del comunicador seleccionado*/

  // Maestro
  if(id == 0) {
    // Solicitar memoria para el array
    datos = (char *)malloc(sizeof(char)*N);
    if(datos == NULL) {
      printf("Maestro: Error en malloc\n");
      MPI_Finalize();
      return -1;
    }

    // Inicializar array
    for(indice = 0; indice < N; indice++)
      datos[indice] = 2; /*inicializa array todos a 2, por ejemplo*/

    // Tomamos tiempo de inicio
    gettimeofday(&inicio, NULL);

    // Realizamos reparto
    /*tam = ceil(N/nump);*/
    tam = ceil(N/(nump*TAM));
    int esclavo;
    for(indice = 0, esclavo = 1; indice < N, esclavo < nump; indice += tam*TAM, esclavo += 1)
      MPI_Send(&(datos[indice]), tam, MPI_CHAR, esclavo, 1, MPI_COMM_WORLD); /*realiza el envío de un mensaje de un proceso fuente a otro destino a traves del comunicador global*/

    // Realizamos la suma del array restante
    suma = 0;
    if(DEBUG) printf("Maestro: Empieza suma en posicion %llu\n", indice);
    for(; indice < N; indice++)
      suma += datos[indice]; /*va haciendo el acumulativo*/
    if(DEBUG) printf("Maestro: Suma parcial %llu\n", suma);

    unsigned long long sumap;

    for(esclavo = 1; esclavo < nump; esclavo += 1) {
      MPI_Recv(&sumap, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status); /*rutina de recibimiento de un mensaje desde un proceso (bloqueante)*/
      if(DEBUG) printf("Maestro: Recibo de proceso %d la suma %llu\n", status.MPI_SOURCE, sumap); /*operacion reduce a mano, juntando ops realizadas en paralelo*/
      suma += sumap;

    }

    // Tomamos tiempo de fin
    gettimeofday(&fin, NULL);

    // Imprimimos resultado
    printf("Maestro: La suma de %d numeros es %llu\n", N, suma);

    // Imprimimos tiempo
    milisegundos = (fin.tv_sec - inicio.tv_sec) * 1000;
    milisegundos += (fin.tv_usec - inicio.tv_usec) / 1000;
    printf("Maestro: Tiempo de suma: %f segundos\n", milisegundos / 1000.0);

    // Liberamos memoria
    free(datos);
  }
  // Esclavos
  else {
    // Obtenemos tamano maximo
    /*tam = ceil(N/nump);*/
    tam = ceil(N/nump*TAM);
    // Solicitar memoria para el array
    datos = (char *)malloc(sizeof(char)*tam);
    if(datos == NULL) {
      printf("Proceso %d: Error en malloc\n", id);
      MPI_Finalize();
      return -1;
    }

    // Recibimos numeros
    MPI_Recv(datos, tam, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &tam); /*obtiene el número de elementos de alto nivel recibidos en un mensaje*/
    printf("Proceso %d: Recibo %d numeros\n", id, tam);

    // Realizamos la suma del array
    suma = 0;
    for(indice = 0; indice < tam; indice++)
      suma += datos[indice];

    printf("Proceso %d: Suma de %d numeros es %llu\n", id, tam, suma);

    // Enviamos resultado de la suma
    MPI_Send(&suma, 1, MPI_LONG_LONG, 0, 2, MPI_COMM_WORLD);

    // Liberamos memoria
    free(datos);
  }

  // Terminamos MPI
  MPI_Finalize();

  return 0;
}
