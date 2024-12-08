#ifndef _PIXEL_H
#define _PIXEL_H
#include <stdint.h>

// There are 10 levels of brightness. It depends on the intensity (brightness) of the pixel.
const char ASCII_CHARS[10] = "@\%#*+=-:. ";

typedef struct
{
    uint32_t rgb[3];
} pixel;

/**
 * Creating a new pixel.
 */

pixel *pixel_new(uint32_t red, uint32_t green, uint32_t blue);

/**
 * Getting the intensity level of a specific pixel.
 *
 * Formula:
 *   I = 0.299*R + 0.587*G + 0.114*B
 */

uint32_t pixel_intensity(pixel px);

/**
 * Converting the pixel to ascii character depends on its intensity (Grayscale value).
 *
 * You can find the index of the suitable ascii code using this formula:
 *  Index = I/255 * strlen(ASCII_CHARS)
 *
 * where I is the intensity of the pixel
 */

char pixel_to_ascii(pixel px);

#endif