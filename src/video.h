#ifndef _VIDEO_H
#define _VIDEO_H

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
typedef struct
{
    const char *filename, *out;
    AVFormatContext *fmt_ctx;
    const AVCodec *codec;
    AVCodecContext *codec_ctx;
    AVStream *video_stream;
    AVStream *audio_stream;
    int video_stream_index;
    int audio_stream_index;
    AVPacket *pkt;
    AVFrame *frame;
} video;

/**
 * Initialize new video
 */
video *video_new(char *filename);

/**
 * Find the stream index of the video.
 */

void video_set_streams(video *vid);

/**
 * Find the decoder of the video
 */
void video_find_decoder(video *vid);

/**
 * Decoding video frames into struct frames (frames.h)
 */

void video_decode_frames(video *vid);

/**
 * Clean up the video from the memory
 */
void video_clean_up(video *vid);

#endif
