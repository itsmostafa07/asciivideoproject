#include "pixel.h"
#include "helpers.h"

/**
 * Creating a new pixel.
 */

pixel *pixel_new(short red, short green, short blue)
{
    UNIMPLEMENTED();
}

/**
 * Getting the intensity level of a specific pixel.
 *
 * Formula:
 *   I = 0.299*R + 0.587*G + 0.114*B
 */

unsigned short pixel_to_grayscale(pixel px)
{
    UNIMPLEMENTED();
}

/*
 * Converting the pixel to ascii character depends on its intensity.
 */

char pixel_to_ascii(pixel px)
{
    UNIMPLEMENTED();
}