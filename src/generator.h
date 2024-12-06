#ifndef _GEN_H
#define _GEN_H

#include "./specs.h"

typedef struct
{
    char *src;
    char *dest;
    specs *specs;
} ascii_video_gen;

/**
 * Creating a new ascii video generator struct.
 */
ascii_video_gen *ascii_viden_gen_new(char *src, char *dest, specs *specs);


/**
 * Running the generator.
*/
int ascii_video_gen_run(ascii_video_gen *gen);

#endif