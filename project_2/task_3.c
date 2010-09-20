#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "mpi.h"

const float G = 1;

struct body
{
  double mass;
  double x, y, vx, vy;
} bodies[8];

const int N = 8;

void init_bodies() {

  bodies[0].mass = 10;
  bodies[0].x = 150;
  bodies[0].y = 150;
  bodies[0].vx = 0;
  bodies[0].vy = 0;

  bodies[1].mass = 10;
  bodies[1].x = 110;
  bodies[1].y = 110;
  bodies[1].vx = 0;
  bodies[1].vy = 3;

  bodies[2].mass = 10;
  bodies[2].x = 125;
  bodies[2].y = 125;
  bodies[2].vx = 0;
  bodies[2].vy = 0;

  bodies[3].mass = 20;
  bodies[3].x = 250;
  bodies[3].y = 250;
  bodies[3].vx = 0;
  bodies[3].vy = 50;

  bodies[4].mass = 15;
  bodies[4].x = 25;
  bodies[4].y = 25;
  bodies[4].vx = 0;
  bodies[4].vy = 0;

  bodies[5].mass = 7;
  bodies[5].x = 300;
  bodies[5].y = 250;
  bodies[5].vx = 0;
  bodies[5].vy = 0;

  bodies[6].mass = 5;
  bodies[6].x = 285;
  bodies[6].y = 2;
  bodies[6].vx = 0;
  bodies[6].vy = 0;

  bodies[7].mass = 2;
  bodies[7].x = 125;
  bodies[7].y = 125;
  bodies[7].vx = 0;
  bodies[7].vy = 0;

}

double distance(struct body *b1, struct body *b2) {
  int x = b2->x - b1->x;
  x *=x;

  int y = b2->y - b1->y;
  y *=y;
  
  int b = x + y;
  
  if(b <= 0)
    return 1;
  
  return sqrt(x + y);
} 

double force(struct body *b1, struct body *b2) {
  double r = distance(b1, b2);

  return (G * b1->mass * b2->mass) / (r * r);
}

double force_on_body(int index) {
  double f = 0.0;
  int i = 0;
  for(i = 0; i < 2; i++ ) {
    if(i == index) continue;
    f += force(&bodies[index], &bodies[i]);
  }

  return f;
}

double f_x(struct body *b1, struct body *b2) {
  double r = distance(b1, b2);
  double f1 = G * b1->mass * b2->mass / r * r;
  double f2 = (b2->x - b1->x) / r;
  double result = f1 * f2;

  return result;
}

double f_y(struct body *b1, struct body *b2) {
  double r = distance(b1, b2);
  double f1 = G * b1->mass * b2->mass / r * r;
  double f2 = (b2->y - b1->y) / r;
  double result = f1 * f2;

  return result;
}

double x_force(int index) {
  double f = 0.0;
  int i = 0;
  for(i = 0; i < 2; i++ ) {
    if(i == index) continue;
    f += f_x(&bodies[index], &bodies[i]);
  }

  return f;
}

double y_force(int index) {
  double f = 0.0;
  int i = 0;
  for(i = 0; i < 2; i++ ) {
    if(i == index) continue;
    f += f_y(&bodies[index], &bodies[i]);
  }

  return f;
}

/*
int main() {
  init_bodies();

  double d = distance(&bodies[0], &bodies[1]);
  double f = force(&bodies[0], &bodies[1]);
  printf("distance: %f\n", d);
  printf("force: %f\n", f);

  int t = 0, i = 0;
  for(t = 0; t < 10; t++) {
    for(i = 0; i < 2; i++) {
      double fx = x_force(i);
      double fy = y_force(i);
      
      printf("force[%d] \t\tx:%f\t\ty:%f\n", i, fx, fy);

      int m = bodies[i].mass;
      double dt = .1;

      bodies[i].vx = bodies[i].vx + fx * dt / m;
      bodies[i].vy = bodies[i].vy + fy * dt / m;
      
      
      printf("velocity[%d] \t\tx:%f\t\ty:%f\n", i,  bodies[i].vx, bodies[i].vy);

      bodies[i].x = bodies[i].x + bodies[i].vx * dt;
      bodies[i].y = bodies[i].y + bodies[i].vy * dt;

      
      printf("position[%d] \t\tx:%f\t\ty:%f\n\n", i, bodies[i].x, bodies[i].y);

    }
  }

  return 0;
}
*/

main(int argc, char **argv )
{
  
  int output = 0;

  init_bodies();

  struct body b[8];

  int rank, size = 4, root = 0, type = 99;
  
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  if(rank == 0) {
    if(output) printf("Master: Broadcasting a the data\n");
    MPI_Bcast(&bodies, sizeof(bodies), MPI_INT, root, MPI_COMM_WORLD);
    
    int max_iterations = 500, counter = 0;

    for(counter = 0; counter < max_iterations; counter++) {
      int i = 0, message_count = 0, message_sent = 0;
      while(1) {
        int j = 0;
        for(j = 1; j < size; j++) {
            if(output) printf("master: sending body:%d to %d\n", i, j);
  
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
            printf("\tx : %f", bodies[index].x);
            printf("\ty : %f", bodies[index].y);
            printf("\tvx: %f", bodies[index].vx);
            printf("\tvy: %f\n", bodies[index].vy);
          }

          bodies[index].x = new_values[1];
          bodies[index].y = new_values[2];
          bodies[index].vx = new_values[3];
          bodies[index].vy = new_values[4];
          
          if(output)
          {
            printf("master: new");
            printf("\tx : %f", bodies[index].x);
            printf("\ty : %f", bodies[index].y);
            printf("\tvx: %f", bodies[index].vx);
            printf("\tvy: %f\n\n", bodies[index].vy);
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
    MPI_Bcast(b, sizeof(b), MPI_INT, root, MPI_COMM_WORLD);
    if(output) printf("slave[%d]: received data\n", rank);

    while(1)
    {
      int index = 0;
    
      MPI_Recv(&index, 10, MPI_INT, 0, type, MPI_COMM_WORLD, &status);
      
      if(index > N) break;
      
      if(output) printf("slave[%d]: calculating values for body: %d\n", rank, index);

      double fx = 0.0, fy = 0.0;
      int i = 0;
      for(i = 0; i < 2; i++ ) {
        if(i == index) continue;
        fx += f_x(&b[index], &b[i]);
      }

      for(i = 0; i < 2; i++ ) {
        if(i == index) continue;
        fy += f_y(&b[index], &b[i]);
      }
   
      double new_values[5];
      new_values[0] = index;
      new_values[3] = fx;
      new_values[4] = fx;

      int m = b[index].mass;
      double dt = .1;

      b[index].vx = b[index].vx + fx * dt / m;
      b[index].vy = b[index].vy + fy * dt / m;
      
      new_values[3] = b[index].vx;
      new_values[4] = b[index].vy;

      b[index].x = b[index].x + b[index].vx * dt;
      b[index].y = b[index].y + b[index].vy * dt;
    
      new_values[1] = b[index].x;
      new_values[2] = b[index].y;
      
      MPI_Send(&new_values, 10, MPI_INT, root, type, MPI_COMM_WORLD);
    }
  }
  
  MPI_Finalize();
}

