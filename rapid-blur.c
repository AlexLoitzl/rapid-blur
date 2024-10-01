/* BSD 3-Clause License */

/* Copyright (c) 2024, AlexLoitzl */

/* Redistribution and use in source and binary forms, with or without */
/* modification, are permitted provided that the following conditions are met: */

/* 1. Redistributions of source code must retain the above copyright notice, this */
/*    list of conditions and the following disclaimer. */

/* 2. Redistributions in binary form must reproduce the above copyright notice, */
/*    this list of conditions and the following disclaimer in the documentation */
/*    and/or other materials provided with the distribution. */

/* 3. Neither the name of the copyright holder nor the names of its */
/*    contributors may be used to endorse or promote products derived from */
/*    this software without specific prior written permission. */

/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE */
/* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL */
/* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "i3lfr.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void print_usage();

void process_image(char *dst_name, char *src_name, int radius, int times, int pixel) {
  int width, height, channels;

  // Read Image data
  unsigned char *src_image = stbi_load(src_name, &width, &height, &channels, 0);
  // Abort if there's not three channels.
  if (!src_image || channels != 3) {
    fprintf(stderr, "Error loading image: %s\n", stbi_failure_reason());
    exit(EXIT_FAILURE);
  }

  unsigned char *dst_image = (unsigned char *) malloc(width * height * channels);

  if (!dst_image) {
    fprintf(stderr, "Failed to allocate memory for output: %s\n", stbi_failure_reason());
    exit(EXIT_FAILURE);
  }

  if (pixel)
    pixelate(dst_image, src_image, height, width, radius);
  else
    box_blur(dst_image, src_image, height, width, radius, times);

  int ret = stbi_write_png(dst_name, width, height, channels, dst_image, width * channels);

  free(src_image);
  free(dst_image);

  if (!ret)
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  int c;
  int radius = 5, times = 3, pixel = 0;
  char *in = "", *out = "";

  // Parse args
  while ((c = getopt(argc, argv, "r:t:ph")) != -1) {
    switch (c)
      {
      case 'h':
        print_usage();
        exit(EXIT_SUCCESS);
        break;
      case 'r':
        radius = atoi (optarg);
        break;
      case 't':
        times = atoi (optarg);
        break;
      case 'p':
        pixel = 1;
        break;
      case '?':
        if (optopt == 'r' || optopt == 't')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        exit(EXIT_FAILURE);
      default:
        abort ();
      }
  }
  // Check args
  if (times < 0) {
    printf("Times has to be non-negative");
    exit(EXIT_FAILURE);
  } else if (radius < 0) {
    printf("Radius has to be non-negative");
    exit(EXIT_FAILURE);
  } else if (argc - optind != 2) {
    printf("Invalid arguments");
    exit(EXIT_FAILURE);
  } else {
    in = argv[optind++];
    out = argv[optind];
  }

  process_image(out, in, radius, times, pixel);

  exit(EXIT_SUCCESS);
}

void print_usage() {
  printf("Usage: rapid-blur [OPTION] INPUT OUTPUT.\n");
  printf("Box blur or pixelate an INPUT image file (png, jpg, ...).\n\n");
  printf(
"The following options are available:\n\
  -p,         Pixelate instead of blurring (The times parameter will be ignored)\n\
  -r PIXELS,  Each pixel's values will be interpolated with a radius of PIXELS pixels (Default: 5)\n\
  -t TIMES,   The blur will be repeated TIMES many times (Default: 3)");
}
