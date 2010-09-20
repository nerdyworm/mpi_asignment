//gcc testing.c -lX11

#define G 1
#define N 32

struct body {
  double mass, x, y, vx, vy;
};


void init_bodies(struct body bodies[]);

void force(double *fx, double *fy, struct body *body, struct body *bodies);

double distance(struct body *b1, struct body *b2);
double new_position(double current, double velocity, double dt);
double new_velocity(double current, double force, double dt, double mass);
