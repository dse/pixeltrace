#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "pixeltrace.h"

char* output_format = NULL;
char* input_format = NULL;

static struct option long_options[] = {
     { "output-format", 1, 0, 'o' },
     { "input-format", 1, 0, 'i' },
     { 0, 0, 0, 0 }
};

int main(int argc, char** argv) {
     int c;
     int digit_optind = 0;
     while (1) {
          int this_option_optind = optind ? optind : 1;
          int option_index = 0;
          c = getopt_long(argc, argv, "o:i:", long_options, &option_index);
          if (c == -1) {
               break;
          }
          switch (c) {
          case 'o':
               if (output_format) free(output_format);
               output_format = strdup(optarg);
               break;
          case 'i':
               if (input_format) free(input_format);
               input_format = strdup(optarg);
               break;
          case '?':
               break;
          default:
               fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
               exit(1);
          }
     }
     if (optind >= argc) {
          fprintf(stderr, "not enough arguments\n");
          exit(1);
     }
     char* filename = argv[optind];
     pixel_trace(filename);
     exit(0);
}
