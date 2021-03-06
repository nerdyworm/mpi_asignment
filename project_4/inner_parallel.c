#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define M 50
#define N 50
#define THREADS 8

int main(int argc, char *argv) 
{
  omp_set_num_threads(THREADS);
  
  int i, j, k;
  double sum;
  double **A, **B, **C;
  A = malloc(M*sizeof(double *));
  B = malloc(M*sizeof(double *));
  C = malloc(M*sizeof(double *));
  
  for (i = 0; i < M; i++) {
    A[i] = malloc(N*sizeof(double));
    B[i] = malloc(N*sizeof(double));
    C[i] = malloc(N*sizeof(double));
  }
  
  double start, end;
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      A[i][j] = j*1;
      B[i][j] = i*j+2;
      C[i][j] = j-i*2;
    }
  }
  
  start = omp_get_wtime();
  
  
  for (i = 0; i < M; i++) 
  {
    #pragma omp parallel shared(A, B, C, i) private(j, k)
    {
      for (j = 0; j < N; j++) 
      {
        sum = 0;
        for (k=0; k < M; k++) {
          sum += A[i][k]*B[k][j];
        }
        
        C[i][j] = sum;
      }
    }
  }
  
  end = omp_get_wtime();
  
  printf("Time of computation: %f\n", end-start);
}
