#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <curand.h>
#include <math.h>
   
#define N 1000000

__global__ void counts(float *x, float *y, int *results)
{
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  
  if(tid < N)
  {
    float result = x[tid] * x[tid] + y[tid] * y[tid];

    if(result <= 1)
      results[tid] = 1;
    else
      results[tid] = 0;
  }
}

int main(void)
{
  int T = 500; // threads per block
  int B = 2000;   // blocks per grid
  
  int *dev_results, *host_results;
  float *dev_x;
  float *dev_y;

  //host memory
  host_results = (int *) calloc(N, sizeof(float));

  //device memory
  cudaMalloc((void**)&dev_x, N * sizeof(float));
  cudaMalloc((void**)&dev_y, N * sizeof(float));
  cudaMalloc((void**)&dev_results, N * sizeof(int));


  //random generator
  curandGenerator_t gen;

  curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
  curandSetPseudoRandomGeneratorSeed(gen, 1234ULL);

  //generate random numbers on device
  curandGenerateUniform(gen, dev_x, N);
  curandGenerateUniform(gen, dev_y, N);

  //timmers
  cudaEvent_t start, stop;
  float elapsedTime;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start, 0);

  //get results
  counts<<<B,T>>>(dev_x, dev_y, dev_results);

  //stop timers
  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventRecord(stop, 0);
  cudaEventElapsedTime(&elapsedTime, start, stop);
  cudaEventDestroy(start);
  cudaEventDestroy(stop);

  //copy results to host's memory
  cudaMemcpy(host_results, dev_results, N*sizeof(int), cudaMemcpyDeviceToHost);

  //sum the results
  int counts = 0;
  for(int i=0;i<N;i++)
  {
    if(host_results[i])
      counts++;
    
    printf("%d ", host_results[i]);
  }
  
  float pi = 4.0 * counts / N;

  printf("Pi: %1.10f\n", pi);

  printf("Execution Time: %1.10f\n", elapsedTime);

  //cleanup
  curandDestroyGenerator(gen);
  cudaFree(dev_x);
  cudaFree(dev_y);
  cudaFree(dev_results);
  free(host_results);
  return 0;
}

