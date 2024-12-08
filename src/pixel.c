#include "pixel.h"
#include "helpers.h"
#include <stdint.h>
/**
 * Creating a new pixel.
 */

pixel *pixel_new(uint32_t red, uint32_t green, uint32_t blue)
{
    UNIMPLEMENTED();
}

/**
 * Getting the intensity level of a specific pixel.
 *
 * Formula:
 *   I = 0.299*R + 0.587*G + 0.114*B
 */

uint32_t pixel_intensity(pixel px)
{
    UNIMPLEMENTED();
}

/**
 * Converting the pixel to ascii character depends on its intensity (Grayscale value).
 *
 * You can find the index of the suitable ascii code using this formula:
 *  index = I/255 * strlen(ASCII_CHARS)
 *
 * where I is the intensity of the pixel
 */

char pixel_to_ascii(pixel px)
{
    UNIMPLEMENTED();
}