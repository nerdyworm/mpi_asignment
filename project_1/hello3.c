#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "mpi.h"
main(int argc, char **argv )
{
  char message[20]; 
  int i,rank, size, type=99;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0) {
    printf("Master: Hello slaves give me your messages\n");
    
    strcpy(message, "Hello, world");
    
    for (i=1; i<size; i++) 
      MPI_Send(message, 13, MPI_CHAR, i, type, MPI_COMM_WORLD);

    for (i=1; i<size; i++) {
      MPI_Recv(message, 20, MPI_CHAR, i, type, MPI_COMM_WORLD, &status);
      printf( "Message reveived from process %d : %s\n", i, message);
    }
    
  } else { 
  
    MPI_Recv(message, 20, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
    
    switch(rank) {
      case 1 :
        MPI_Send("Hello, I am John", 20, MPI_CHAR, 0, type, MPI_COMM_WORLD);
        break;
      case 2 :
        MPI_Send("Hello, I am Mary", 20, MPI_CHAR, 0, type, MPI_COMM_WORLD);
        break;
      case 3 :
        MPI_Send("Hello, I am Susan", 20, MPI_CHAR, 0, type, MPI_COMM_WORLD);
        break;
     }
  }

  MPI_Finalize();
}
