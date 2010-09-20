#include <stdio.h>
#include <string.h>
#include <math.h>

#include "physics.h"

const double dt = .01;
const int T = 100;

void main() {
  struct body bodies[N];
  struct body new_bodies[N];

  init_bodies(bodies);

  int t = 0, i = 0;
  for(t = 0; t < T; t++) {
    for(i = 0; i < N; i++) {
      double fx, fy, mass = bodies[i].mass;; 
      force(&fx, &fy, &bodies[i], bodies);
      
      double vx = new_velocity(bodies[i].vx, fx, dt, mass);
      double vy = new_velocity(bodies[i].vy, fy, dt, mass);
      double x  = new_position(bodies[i].x, vx, mass);
      double y  = new_position(bodies[i].y, vy, mass);

      printf("force[%d] \t\tx:%f\t\ty:%f\n", i, fx, fy);
      printf("velocity[%d] \t\tx:%f\t\ty:%f\n", i,  vx, vy);
      printf("position[%d] \t\tx:%f\t\ty:%f\n\n", i, x, y);

      new_bodies[i].x = x;
      new_bodies[i].y = y;
      new_bodies[i].vx = vx;
      new_bodies[i].vy = vy;
      new_bodies[i].mass = mass;
    }

    for(i = 0; i < N; i++) {
      bodies[i] = new_bodies[i];
    }
  }
}

