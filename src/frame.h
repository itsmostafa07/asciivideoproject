#ifndef _FRAME_H
#define _FRAME_H

#include <stdint.h>

typedef struct
{
  short rgb[3];
} pixel;

/**
 * Creating a new pixel.
 */

pixel *pixel_new(short red, short green, short blue);

/**
 * Converting the pixel into gray scale.
 *
 * Formula:
 *   GrayScale = 0.299*R + 0.587*G + 0.114*B
 */

unsigned short pixel_grayscale(pixel px);

/*
 * Converting the pixel to ascii character depends on its intensity.
 */

char pixel_to_ascii(pixel px);

typedef struct
{
  uint64_t id; // the ID of the frame, its order in the video
  uint32_t height;
  uint32_t widht;
  pixel pixels[]; // Array of pixels for the frame.
} frame;

/*
 * Creating a new frame.
 */
frame *frame_new(uint64_t id, pixel pixels[], unsigned int width, unsigned int height);

/*
 * Converting the frame into Ascii Art.
 */
char *frame_to_ascii(frame *frame);

/*
 * Writing the frame that converted into Ascii Art to the path of the
 * destination file.
 */
int frame_ascii_write_to(frame *frame);
#endif
