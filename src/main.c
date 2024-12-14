#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <ncurses.h>

#define __USE_MISC
#include <unistd.h>

#include "video.h"
#include "helpers.h"

#define FRAME_DELAY 33333 // Microseconds (1/30 second)

static void ascii_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
{
    FILE *file = fopen(filename, "wb");
    const char *ascii_map = " .:-=+*%#@";
    int ascii_len = strlen(ascii_map);

    for (int y = 0; y < ysize; y++)
    {
        for (int x = 0; x < xsize; x++)
        {
            // Map pixel intensity (0-255) to an ASCII character
            unsigned char pixel_value = buf[y * wrap + x];
            char ascii_char = ascii_map[(pixel_value * (ascii_len - 1)) / 255];
            fputc(ascii_char, file);
            fputc(ascii_char, file);
            fputc(ascii_char, file);
            // printf("f");
        }
        fputc('\n', file); // New line for each row
    }

    // fwrite(file_buf, xsize * ysize, xsize * ysize, file);
    fclose(file);
}

static void player(const char *prefix, int width, int height)
{

    for (int i = 1; i <= 450; i++)
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

        char ch;
        while ((ch = fgetc(file)) != EOF)
        {
            printw("%c", ch);
        }

        fclose(file); // Close the file

        // Pause for the frame delay
        refresh();           // Refresh the screen to display the frame
        usleep(FRAME_DELAY); // Delay in microseconds (e.g., 33ms for ~30fps)

        clear(); // Clear the screen for the next frame
    }
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

    video_decode_frames(vid, decode);
    // Flush the decoder
    // decode(codec_ctx, frame, NULL, outfilename);

    initscr();
    noecho();    // Don't echo input characters
    curs_set(0); // Hide the cursor
    INFO("w=%i, h=%i", vid->codec_ctx->width, vid->codec_ctx->height);
    player(outfilename, vid->codec_ctx->width, vid->codec_ctx->height);
    endwin();
    // Clean up
    video_clean_up(vid);

    return 0;
}
