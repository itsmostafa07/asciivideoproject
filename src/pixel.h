#ifndef _PIXEL_H
#define _PIXEL_H

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

#endif