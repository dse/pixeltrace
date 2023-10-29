#ifndef PIXELTRACE2_H
#define PIXELTRACE2_H

struct pixel_trace {
     int width;
     int height;
     int** pixels;
     int*** look_dirns;
     int (*draw_moveto)(int, int);
     int (*draw_lineto)(int, int);
     int (*draw_closepath)();
     int (*draw_fill)();
};

enum { EAST, WEST, NORTH, SOUTH };

#define X_Y_AHEAD_LEFT(x, y, dirn) \
     ((x) + dx_ahead_left[dirn]), ((y) + dy_ahead_left[dirn])

#define X_Y_AHEAD_RIGHT(x, y, dirn) \
     ((x) + dx_ahead_right[dirn]), ((y) + dy_ahead_right[dirn])

#define IN_BOUNDS(pt, x, y) \
     (((x) > 0) && ((x) < (pt).width) && ((y) > 0) && ((y) < (pt).height))

#define PIXEL_IN_BOUNDS(pt, x, y) \
     (((x) >= 0 && (x) < (pt).width && (y) >= 0 && (y) < (pt).height))

#define MOVE(x, y, dirn) \
     (x += dx[dirn], y += dy[dirn])

int get_pixel(struct pixel_trace, int, int);
void set_pixel(struct pixel_trace, int, int, int);
int get_look_dirn(struct pixel_trace, int, int, int);
void set_look_dirn(struct pixel_trace, int, int, int, int);
void pixel_trace_2(struct pixel_trace);
void pixel_trace_2_fill(struct pixel_trace);

#endif
