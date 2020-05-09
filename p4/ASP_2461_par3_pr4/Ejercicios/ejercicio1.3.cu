%%cu
/* --------------------------------------------------------------------------
* Práctica 4 de Arquitectura de Sistemas Paralelos
*
* Aproximacion de Pi por Monte Carlo, CUDA
* (Ejercicio 1.3)
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

using namespace std;

// Kernel para estimar pi
__global__
void puntos_dentro_circulo(float* d_X, float* d_Y, int* d_bloques, int n_blques, int muestras){
  __shared__ int bloques[200];

  int indice = blockIdx.x * blockDim.x + threadIdx.x;
  int acc = blockDim.x * n_bloques;
  int cuenta = 0;
	int total_bloque = 0;
	int i, j;

  for (i = indice; i < muestras; i+= acc){
    float x = d_X[i];
    float y = d_Y[i];

    if (x*x + y*y <= 1.0f) {
      cuenta++;
    }
  }

  bloques[threadIdx.x] = cuenta;

  __syncthreads();

  if (threadIdx.x == 0){
    for (j = 0; j < blockDim.x; j++){
      total_bloque += bloques[j];
    }
    d_bloques[blockIdx.x] = total_bloque;
  }
}

int muestras = 1e8;

int main(void) {
	int i;
	int muestras_circulo = 0;
  int hilos_bloque = 200;
  int n_bloques = muestras / (1000 * hilos_bloque);
  int *d_bloques;
  int *h_bloques = new int[n_bloques];

	float pi = 0;
  float *d_X;
  float *d_Y;

  vector<float> h_X(muestras);
  vector<float> h_Y(muestras);

  size_t tam = muestras * sizeof(float);
  size_t cuenta_bloques = n_bloques * sizeof(int);

  struct timeval tv;
	unsigned long long inicio;

  srand(time(NULL));
  for (i = 0; i < h_X.size(); ++i){
    h_X[i] = float(rand()) / RAND_MAX;
    h_Y[i] = float(rand()) / RAND_MAX;
  }

  // Envio de los valores a la GPU
  cudaMalloc(&d_X, tam);
  cudaMalloc(&d_Y, tam);
  cudaMemcpy(d_X, &h_X.front(), tam, cudaMemcpyHostToDevice);
  cudaMemcpy(d_Y, &h_Y.front(), tam, cudaMemcpyHostToDevice);

  // Init kernel
  cudaMalloc(&d_bloques, cuenta_bloques);

  // Comienzo cuenta tiempo
  gettimeofday(&tv, NULL);
  inicio=tv.tv_sec*1000000+tv.tv_usec;

  // Llamada al kernel
  puntos_dentro_circulo<<<n_bloques, hilos_bloque>>>(d_X, d_Y, d_bloques, n_bloques, muestras);
  if (cudaSuccess != cudaGetLastError())
    cout << "Error (puntos_dentro_circulo)\n";

  // Fin de tiempo
  gettimeofday(&tv, NULL);
	printf("Tiempo empleado: %lf ms\n", (tv.tv_sec*1000000+tv.tv_usec - inicio)/1000.0);

  cudaMemcpy(h_bloques, d_bloques, cuenta_bloques, cudaMemcpyDeviceToHost);

  for (i = 0 ; i < n_bloques; i++) {
    muestras_circulo = muestras_circulo + h_bloques[i];
  }

	// Liberar memoria
  cudaFree(d_X);
  cudaFree(d_Y);
  cudaFree(d_bloques);

  // Calculo de pi final
  pi = 4.0 * float(muestras_circulo) / muestras;

  cout << "Valor calculado de pi: " << pi << endl;
}
