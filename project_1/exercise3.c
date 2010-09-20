#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "mpi.h"
main(int argc, char **argv )
{
  char buffer[50];
  int rank, root = 0;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0) {
    MPI_Bcast("FUN BROADCASTING\n", 50, MPI_CHAR, root, MPI_COMM_WORLD);
    printf("Master: Broadcasting a message\n");
  } else { 
    MPI_Bcast(buffer, 50, MPI_CHAR, root, MPI_COMM_WORLD);
    printf("Slave got message: %s", buffer);
  }
  
  MPI_Finalize();
}
