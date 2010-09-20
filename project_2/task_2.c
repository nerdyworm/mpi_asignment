//gcc testing.c -lX11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "physics.h"

const double dt = .00001;

Display *dis;
int screen;
Window win;
GC gc;

struct body bodies[N];
struct body new_bodies[N];

void draw() {
  XClearArea(dis, win, 0, 0, 0, 0, False);
  int i = 0;
  for(i = 0; i < N; i++) {
    //XDrawPoint(dis, win, gc, bodies[i].x, bodies[i].y);;
    XDrawArc(dis, win, gc, bodies[i].x, bodies[i].y, bodies[i].mass, bodies[i].mass, 0, 360 * 64);
  }  
  XFlush(dis);
}

void update() {
  int i;
  for(i = 0; i < N; i++) {
    double fx, fy, mass = bodies[i].mass;; 
    force(&fx, &fy, &bodies[i], bodies);
    
    double vx = new_velocity(bodies[i].vx, fx, dt, mass);
    double vy = new_velocity(bodies[i].vy, fy, dt, mass);
    double x  = new_position(bodies[i].x, vx, mass);
    double y  = new_position(bodies[i].y, vy, mass);

    //printf("force[%d] \t\tx:%f\t\ty:%f\n", i, fx, fy);
    //printf("velocity[%d] \t\tx:%f\t\ty:%f\n", i,  vx, vy);
    //printf("position[%d] \t\tx:%f\t\ty:%f\n\n", i, x, y);

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

void init_x() {
  dis = XOpenDisplay(NULL);

  screen = DefaultScreen(dis);
  unsigned long white = WhitePixel(dis, screen);
  unsigned long black = BlackPixel(dis, screen);

  win = XCreateSimpleWindow(dis,
                            DefaultRootWindow(dis),
                            50, 50,   // origin
                            800, 800, // size
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
  init_bodies(bodies);
  init_x();

  while(1) {
    update();
    draw();
    usleep(10000);
  }
}
