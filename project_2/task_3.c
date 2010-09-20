#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "mpi.h"

#include "physics.h"

const double dt = .001;

main(int argc, char **argv )
{
  struct body master_bodies[N];
  struct body slave_bodies[N];
  
  init_bodies(master_bodies);
  
  int output = 1, verbose_output = 0;
  
  int rank, size = 4, root = 0, type = 99;
  
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0) {
    if(verbose_output) printf("Master: Broadcasting a the data\n");
    
    MPI_Bcast(&master_bodies, sizeof(master_bodies), MPI_INT, root, MPI_COMM_WORLD);
    
    int max_iterations = 100, counter = 0;
    for(counter = 0; counter < max_iterations; counter++) {
      int i = 0, message_count = 0, message_sent = 0;
      while(1) {
        int j;
        for(j = 1; j < size; j++) {
            if(verbose_output) printf("master: sending body:%d to %d\n", i, j);
  
            MPI_Send(&i, sizeof(i), MPI_INT, j, type, MPI_COMM_WORLD);
           
            i++; message_sent++;  
            
            if(i >= N) break; 
        }

        while(message_count < message_sent) {
          double new_values[5];
          MPI_Recv(&new_values, 100, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          message_count++;

          int index = new_values[0];
        
          if(output)
          {
            printf("master: recv values for body: %d\n", index);
            printf("master: old");
            printf("\tx : %f", master_bodies[index].x);
            printf("\ty : %f", master_bodies[index].y);
            printf("\tvx: %f", master_bodies[index].vx);
            printf("\tvy: %f\n", master_bodies[index].vy);
          }

          master_bodies[index].x  = new_values[1];
          master_bodies[index].y  = new_values[2];
          master_bodies[index].vx = new_values[3];
          master_bodies[index].vy = new_values[4];
          
          if(output)
          {
            printf("master: new");
            printf("\tx : %f", master_bodies[index].x);
            printf("\ty : %f", master_bodies[index].y);
            printf("\tvx: %f", master_bodies[index].vx);
            printf("\tvy: %f\n\n", master_bodies[index].vy);
          }

        }
        
        message_count = 0;
        message_sent = 0;
        if(i >= N) break; 
      }
    }

    //let the slave break out of their loop
    int j = 0, break_signal = N + 1;
    for(j = 1; j < size; j++) {
      MPI_Send(& break_signal, 10, MPI_INT, j, type, MPI_COMM_WORLD);
    }
  
  } else { 
    MPI_Bcast(&slave_bodies, sizeof(slave_bodies), MPI_INT, root, MPI_COMM_WORLD);
    
    if(verbose_output) printf("slave[%d]: received data\n", rank);

    while(1) {
      int index = 0;
    
      MPI_Recv(&index, 10, MPI_INT, 0, type, MPI_COMM_WORLD, &status);
      
      if(index > N) break;
      
      if(verbose_output) printf("slave[%d]: calculating values for body: %d\n", rank, index);

      double fx, fy, mass = slave_bodies[index].mass;
      force(&fx, &fy, &slave_bodies[index], slave_bodies);

      double vx = new_velocity(slave_bodies[index].vx, fx, dt, mass);
      double vy = new_velocity(slave_bodies[index].vy, fy, dt, mass);
      double x  = new_position(slave_bodies[index].x, vx, mass);
      double y  = new_position(slave_bodies[index].y, vy, mass);
   
      double new_values[5];
      new_values[0] = index;
      new_values[1] = x;
      new_values[2] = y;
      new_values[3] = fx;
      new_values[4] = fy;
      
      MPI_Send(&new_values, 10, MPI_INT, root, type, MPI_COMM_WORLD);
    }
  }
  
  MPI_Finalize();
}
