#ifndef _GEN_H
#define _GEN_H

#include "./specs.h"

typedef struct
{
    char *src;
    char *dest;
    specs *specs;
} video_gen;

/**
 * Creatting a new video generator struct.
 */
video_gen *viden_gen_new(char *src, char *dest, specs *specs);


/**
 * Running the generator.
*/
int video_gen_run(video_gen *gen);

#endif