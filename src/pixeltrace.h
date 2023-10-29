#ifndef PIXELTRACE_H
#define PIXELTRACE_H

enum {
     PIXEL_TYPE_RECTANGLE = 1,
     PIXEL_TYPE_SCANLINE  = 2,
     PIXEL_TYPE_ELLIPSE   = 3
};

int pixel_trace(char* filename);

#endif
