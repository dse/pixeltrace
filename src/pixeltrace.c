/**
 * pixeltrace.c - activated like 'autotrace' in fontforge to trace BDF
 * fonts into exact pixel TTF fonts.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pixeltrace.h"
#include "endian.h"

/* quick and dirty packed structure lol */
char color_data[4];
unsigned char* red   = (unsigned char*)(color_data + 0);
unsigned char* green = (unsigned char*)(color_data + 1);
unsigned char* blue  = (unsigned char*)(color_data + 2);

/* quick and dirty packed structure lol */
char bmp_header[54];
uint32_t* bmp_bytes            = (uint32_t*)(bmp_header + 2);
uint32_t* bmp_reserved         = (uint32_t*)(bmp_header + 6);
uint32_t* bmp_data_offset      = (uint32_t*)(bmp_header + 10);
uint32_t* bmp_size             = (uint32_t*)(bmp_header + 14);
uint32_t* bmp_width            = (uint32_t*)(bmp_header + 18);
uint32_t* bmp_height           = (uint32_t*)(bmp_header + 22);
uint16_t* bmp_planes           = (uint16_t*)(bmp_header + 26);
uint16_t* bmp_bpp              = (uint16_t*)(bmp_header + 28);
uint32_t* bmp_compression      = (uint32_t*)(bmp_header + 30);
uint32_t* bmp_image_size       = (uint32_t*)(bmp_header + 34);
uint32_t* bmp_x_px_per_m       = (uint32_t*)(bmp_header + 38);
uint32_t* bmp_y_px_per_m       = (uint32_t*)(bmp_header + 42);
uint32_t* bmp_colors_used      = (uint32_t*)(bmp_header + 46);
uint32_t* bmp_important_colors = (uint32_t*)(bmp_header + 50);

int pixel_trace(char* filename) {
     int pixel_trace_success = 0;
     int ofs = 0;
     FILE* fh = NULL;
     unsigned char* pixel_data = NULL;
     size_t bytes;
     double brightnesses[2];
     int black_color;
     int white_color;
     int fread_bytes_per_row;
     int pixel_type = PIXEL_TYPE_RECTANGLE; /* or _SCANLINE or _ELLIPSE */
     char* getenv_val;
     char* val = NULL;
     double pixel_height = NAN;

     fh = fopen(filename, "rb");
     if (fh == NULL) {
          perror(filename);
          goto done;
     }
     bytes = fread(bmp_header, 1, 54, fh);
     ofs += bytes;
     if (bytes < 54) {
          fprintf(stderr, "%s too small\n", filename);
          goto done;
     }
     *bmp_bytes             = le32toh(*bmp_bytes);
     *bmp_reserved          = le32toh(*bmp_reserved);
     *bmp_data_offset       = le32toh(*bmp_data_offset);
     *bmp_size              = le32toh(*bmp_size);
     *bmp_width             = le32toh(*bmp_width);
     *bmp_height            = le32toh(*bmp_height);
     *bmp_planes            = le16toh(*bmp_planes);
     *bmp_bpp               = le16toh(*bmp_bpp);
     *bmp_compression       = le32toh(*bmp_compression);
     *bmp_image_size        = le32toh(*bmp_image_size);
     *bmp_x_px_per_m        = le32toh(*bmp_x_px_per_m);
     *bmp_y_px_per_m        = le32toh(*bmp_y_px_per_m);
     *bmp_colors_used       = le32toh(*bmp_colors_used);
     *bmp_important_colors  = le32toh(*bmp_important_colors);

     if (bmp_header[0] != 'B' || bmp_header[1] != 'M') {
          fprintf(stderr, "%s: invalid signature\n", filename);
          goto done;
     }
     if (*bmp_bpp != 1) {
          fprintf(stderr, "%s: %d bpp not supported\n", filename, *bmp_bpp);
          goto done;
     }

     for (int i = 0; i < 2; i += 1) {
          bytes = fread(color_data, 1, 4, fh);
          ofs += bytes;
          if (bytes < 4) {
               fprintf(stderr, "%s too small\n", filename);
               goto done;
          }
          brightnesses[i] = (0.2126 * *red + 0.7152 * *green + 0.0722 * *blue) / 255.0;
     }
     black_color = brightnesses[0] < brightnesses[1] ? 0 : 1;
     white_color = brightnesses[0] < brightnesses[1] ? 1 : 0;

     if (0 != fseek(fh, *bmp_data_offset, SEEK_SET)) {
          perror(filename);
          goto done;
     }

     fread_bytes_per_row = ((*bmp_width + 31) / 32) * 4;
     if (NULL == (pixel_data = malloc(fread_bytes_per_row))) {
          perror("malloc");
          goto done;
     }

     if (NULL != (getenv_val = getenv("PIXELTRACE_HEIGHT"))) {
          if (sscanf(getenv_val, "%lf", &pixel_height) < 1) {
               fprintf(stderr, "invalid pixel height value: %s\n", getenv_val);
               goto done;
          }
          if (pixel_height < 0) {
               pixel_height = 0;
          } else if (pixel_height > 1) {
               pixel_height = 1;
          }
     }

     printf("%%!PS-Adobe-3.0 EPSF-3.0\n");
     printf("%%%%Creator: pixeltrace\n");
     printf("%%%%LanguageLevel: 2\n");
     printf("%%%%BoundingBox: 0 0 %d %d\n", *bmp_width, *bmp_height);
     printf("%%%%HiResBoundingBox: 0 0 %d.000000 %d.000000\n", *bmp_width, *bmp_height);
     printf("%%%%Pages: 1\n");
     printf("%%%%EndComments\n");
     printf("%%%%Page: 1 1\n");
     for (int y = 0; y < *bmp_height; y += 1) {
          bytes = fread(pixel_data, 1, fread_bytes_per_row, fh);
          if (bytes < fread_bytes_per_row) {
               fprintf(stderr, "%s too small\n", filename);
               goto done;
          }
          if (black_color == 1) {
               for (int i = 0; i < fread_bytes_per_row; i += 1) {
                    pixel_data[i] = ~pixel_data[i];
               }
          }
          int y1 = y;
          int y2 = y + 1;
          int x1 = 0;
          int x2 = 0;
          while (x1 < *bmp_width) {
               while (x1 < *bmp_width) {
                    /* skip the '0' bits */
                    int bit = 1 & (pixel_data[x1 / 8] >> (7 - x1 % 8));
                    if (bit == 0) {
                         break;
                    }
                    x1 += 1;
               }
               if (x1 >= *bmp_width) {
                    break;
               }
               x2 = x1;
               while (x2 < *bmp_width) {
                    /* count the '1' bits */
                    int bit = 1 & (pixel_data[x2 / 8] >> (7 - x2 % 8));
                    if (bit == 1) {
                         break;
                    }
                    x2 += 1;
               }
               if (!isnan(pixel_height)) {
                    double dy2 = y1 + (y2 - y1) * pixel_height;
                    printf("%d %d moveto %d %lf lineto %d %lf lineto %d %d lineto 0 setlinewidth 0 setgray closepath fill\n",
                           x1, y1, x1, dy2, x2, dy2, x2, y1);
               } else {
                    printf("%d %d moveto %d %d lineto %d %d lineto %d %d lineto 0 setlinewidth 0 setgray closepath fill\n",
                           x1, y1, x1, y2, x2, y2, x2, y1);
               }
               x1 = x2;
          }
     }
     printf("%%%%EOF\n");
     pixel_trace_success = 1;
done:
     if (fh != NULL) {
          fclose(fh);
          fh = NULL;
     }
     if (pixel_data != NULL) {
          free(pixel_data);
          pixel_data = NULL;
     }
     return pixel_trace_success;
}
