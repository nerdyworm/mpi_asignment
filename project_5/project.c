#include <stdio.h> 
#include <string.h> 
#include <stddef.h> 
#include <stdlib.h> 
#include "mpi.h" 

#include "hashtable.h"

#define CHUNKSIZE   10
#define N           100 

void openmp_code()
{ 
	int nthreads, tid, i, chunk; 
	float a[N], b[N], c[N]; 
	
  for (i=0; i < N; i++) 
		a[i] = b[i] = i * 1.0;             // initialize arrays 
	chunk = CHUNKSIZE; 
	
  #pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,tid) 
	{ 
		tid = omp_get_thread_num(); 
		
    if (tid == 0)
    { 
			nthreads = omp_get_num_threads(); 
			printf("Number of threads = %d\n", nthreads); 
		} 
		
    //printf("Thread %d starting...\n",tid); 
		#pragma omp for schedule(dynamic,chunk) 
		for (i=0; i<N; i++)
    { 
			c[i] = a[i] + b[i]; 
			//printf("Thread %d: c[%d]= %f\n",tid,i,c[i]); 
		} 
	}  /* end of parallel section */ 
} 

int compare_ints(const void *a, const void *b)
{
  double temp = *(const int*)a - *(const int*)b;
  if (temp > 0)
    return 1;
  else if (temp < 0)
    return -1;
  else
    return 0;
}

void parallel_sort(int sortme[], int size, int *output)
{
  struct hashtable *h;

  int i;
  //qsort(sortme, size, sizeof(int), compare_ints);

  for(i=0; i < size; i++)
  {
    output[i] = sortme[i];
  }
}

void merge(int a[], int b[], int c[], int k, int j)
{
   int indexA = 0, indexB = 0, indexC = 0;
   
   while(indexA < k && indexB < j)
   {
      if(a[indexA] < b[indexB]) {
        c[indexC] = a[indexA];
         indexA++;
      } else {
         c[indexC] = b[indexB];
         indexB++;
      }

      indexC++;
   }

   while(indexA < k)
   {
      c[indexC]=a[indexA];
      indexA++;
      indexC++;
   }

   while(indexB < j)
   {
      c[indexC]=b[indexB];
      indexB++;
      indexC++;
   }
}

main(int argc, char **argv ) 
{ 
  int data[CHUNKSIZE];
	int i, rank, size, type=99;

	MPI_Status status; 
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &size); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	
  if(rank == 0) 
  { 
    int generated_data[N]; 
    
    for(i=0; i < N; i++)
      generated_data[i] = N - i;

    int k = 0, current = 0;
    for(i = 1; i < size; i++)
    {
		  int part[CHUNKSIZE];
    
      for(k = 0; k < CHUNKSIZE; k++)
        data[k] = generated_data[k + current];
      
      current += CHUNKSIZE;

      MPI_Send(data, CHUNKSIZE, MPI_INT, i, type, MPI_COMM_WORLD); 
    }

    //generate data for rank 0 machine
    for(k = 0; k < CHUNKSIZE; k++)
      data[k] = generated_data[k + current];
  }
	else 
		MPI_Recv(data, CHUNKSIZE, MPI_INT, 0, type, MPI_COMM_WORLD, &status); 
	
 
  int sorted[CHUNKSIZE];
  
  parallel_sort(data, CHUNKSIZE, sorted);
	
  if(rank > 0) 
    MPI_Send(sorted, CHUNKSIZE, MPI_INT, 0, type, MPI_COMM_WORLD);

  if(rank == 0)
  {
    int solution[N];
    int chunks_recevied = 1;
    int sorted_chunk[CHUNKSIZE];
    size_t css = CHUNKSIZE;//current solution size
    
    merge(NULL, sorted, solution, 0, CHUNKSIZE);//merge rank0's list

    while(1)
    {
      MPI_Recv(sorted_chunk, CHUNKSIZE, MPI_INT, MPI_ANY_SOURCE, type, MPI_COMM_WORLD, &status);
      
      int *temp = (int *)malloc(css * sizeof(int));

      for(i = 0; i < css; i++) 
        temp[i] = solution[i];

      merge(temp, sorted_chunk, solution, css, CHUNKSIZE);
   
      chunks_recevied++;
      css += CHUNKSIZE;
      free(temp);

      if(chunks_recevied >= N / CHUNKSIZE) break; 
    }


    for(i = 0; i < css; i++)
      printf("solution[%d]: %d\n", i, solution[i]);
  }

  MPI_Finalize(); 
}

