#include <stdio.h> 
#include <string.h> 
#include <stddef.h> 
#include <stdlib.h> 
#include "mpi.h" 
#define CHUNKSIZE   10 
#define N       100 

void openmp_code(){ 
	int nthreads, tid, i, chunk; 
	float a[N], b[N], c[N]; 
	for (i=0; i < N; i++) 
		a[i] = b[i] = i * 1.0;             // initialize arrays 
	chunk = CHUNKSIZE; 
	#pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,tid) 
	{ 
		tid = omp_get_thread_num(); 
		if (tid == 0){ 
			nthreads = omp_get_num_threads(); 
			printf("Number of threads = %d\n", nthreads); 
		} 
		// printf("Thread %d starting...\n",tid); 
		#pragma omp for schedule(dynamic,chunk) 
		for (i=0; i<N; i++){ 
			c[i] = a[i] + b[i]; 
			// printf("Thread %d: c[%d]= %f\n",tid,i,c[i]); 
		} 
	}  /* end of parallel section */ 
} 

main(int argc, char **argv ) { 
	char message[20]; 
	int i,rank, size, type=99; 
	MPI_Status status; 
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
	if(rank == 0) { 
		strcpy(message, "Hello, world"); 
		for (i=1; i<size; i++) 
			MPI_Send(message, 13, MPI_CHAR, i, type, MPI_COMM_WORLD); 
	} 
	else 
		MPI_Recv(message, 20, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status); 
	if( 1){ //* if this machine has multiple cores 
		openmp_code(); //run the mpi code 
	} 
	printf( "Message from process =%d : %.13s\n", rank,message); 
	MPI_Finalize(); 
}
