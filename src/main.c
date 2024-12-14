#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <ncurses.h>

#define __USE_MISC
#include <unistd.h>

#include "video.h"
#include "pixel.h"
#include "helpers.h"
#include "specs.h"

#define FRAME_DELAY 33333 // Microseconds (1/30 second)

static void ascii_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
{
    FILE *file = fopen(filename, "wb");

    for (int y = 0; y < ysize; y++)
    {
        for (int x = 0; x < xsize; x++)
        {
            pixel px = pixel_new(
                buf[y * wrap + x],
                buf[y * wrap + x + 1],
                buf[y * wrap + x + 2]);
            char ascii_char = pixel_to_ascii(px);

            fputc(ascii_char, file);
            fputc(ascii_char, file);
            fputc(ascii_char, file);
            // printf("f");
        }
        fputc('\n', file);
    }

    fclose(file);
}

static void player(const char *prefix, int width, int height, int frames_count)
{
    int term_rows, term_cols;
    getmaxyx(stdscr, term_rows, term_cols); // Get terminal size

    int offset_y = (term_rows - height) / 2;
    int offset_x = (term_cols - width) / 2;

    WINDOW *frame_pad = newpad(height, width);
    if (!frame_pad)
    {
        fprintf(stderr, "Error: Could not create frame pad\n");
        return;
    }

    for (int i = 1; i <= frames_count; i++)
    {
        char filename[128];
        snprintf(filename, sizeof(filename), "%s-%i.ascii", prefix, i);

        // Open the ASCII file for reading
        FILE *file = fopen(filename, "r");
        if (!file)
        {
            fprintf(stderr, "Error: Could not open file %s\n", filename);
            break; // Exit loop on failure
        }

        werase(frame_pad);

        char line[width + 1];
        for (int y = 0; y < height; y++)
        {
            if (fgets(line, sizeof(line), file) == NULL)
                break; 

            mvwprintw(frame_pad, y, 0, "%.*s", width, line);
        }

        fclose(file); 

        prefresh(frame_pad, 0, 0, offset_y, offset_x, offset_y + height - 1, offset_x + width - 1);

        usleep(FRAME_DELAY); 
    }

    delwin(frame_pad);
}

static int decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt, const char *filename)
{
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0)
    {
        ERROR("Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0)
    {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            goto done;
        else if (ret < 0)
        {
            ERROR("Error during decoding\n");
            return -1;
        }

        INFO("Saving frame %ld: %ix%i\n", dec_ctx->frame_num, frame->width, frame->height);
        snprintf(buf, sizeof(buf), "%s-%ld.ascii", filename, dec_ctx->frame_num);

        ascii_save(frame->data[0], frame->linesize[0], frame->width, frame->height, buf);
        // usleep(FRAME_DELAY);
    }
    printf("Decoding is done\n");
done:
    return 0;
}

int main(int argc, char **argv)
{
    const char *filename, *outfilename;

    if (argc <= 2)
    {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }
    filename = argv[1];
    outfilename = argv[2];

    video *vid = video_new(filename, outfilename);
    AVRational framerate = vid->codec_ctx->framerate;

    float fps = framerate.num / framerate.den;
    float duration = vid->fmt_ctx->duration / (float)AV_TIME_BASE;
    int frames_count = (int)fps * (int)duration;
    specs *specs = specs_new((int)fps * (int)duration, fps, duration, vid->video_stream->codecpar->width, vid->video_stream->codecpar->height);

    INFO("Creating specs file, content={%s}", specs_serialize(specs));

    video_decode_frames(vid, decode);
    // Flush the decoder
    // decode(codec_ctx, frame, NULL, outfilename);

    initscr();
    noecho();    // Don't echo input characters
    curs_set(0); // Hide the cursor
    INFO("w=%i, h=%i", vid->codec_ctx->width, vid->codec_ctx->height);
    player(outfilename, vid->codec_ctx->width * 4, vid->codec_ctx->height, frames_count);
    endwin();
    // Clean up
    video_clean_up(vid);

    return 0;
}
