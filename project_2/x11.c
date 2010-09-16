//gcc testing.c -L/usr/X11R6/lib -lX11 -1m

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

Display *dis;
int screen;
Window win;
GC gc;

int x = 0;
int moving_right = 1;


main () {
  init_x();

  while(1) {
    update();
    draw();
    usleep(1000);
  }
}

void draw() {
  XClearArea(dis, win, 0, 0, 0, 0, False);

  XDrawRectangle(dis, win, gc, x, 50, 50, 50);
  
  XFlush(dis);
}

void update() {
  if(moving_right && x < 200) {
    x++;
  }

  if(!moving_right && x > 0) {
    x--;
  }

  if(x == 200)
    moving_right = 0;
  if(x == 0)
    moving_right = 1;

}

void init_x() {
  dis = XOpenDisplay(NULL);

  screen = DefaultScreen(dis);
  unsigned long white = WhitePixel(dis, screen);
  unsigned long black = BlackPixel(dis, screen);

  win = XCreateSimpleWindow(dis,
                            DefaultRootWindow(dis),
                            50, 50,   // origin
                            200, 200, // size
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


