#ifndef _GEN_H
#define _GEN_H

#include "./specs.h"

typedef struct
{
    char *src;
    specs *specs;
} video_player;

/**
 * Creatting a new video player struct.
 */
video_player *viden_player_new(char *src, specs *specs);

/**
 * Running the player.
 */
int video_player_run(video_player *player);

#endif