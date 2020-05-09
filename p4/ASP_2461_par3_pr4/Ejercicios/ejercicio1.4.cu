%%writefile main.cu
/* --------------------------------------------------------------------------
* Práctica 4 de Arquitectura de Sistemas Paralelos
*
* Multiplicacion de matrices NxN, CUDA
* (Ejercicio 1.4)
* Michael Alexander Fajardo y Gloria del Valle
* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cuda.h>
#define tamMatriz 400
__global__ void multM (int *matA, int *matB, int *matC, int tam);
__global__ void multM (int *matA, int *matB, int *matC, int tam){
  int k, sum = 0;
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;
  if(row < tam && col < tam) {
    for (k = 0; k < tam; k++){
    sum += matA[row * tam + k] * matB[k * tam + col];
    }
    matC[row * tam + col] = sum;
  }
}
int main(void) {
 int size,i,j;
 int **matrizA,**matrizB,**matrizC;
 int dist;
 int *dev_a, *dev_b, *dev_c;
 int *chunkA, *chunkB, *chunkC;
 chunkA = (int*) malloc(sizeof(int)*tamMatriz*tamMatriz);
 chunkB = (int*) malloc(sizeof(int)*tamMatriz*tamMatriz);
 chunkC = (int*) malloc(sizeof(int)*tamMatriz*tamMatriz);
 matrizA = (int**) malloc(sizeof(int*)*tamMatriz);
 matrizB = (int**) malloc(sizeof(int*)*tamMatriz);
 matrizC = (int**) malloc(sizeof(int*)*tamMatriz);

 for(i = 0; i < tamMatriz; i++){
  matrizA[i] = chunkA + i*tamMatriz;
  matrizB[i] = chunkB + i*tamMatriz;
  matrizC[i] = chunkC + i*tamMatriz;
 }

 size = tamMatriz * tamMatriz * sizeof(int);

 for(i = 0; i < tamMatriz; i++){
  for(j = 0; j < tamMatriz; j++){
    matrizA[i][j] = 1;
  if(i == j)
   matrizB[i][j] = 1;
  else
   matrizB[i][j] = 0;
  }
 }

 cudaMalloc((void **) &dev_a, size);
 cudaMalloc((void **) &dev_b, size);
 cudaMalloc((void **) &dev_c, size);
 cudaMemcpy(dev_a, chunkA, size, cudaMemcpyHostToDevice);
 cudaMemcpy(dev_b, chunkB, size, cudaMemcpyHostToDevice);

  if(tamMatriz <= 1024){
      dist = ceil(sqrt(tamMatriz)) + 1;
  }
  else{
      dist  = 32;
  }

  dim3 blocksPerGrid(dist, dist);
  dim3 threadsPerBlock(dist,dist);


 multM<<<blocksPerGrid,threadsPerBlock>>>(dev_a, dev_b, dev_c, tamMatriz);
 cudaMemcpy(chunkC, dev_c, size, cudaMemcpyDeviceToHost);
  cudaError_t error = cudaGetLastError();
  if(error!=cudaSuccess)
  {
    fprintf(stderr,"ERROR: %s\n", cudaGetErrorString(error) );
    exit(-1);
  }
 for(i = 0; i < tamMatriz; i++){
  for(j = 0; j < tamMatriz; j++){
    printf("%d ", matrizA[i][j]);
  }
  printf("\n");
 }
 printf("\n\n");
 for(i = 0; i < tamMatriz; i++){
  for(j = 0; j < tamMatriz; j++){
    printf("%d ", matrizB[i][j]);
  }
  printf("\n");
 }
 printf("\n\n");
 for(i = 0; i < tamMatriz; i++){
  for(j = 0; j < tamMatriz; j++){
    printf("%d ", matrizC[i][j]);
  }
  printf("\n");

 }
 printf("\n\n");


 free(chunkA);
 free(chunkB);
 free(chunkC);
 cudaFree(dev_a);
 cudaFree(dev_b);
 cudaFree(dev_c);
 return 1;
}
