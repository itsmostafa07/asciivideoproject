#include <string.h>
#include "./video.h"
#include "helpers.h"
/**
 * Initialize new video
 */
video *video_new(char *filename)
{
    video *vid = malloc(sizeof(video));

    strcpy(vid->filename, filename);
    const char *filename, *outfilename;
    vid->fmt_ctx = NULL;
    const AVCodec *codec;
    vid->codec_ctx = NULL;
    vid->video_stream = NULL;
    vid->audio_stream = NULL;
    vid->video_stream_index = -1;
    vid->audio_stream_index = -1;
    vid->pkt = NULL;
    vid->frame = NULL;

    if (avformat_open_input(&vid->fmt_ctx, filename, NULL, NULL) < 0)
    {
        eprintf("Could not open input file\n");
        exit(1);
    }

    if (avformat_find_stream_info(vid->fmt_ctx, NULL) < 0)
    {
        eprintf("Could not find stream information\n");
        exit(1);
    }

    video_set_streams(vid);
    return vid;
}

/**
 * Find the stream index of the video.
 */

void video_set_streams(video *vid)
{
    // Find the video stream
    for (unsigned int i = 0; i < vid->fmt_ctx->nb_streams; i++)
    {
        if (vid->fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            vid->video_stream_index = i;
            vid->video_stream = vid->fmt_ctx->streams[i];
            break;
        }
        if (vid->fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            vid->audio_stream_index = i;
            vid->audio_stream = vid->fmt_ctx->streams[i];
            break;
        }
    }
}

/**
 * Find the decoder of the video
 */
void video_find_decoder(video *vid)
{
    // Find the decoder for the video stream
    vid->codec = avcodec_find_decoder(vid->video_stream->codecpar->codec_id);
    if (!vid->codec)
    {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    // Allocate codec context
    vid->codec_ctx = avcodec_alloc_context3(vid->codec);
    if (!vid->codec_ctx)
    {
        fprintf(stderr, "Could not allocate codec context\n");
        exit(1);
    }

    // Copy codec parameters from the input stream to the codec context
    if (avcodec_parameters_to_context(vid->codec_ctx, vid->video_stream->codecpar) < 0)
    {
        fprintf(stderr, "Could not copy codec parameters to context\n");
        exit(1);
    }
}

/**
 * Decoding video frames into struct frames (frames.h)
 */

void video_decode_frames(video *vid)
{
    // Allocate frame and packet
    vid->frame = av_frame_alloc();
    vid->pkt = av_packet_alloc();
    if (!vid->frame || !vid->pkt)
    {
        fprintf(stderr, "Could not allocate frame or packet\n");
        exit(1);
    }

    // Read frames from the file
    while (av_read_frame(vid->fmt_ctx, vid->pkt) >= 0)
    {
        if (vid->pkt->stream_index == vid->video_stream_index)
        {
            decode(vid->codec_ctx, vid->frame, vid->pkt, vid->out);
        }
        av_packet_unref(vid->pkt);
    }
}

/**
 * Clean up the video from the memory
 */
void video_clean_up(video *vid)
{
    av_frame_free(&vid->frame);
    av_packet_free(&vid->pkt);
    avcodec_free_context(&vid->codec_ctx);
    avformat_close_input(&vid->fmt_ctx);
}