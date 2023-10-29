#ifndef PIXELTRACE_H
#define PIXELTRACE_H

enum {
     PIXEL_TYPE_NONE      = 0,  /* no pixel type selected */
     PIXEL_TYPE_PLAIN     = 1,  /* straight up plain pixel fonts */
     PIXEL_TYPE_RECTANGLE = 2,  /* mainly for LCD display module lookin' fonts */
     PIXEL_TYPE_SCANLINE  = 3,  /* mainly for CRT lookin' fonts */
     PIXEL_TYPE_CIRCLE    = 4   /* more for dot matrix printing */
};

int pixel_trace(char* filename);

void draw_path_done();
void draw_lineto(double, double);
void draw_moveto(double, double);
void draw_arc(double, double, double);

#endif
