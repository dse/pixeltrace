#ifndef PIXELTRACE_H
#define PIXELTRACE_H

enum {
     PIXEL_TYPE_NONE      = 0,
     PIXEL_TYPE_PLAIN     = 1,
     PIXEL_TYPE_RECTANGLE = 2,
     PIXEL_TYPE_SCANLINE  = 3,
     PIXEL_TYPE_ELLIPSE   = 4
};

int pixel_trace(char* filename);

#endif
