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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "i3lfr.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void process_image(char *dst_name, char *src_name, int radius, int times) {
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

  box_blur(dst_image, src_image, height, width, radius, times);

  if(!stbi_write_png(dst_name, width, height, channels, dst_image, width * channels)) {
    printf("Error");
    exit(EXIT_FAILURE);
  }
  free(src_image);
  free(dst_image);
}

int main(int argc, char *argv[])
{
  int c;
  int radius = 5, times = 3;
  char *in = "", *out = "";

  while ((c = getopt(argc, argv, "i:o:r:t")) != -1) {
    switch (c)
      {
      case 'i':
        in = optarg;
        break;
      case 'o':
        out = optarg;
        break;
      case 'r':
        radius = atoi (optarg);
        break;
      case 't':
        times = atoi (optarg);
        break;
      default:
        break;
    }
  }

  /* if (!(src && dst)) { */
  /*   fprintf(stderr, "Specify in- and output files\n"); */
  /*   return (EXIT_FAILURE); */
  /* } */
  process_image(out, in, radius, times);
  /* if (!(src && dst)) { */
  /*   fprintf(stderr, "Specify in- and output files\n"); */
  /*   return (EXIT_FAILURE); */
  /* } */

  /* fseek(src, 0L, SEEK_END); */
  /* size_t size = ftell(src); */
  /* fseek(src, 0L, SEEK_SET); */

  /* unsigned char * preblur = (unsigned char *) malloc(size); */
  /* unsigned char * postblur = (unsigned char *) malloc(size); */

  /* fread(preblur, sizeof(unsigned char), size, src); */
  /* fclose(src); */

  /* if (radius < 0 || times < 0) { */
  /*   fprintf(stderr, "Radius has to be non-negative!\n"); */
  /*   free(preblur); */
  /*   free(postblur); */
  /*   exit(EXIT_FAILURE); */
  /* } */
  /* box_blur(postblur, preblur, height, width, radius, times); */

  /* fwrite(postblur, sizeof(unsigned char), size, dst); */
  /* fclose(dst); */

  /* free(preblur); */
  /* free(postblur); */
  return 0;
}
