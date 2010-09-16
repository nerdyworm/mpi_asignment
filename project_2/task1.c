#include <stdio.h>
#include <math.h>

const float G = 1;

struct body
{
  double mass;
  double x, y, vx, vy;
} bodies[5];


void init_bodies() {

  bodies[0].mass = 10;
  bodies[0].x = 0;
  bodies[0].y = 0;
  bodies[0].vx = 0;
  bodies[0].vy = 0;

  bodies[1].mass = 10;
  bodies[1].x = 10;
  bodies[1].y = 10;
  bodies[1].vx = 0;
  bodies[1].vy = 0;

}

double distance(struct body *b1, struct body *b2) {
  int x = b2->x - b1->x;
  x *=x;

  int y = b2->y - b1->y;
  y *=y;
  
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


void main() {
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
}

