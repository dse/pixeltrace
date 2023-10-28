#ifndef PIXELTRACE_H
#define PIXELTRACE_H

enum {
     PIXEL_TYPE_SQUARE   = 1,
     PIXEL_TYPE_SCANLINE = 2,
     PIXEL_TYPE_CIRCLE   = 3
};

int pixel_trace(char* filename);

#endif
