/* --------------------------------------------------------------------------
* Práctica 4 de Arquitectura de Sistemas Paralelos
*
* Convolucion 1D, serie
* (Ejercicio 1.2)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define N 10
#define M 5

float *conv_cpu(float *input, float *output, float *filter){
  float temp;
  int i, j;

  for(j = 0; j < N; j++){
    temp = 0.0;
    for(i = 0; i < M; i++){
      temp += filter[i]*input[i+j];
    }
    output[j] = temp;
  }
  return output;
}
int main(){
  struct timeval tv;
	unsigned long long inicio;
  float *input;
  float *filter;
  float *output;
  int i, j;

  input = (float*)malloc(N*sizeof(float));
  filter = (float*)malloc(M*sizeof(float));
  output = (float*)malloc(N*sizeof(float));

  for(i = 0; i < N; ++i){
		input[i] = (float)i;
	}

  for(j = 0; j < M; ++j){
		filter[j] = (float)j;
	}

  gettimeofday(&tv, NULL);
  inicio=tv.tv_sec*1000000+tv.tv_usec;

  // Llamada al kernel
  output = conv_cpu(input, output, filter);

  // Fin de tiempo
  gettimeofday(&tv, NULL);
	printf("Tiempo empleado: %lf ms\n", (tv.tv_sec*1000000+tv.tv_usec - inicio)/1000.0);

  for(i = 0; i < N; ++i){
		printf("%f\n", output[i]);
	}

  free(input);
	free(filter);
	free(output);
  return 0;
}
