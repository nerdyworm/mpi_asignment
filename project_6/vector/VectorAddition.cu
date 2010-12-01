#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>

#define N       10 
#define THREADS 1 
#define BLOCKS  1

// size of array
__global__ void add(int *a,int *b, int *c)
{
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  if(tid < N)
  {
    c[tid] = a[tid]+b[tid];
  }
}

int main(void)
{
  int T = 100; // threads per block
  int B = 2;   // blocks per grid

  // VS may not like defined constants in <<< >>>
  int a[N],b[N],c[N];
  int *dev_a, *dev_b, *dev_c;
  cudaMalloc((void**)&dev_a,N * sizeof(int));
  cudaMalloc((void**)&dev_b,N * sizeof(int));
  cudaMalloc((void**)&dev_c,N * sizeof(int));

  for(int i=0;i<N;i++)
  {
    a[i] = i;
    b[i] = i*1;
  }

  cudaMemcpy(dev_a, a , N*sizeof(int),cudaMemcpyHostToDevice);
  cudaMemcpy(dev_b, b , N*sizeof(int),cudaMemcpyHostToDevice);
  cudaMemcpy(dev_c, c , N*sizeof(int),cudaMemcpyHostToDevice);


  cudaEvent_t start, stop;
  float elapsedTime;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start, 0);

  add<<<B,T>>>(dev_a,dev_b,dev_c);

  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventRecord(stop, 0);
  cudaEventElapsedTime(&elapsedTime, start, stop);
  cudaEventDestroy(start);
  cudaEventDestroy(stop);

  cudaMemcpy(c, dev_c, N*sizeof(int), cudaMemcpyDeviceToHost);

  for(int i=0;i<N;i++)
  {
    printf("%d+%d=%d\n",a[i],b[i],c[i]);
  }
  
 // printf("Execution Time: %1.10f\n", elapsedTime);


  cudaFree(dev_a);
  cudaFree(dev_b);
  cudaFree(dev_c);
  return 0;
}

