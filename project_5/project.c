#include <stdio.h> 
#include <string.h> 
#include <stddef.h> 
#include <stdlib.h> 
#include "omp.h"
#include "md5.h"
#include "mpi.h"

#define CHAR_LOWER 33
#define CHAR_UPPER 126

char * md5(char *str)
{
  md5_byte_t digest[16];
  md5_state_t state;
  md5_init  (&state);
	md5_append(&state, str, strlen(str));
	md5_finish(&state, digest);
  
  char hex[16 * 2 + 1]; int i;
  for (i = 0; i < 16; ++i)
    sprintf(hex + i * 2, "%02x", digest[i]);
  
  return strdup(hex);
}

void openmp_code(char *hex, int start, int end, char *solution)
{
  int finished = 0;
  
  char str[6];

  #pragma omp parallel shared(start, end, finished) private(str)
  {
    int a, b, c, d, e, f;
    
    #pragma omp for
    for(a = start; a <= end; a++)
    for(b = CHAR_LOWER; b <= CHAR_UPPER; b++)
    for(c = CHAR_LOWER; c <= CHAR_UPPER; c++)
    for(d = CHAR_LOWER; d <= CHAR_UPPER; d++)
    for(e = CHAR_LOWER; e <= CHAR_UPPER; e++)
    for(f = CHAR_LOWER; f <= CHAR_UPPER; f++)
    {
      if(finished == 1) continue;

      str[0] = a;
      str[1] = b;
      str[2] = c;
      str[3] = d;
      str[4] = e;
      str[5] = f;

      char *sum = md5(str);
      int result = strcmp(sum, hex);
      free(sum);
      
      if(result == 0) 
      {
        strcpy(solution, str);
        
        finished = 1;
      }
    }
  }
}

int main(int argc, char **argv)
{
  int data[2];
  int i, rank, size, type = 404;
  MPI_Status status;
  MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size); 
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //just generate some md5 hash for testing
  char *hex =  md5("c!#:Ka");

  if(rank == 0)
  {
    printf("there are %d machines to send data to.\n", size);
    
    int work_per_node = (CHAR_UPPER - CHAR_LOWER) / size;

    printf("each machine should do %d outermost itereations\n", work_per_node);

    int start = CHAR_LOWER;
    int end   = CHAR_LOWER + work_per_node;

    for(i = 1; i < size; i++)
    {
      start = end + 1;
      end   = start + work_per_node;

      printf("machine: %d will start with %d and end with %d\n", i, start, end);
      data[0] = start;
      data[1] = end;

      MPI_Send(data, 2, MPI_INT, i, type, MPI_COMM_WORLD);
    }

    //rank 0's work
    data[0] = CHAR_LOWER;
    data[1] = CHAR_LOWER + work_per_node;

  }
  else
	  MPI_Recv(data, 2, MPI_INT, 0, type, MPI_COMM_WORLD, &status); 

  printf("I am machine %d i should start at %d and end at %d\n", rank, data[0], data[1]);
  
  char solution[6]; 
    
  openmp_code(hex, data[0], data[1], solution);

  printf("The string is: \"%s\"\n", solution);

  //someone found an answer, no more work to do so abort
  MPI_Abort(MPI_COMM_WORLD, 0);
}
