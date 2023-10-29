#include "pixeltrace2.h"

int dx_ahead_left[4]  = { 0,     -1,    -1,    0     };
int dx_ahead_right[4] = { 0,     -1,    0,     -1    };
int dy_ahead_left[4]  = { -1,    0,     -1,    0     };
int dy_ahead_right[4] = { 0,     -1,    -1,    0     };
int left[4]           = { NORTH, SOUTH, WEST,  EAST  };
int right[4]          = { SOUTH, NORTH, EAST,  WEST  };
int dx[4]             = { 1,     -1,    0,     0     };
int dy[4]             = { 0,     0,     -1,    1     };

int get_pixel(struct pixel_trace pt, int x, int y) {
     if (!PIXEL_IN_BOUNDS(pt, x, y)) {
          return 0;
     }
     return pt.pixels[x][y];
}

void set_pixel(struct pixel_trace pt, int x, int y, int flag) {
     if (!PIXEL_IN_BOUNDS(pt, x, y)) {
          return;
     }
     pt.pixels[x][y] = flag;
}

int get_look_dirn(struct pixel_trace pt, int x, int y, int dirn) {
     if (!PIXEL_IN_BOUNDS(pt, x, y)) {
          return 0;
     }
     return pt.look_dirns[x][y][dirn];
}

void set_look_dirn(struct pixel_trace pt, int x, int y, int dirn, int flag) {
     if (!PIXEL_IN_BOUNDS(pt, x, y)) {
          return;
     }
     pt.look_dirns[x][y][dirn] = flag;
}

void pixel_trace_2(struct pixel_trace pt) {
     int x, y, x_orig, y_orig, dirn;
     /* at this point all pixels are either zero or one */
     for (x = 0; x < pt.width; x += 1) {
          for (y = 0; y < pt.height; y += 1) {
               if (get_pixel(pt, x, y) != 1) {
                    continue;
               }
               (pt.draw_moveto)((double)x, (double)y);
               x_orig = x;
               y_orig = y;
               dirn = EAST;
               while (1) {
                    while (IN_BOUNDS(pt, x, y) &&
                           get_pixel(pt, X_Y_AHEAD_LEFT(x, y, dirn)) == 0 &&
                           get_pixel(pt, X_Y_AHEAD_RIGHT(x, y, dirn)) == 1) {
                         set_look_dirn(pt, X_Y_AHEAD_RIGHT(x, y, dirn), right[dirn], 1);
                         set_pixel(pt, X_Y_AHEAD_RIGHT(x, y, dirn), 2);
                         MOVE(x, y, dirn);
                         if (x == x_orig && y == y_orig) {
                              goto done;
                         }
                    }
                    (pt.draw_lineto)((double)x, (double)y);
                    if ((dirn == EAST && x == pt.width) || (dirn == WEST && x == 0) ||
                        (dirn == SOUTH && y == pt.height) || (dirn == NORTH && y == 0)) {
                         dirn = right[dirn];
                    } else if (get_pixel(pt, X_Y_AHEAD_LEFT(x, y, dirn)) == 0) {
                         dirn = left[dirn];
                    } else {
                         dirn = right[dirn];
                    }
               }
          done:
               pixel_trace_2_fill(pt);
          }
     }
}

void pixel_trace_2_fill(struct pixel_trace pt) {
     (pt.draw_path_done)();
     /* mark any other pixels inside the contour as done */
     for (int x1 = 0; x1 < pt.width; x1 += 1) {
          for (int y1 = 0; y1 < pt.height; y1 += 1) {
               if (get_pixel(pt, x1, y1) == 0) {
                    continue;
               }
               int x2 = x1, y2 = y1;
               for (int dirn = 0; dirn < 4; dirn += 1) {
                    if (get_look_dirn(pt, x1, y1, dirn)) {
                         for (MOVE(x2, y2, dirn);
                              IN_BOUNDS(pt, x2, y2) && get_pixel(pt, x2, y2);
                              MOVE(x2, y2, dirn)) {
                              set_pixel(pt, x2, y2, 2);
                         }
                    }
               }
          }
     }
}
