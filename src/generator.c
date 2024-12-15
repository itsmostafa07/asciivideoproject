#include <string.h>

#include "generator.h"
#include "video.h"
#include "frame.h"
#include "pixel.h"
#include "helpers.h"
#include "generator.h"

int decode_handler(AVCodecContext *dec_ctx, AVFrame *av_frame, AVPacket *pkt, const char *filename)
{
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0)
    {
        ERROR("Error sending a packet for decoding, ret={%i}", ret);
        return -1;
    }

    while (ret >= 0)
    {
        ret = avcodec_receive_frame(dec_ctx, av_frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            goto done;
        else if (ret < 0)
        {
            ERROR("Error during decoding");
            return -1;
        }

        int frame_n = dec_ctx->frame_num;
        int w = av_frame->width, h = av_frame->height;
        snprintf(buf, sizeof(buf), "%s%ld.ascii", filename, frame_n);

        INFO("Saving frame #%ld: path={\"%s\"}, res={%ix%i}", frame_n, buf, av_frame->width, av_frame->height);

        frame *fm = frame_new(frame_n, av_frame->data[0], av_frame->linesize[0], w, h);

        if (!fm)
        {
            ERROR("Creating frame#%i was failed", dec_ctx->frame_num);
        }

        int writing_status = frame_ascii_write_to(fm, buf);
        if (writing_status == -1)
        {
            ERROR("Couldn't write frame <%i> into %s", dec_ctx->frame_num, buf);
            return -1;
        }
        frame_free(fm);
    }

done:
    return 0;
}

/**
 * Creating a new video generator struct.
 */
ascii_video_gen *ascii_viden_gen_new(const char *src, const char *dest, video *vid)
{
    ascii_video_gen *gen = (ascii_video_gen *)malloc(sizeof(ascii_video_gen));

    gen->src = src;
    gen->dest = dest;
    gen->vid = vid;
    return gen;
}

/**
 * Running the generator.
 */
int ascii_video_gen_run(ascii_video_gen *gen)
{
    video_decode_frames(gen->vid, decode_handler);

    // Flush the decoder
    decode_handler(gen->vid->codec_ctx, gen->vid->frame, NULL, gen->dest);
    return 0;
}
