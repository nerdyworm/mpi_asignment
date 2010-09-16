//gcc testing.c -lX11

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


const float G = 1;
const int N = 8;
struct body
{
  double mass;
  double x, y, vx, vy;
} bodies[8];


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
  bodies[1].vy = 0;

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
  for(i = 0; i < N; i++ ) {
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
  for(i = 0; i < N; i++ ) {
    if(i == index) continue;
    f += f_x(&bodies[index], &bodies[i]);
  }

  return f;
}

double y_force(int index) {
  double f = 0.0;
  int i = 0;
  for(i = 0; i < N; i++ ) {
    if(i == index) continue;
    f += f_y(&bodies[index], &bodies[i]);
  }

  return f;
}
















Display *dis;
int screen;
Window win;
GC gc;

int x = 0;
int moving_right = 1;


void draw() {
  XClearArea(dis, win, 0, 0, 0, 0, False);
  int i = 0;
  for(i = 0; i < N; i++) {
    
    //XDrawRectangle(dis, win, gc, bodies[i].x, bodies[i].y, 10, 10);
    XDrawPoint(dis, win, gc, bodies[i].x, bodies[i].y);
  }  
  XFlush(dis);
}

void update() {
  
 int t = 0, i = 0;
  for(t = 0; t < 1; t++) {
    for(i = 0; i < N; i++) {
      double fx = x_force(i);
      double fy = y_force(i);
      
      //printf("force[%d] \t\tx:%f\t\ty:%f\n", i, fx, fy);

      int m = bodies[i].mass;
      double dt = .1;

      bodies[i].vx = bodies[i].vx + fx * dt / m;
      bodies[i].vy = bodies[i].vy + fy * dt / m;
      
      
      //printf("velocity[%d] \t\tx:%f\t\ty:%f\n", i,  bodies[i].vx, bodies[i].vy);

      bodies[i].x = (int) bodies[i].x + bodies[i].vx * dt;
      bodies[i].y = (int) bodies[i].y + bodies[i].vy * dt;

      
      //printf("position[%d] \t\tx:%f\t\ty:%f\n\n", i, bodies[i].x, bodies[i].y);

    }
  }
}

void init_x() {
  dis = XOpenDisplay(NULL);

  screen = DefaultScreen(dis);
  unsigned long white = WhitePixel(dis, screen);
  unsigned long black = BlackPixel(dis, screen);

  win = XCreateSimpleWindow(dis,
                            DefaultRootWindow(dis),
                            50, 50,   // origin
                            500, 500, // size
                            0, white, // border
                            black );  // backgd
  XMapWindow( dis, win );

  XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);

  gc = XCreateGC(dis, win, 0, 0);

  long eventMask = StructureNotifyMask;
  XSelectInput( dis, win, eventMask );

  XEvent evt;
  do{
    XNextEvent(dis, &evt );// calls XFlush
  }while( evt.type != MapNotify );

  gc = XCreateGC(dis, win, 0, NULL );
  
  XSetForeground(dis, gc, white);
};

void close_x() {
  XFreeGC(dis, gc);
  XDestroyWindow(dis,win);
  XCloseDisplay(dis);
}






main () {
  init_bodies();
  init_x();
  
  double d = distance(&bodies[0], &bodies[1]);
  double f = force(&bodies[0], &bodies[1]);

  while(1) {
    update();
    draw();
    usleep(50000);
    //sleep(1);  
  }
}


