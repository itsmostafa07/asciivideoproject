#define __USE_MISC
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <ncurses.h>

#include "generator.h"
#include "player.h"
#include "video.h"
#include "pixel.h"
#include "helpers.h"
#include "specs.h"
#include "frame.h"

int main(int argc, char **argv)
{
    const char *src, *out;

    if (argc <= 2)
    {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }
    src = argv[1];
    out = argv[2];

    video *vid = video_new(src, out);
    AVRational framerate = vid->codec_ctx->framerate;

    float fps = framerate.num / framerate.den;
    float duration = vid->fmt_ctx->duration / (float)AV_TIME_BASE;
    int frames_count = (int)fps * (int)duration;
    specs *specs = specs_new((int)fps * (int)duration, fps, duration, vid->video_stream->codecpar->width, vid->video_stream->codecpar->height);

    INFO("Creating specs file, content={%s}", specs_serialize(specs));

    ascii_video_gen *gen = ascii_viden_gen_new(src, out, vid);
    ascii_video_gen_run(gen);

    initscr();
    noecho();
    curs_set(0);
    INFO("w=%i, h=%i", vid->codec_ctx->width, vid->codec_ctx->height);

    video_player *vid_player = video_player_new(out, specs);

    if (video_player_run(vid_player) == -1)
    {
        ERROR("player failed to play the video");
    }

    endwin();
    // Clean up
    video_clean_up(vid);

    return 0;
}
