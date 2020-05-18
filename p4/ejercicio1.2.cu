%%cu
/* --------------------------------------------------------------------------
* Práctica 4 de Arquitectura de Sistemas Paralelos
*
* Convolucion 1D, CUDA
* (Ejercicio 1.2)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <iostream>
#include <vector>
#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define TAM_N 10
#define BUFF 20
#define TAM_M 5

__constant__ float k[TAM_M];

__global__
void convolucion_1D(float *x, float *y){
  int i;
  int x_i;
	int indice = blockIdx.x * blockDim.x + threadIdx.x;
  int actual = blockIdx.x * blockDim.x;
  int siguiente = (blockIdx.x+1) * blockDim.x;
  int comienzo = indice - (TAM_M/2);
  int paso;

  float y_valores = 0;
	__shared__ float x_s[TAM_N];

	x_s[threadIdx.x]=x[indice];

	__syncthreads();

	for(i = 0; i < TAM_M; i++){
		x_i = comienzo + i;
		if(x_i >= 0 && x_i < BUFF){
			if(x_i < siguiente && x_i >= actual){
        paso = i - (TAM_M/2);
				y_valores += x_s[threadIdx.x+paso]*k[i];
			}
			else{
				y_valores += x[x_i]*k[i];
			}
		}
	}
	y[indice]=y_valores;
}

int main(){
  int i, j;

  // BUFF: X, salida: Y
	float *d_X = 0;
	float *d_Y = 0;

  // HOST variables
	float *h_X = (float*)malloc(BUFF*sizeof(float));
  float *h_K = (float*)malloc(TAM_M*sizeof(float));
	float *h_Y = (float*)malloc(BUFF*sizeof(float));

  struct timeval tv;
	unsigned long long inicio;

	cudaMalloc(&d_X,BUFF*sizeof(float));
	cudaMalloc(&d_Y,BUFF*sizeof(float));

	for(i=0;i<BUFF;++i){
		h_X[i]=(float)i;
	}

	cudaMemcpy(d_X,h_X,BUFF*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_Y,h_Y,BUFF*sizeof(float), cudaMemcpyHostToDevice);

	for(j=0;j<TAM_M;++j){
		h_K[j]=(float)j;
	}

	cudaMemcpyToSymbol(k,h_K,TAM_M*sizeof(float));

  // Comienzo cuenta tiempo
  gettimeofday(&tv, NULL);
  inicio=tv.tv_sec*1000000+tv.tv_usec;

	// Llamada al kernel
	convolucion_1D<<<(BUFF+TAM_N-1)/TAM_N,TAM_N >>>(d_X,d_Y);

  // Fin de tiempo
  gettimeofday(&tv, NULL);
	printf("Tiempo empleado: %lf ms\n", (tv.tv_sec*1000000+tv.tv_usec - inicio)/1000.0);

	// Recuperacion resultados
	cudaMemcpy(h_Y,d_Y,BUFF*sizeof(float), cudaMemcpyDeviceToHost);

	for(i=0; i<BUFF;++i){
		printf("%f\n", h_Y[i]);
	}

	cudaFree(d_X);
	cudaFree(d_Y);
	cudaFree(k);

	free(h_X);
	free(h_Y);
	free(h_K);

}
