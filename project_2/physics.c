#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "physics.h"

void init_bodies(struct body bodies[]){
  bodies[0].mass = 10;
  bodies[0].x = 150;
  bodies[0].y = 150;
  bodies[0].vx = 0;
  bodies[0].vy = 0;

  bodies[1].mass = 10;
  bodies[1].x = 110;
  bodies[1].y = 110;
  bodies[1].vx = 0;
  bodies[1].vy = 0;

  bodies[2].mass = 10;
  bodies[2].x = 125;
  bodies[2].y = 125;
  bodies[2].vx = 0;
  bodies[2].vy = 0;

  bodies[3].mass = 15;
  bodies[3].x = 250;
  bodies[3].y = 250;
  bodies[3].vx = 0;
  bodies[3].vy = 0;

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
  
  if(N > 8) { 
    int i;
    for(i = 0; i < N; i++) {
      bodies[i].mass = (int) rand() % 20 + 1;
      bodies[i].x = 200;
      bodies[i].y = 200;
      bodies[i].vx = (-1 + 2 * ((float)rand())/RAND_MAX) / 5;
      bodies[i].vy = (-1 + 2 * ((float)rand())/RAND_MAX) / 5;

    }
  }
}

double distance(struct body *b1, struct body *b2) {
  int x = b2->x - b1->x;
  x *=x;

  int y = b2->y - b1->y;
  y *=y;
  
  int b = x + y;
  
  if(b == 0)
    return 1;
  else if( b < 0 ) {
    return -1 * sqrt(abs(b));
  }
  
  return sqrt(b);
}

//repeating code repeats itself
double f_x(struct body *b1, struct body *b2) {
  double r = distance(b1, b2);
  double f1 = G * b1->mass * b2->mass / r * r;
  double f2 = (b2->x - b1->x) / r;
  double result = f1 * f2;
  return result;
}

//repeating code repeats itself
double f_y(struct body *b1, struct body *b2) {
  double r = distance(b1, b2);
  double f1 = G * b1->mass * b2->mass / r * r;
  double f2 = (b2->y - b1->y) / r;
  double result = f1 * f2;
  return result;
}

void force(double *fx, double *fy, struct body *body, struct body *bodies) {
  *fx = 0.0, *fy = 0.0;
  int i = 0;
  for(i = 0; i < N; i++ ) {
    *fx += f_x(body, &bodies[i]);
    *fy += f_y(body, &bodies[i]);
  }
}

double new_velocity(double current, double force, double dt, double mass) {
  return current + force * dt / mass;
}

double new_position(double current, double velocity, double dt) {
  return current + velocity * dt;
}



