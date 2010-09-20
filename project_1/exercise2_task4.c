#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "mpi.h"
main(int argc, char **argv )
{
  char buffer[100];
  int i, rank, size, type=99;
  MPI_Status status;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start_time, end_time, exe_time;
  start_time = MPI_Wtime();

  if(rank == 0) {
    printf("Master: Hello slaves give me your messages\n");
    
    for (i=1; i<size; i++) {
      MPI_Send("hi msg plz", 10, MPI_CHAR, i, type, MPI_COMM_WORLD);
    }
    
    int message_count = 0;
   
    while(message_count < size - 1) {
      MPI_Recv(buffer, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      message_count++;
      
      printf("Message reveived from process: %s\n", buffer);
    }
    
    end_time = MPI_Wtime();
    exe_time = end_time - start_time;
    printf("Execution Time: %f\n", exe_time);
    
  } else { 
    MPI_Recv(buffer, 10, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
    
    char hostname[80]; gethostname(&hostname, 80);
    
    sprintf(buffer, "Hi, my name is %s and my rank is: %d", hostname, rank);
    
    MPI_Send(buffer, 100, MPI_CHAR, 0, type, MPI_COMM_WORLD);
  }
  
  MPI_Finalize();
}
