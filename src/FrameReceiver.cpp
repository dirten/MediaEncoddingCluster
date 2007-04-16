#include "FrameReceiver.h"


//#define STREAM_DURATION   5.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
//#define STREAM_NB_FRAMES  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
#define STREAM_PIX_FMT PIX_FMT_YUV420P /* default pix_fmt */

AVFrame *picture, *tmp_picture;
using namespace org::esb::socket;

static AVFrame *alloc_picture(int pix_fmt, int width, int height)
{
    AVFrame *picture;
    uint8_t *picture_buf;
    int size;

    picture = avcodec_alloc_frame();
    if (!picture){
	printf("Fehler bei alloc_picture");
//	cout << "Fehler bei alloc_picture"<<endl;
        return NULL;
    }
    size = avpicture_get_size(pix_fmt, width, height);
    picture_buf = (uint8_t*)malloc(size);
    if (!picture_buf) {
	printf("Fehler bei alloc_picture");
        av_free(picture);
        return NULL;
    }
    avpicture_fill((AVPicture *)picture, picture_buf,
                   pix_fmt, width, height);
    return picture;
}

int main(int argc, char *argv[]){
    char * filename="test.avi";    
    av_register_all();
    picture=alloc_picture(PIX_FMT_RGB24, 512, 256);
    AVFormatContext * oc = av_alloc_format_context();
    AVOutputFormat * fmt=guess_format(NULL,filename,NULL);
    oc->oformat = fmt;
    
    AVStream * st = av_new_stream(oc, 0);

    AVCodecContext * c = st->codec;
    c->codec_id = fmt->video_codec;
    c->codec_type = CODEC_TYPE_VIDEO;

    /* put sample parameters */
    c->bit_rate = 4000000;
    /* resolution must be a multiple of two */
    c->width = 512;
    c->height = 256;
    /* time base: this is the fundamental unit of time (in seconds) in terms
       of which frame timestamps are represented. for fixed-fps content,
       timebase should be 1/framerate and timestamp increments should be
       identically 1. */
    c->time_base.den = STREAM_FRAME_RATE;
    c->time_base.num = 1;
    c->gop_size = 1; /* emit one intra frame every twelve frames at most */
    c->pix_fmt = STREAM_PIX_FMT;
    if (c->codec_id == CODEC_ID_MPEG2VIDEO) {
        /* just for testing, we also add B frames */
        c->max_b_frames = 2;
    }
    if (c->codec_id == CODEC_ID_MPEG1VIDEO){
        /* needed to avoid using macroblocks in which some coeffs overflow
           this doesnt happen with normal video, it just happens here as the
           motion of the chroma plane doesnt match the luma plane */
        c->mb_decision=2;
    }
    // some formats want stream headers to be seperate
    if(!strcmp(oc->oformat->name, "mp4") || !strcmp(oc->oformat->name, "mov") || !strcmp(oc->oformat->name, "3gp"))
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;

    if (av_set_parameters(oc, NULL) < 0) {
        fprintf(stderr, "Invalid output format parameters\n");
        exit(1);
    }

    dump_format(oc, 0, filename, 1);

    /* find the video encoder */
    AVCodec * codec = avcodec_find_encoder(c->codec_id);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    /* open the codec */
    if (avcodec_open(c, codec) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    

    /* allocate the encoded raw picture */
    picture = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!picture) {
        fprintf(stderr, "Could not allocate picture\n");
        exit(1);
    }
    /* open the output file, if needed */
    if (!(fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&oc->pb, filename, URL_WRONLY) < 0) {
            fprintf(stderr, "Could not open '%s'\n", filename);
            exit(1);
        }
    }
    av_write_header(oc);
    tmp_picture = NULL;
//    if (c->pix_fmt != PIX_FMT_YUV420P) {
        tmp_picture = alloc_picture(PIX_FMT_RGB24, c->width, c->height);
        if (!tmp_picture) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
//    }

    Socket *so1=new Socket();
//    cout << "Hostname"<<so1->getHostname()<<endl;
//    so1->setHostname("localhost");
    so1->setPort(10000);
    so1->Connect();
    SocketData * r=so1->read();

    memcpy(tmp_picture->data[0],r->data,r->data_length);

    img_convert((AVPicture *)picture, c->pix_fmt,
                (AVPicture *)tmp_picture, PIX_FMT_RGB24,
                c->width, c->height);


        /* encode the image */
        uint8_t *video_outbuf;
        int video_outbuf_size, ret;
	video_outbuf = NULL;
	if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
        /* allocate output buffer */
        /* XXX: API change will be done */
    	    video_outbuf_size = 200000;
    	    video_outbuf = (uint8_t*)malloc(video_outbuf_size);
	}

        int out_size = avcodec_encode_video(c, video_outbuf, video_outbuf_size, picture);
	
        /* if zero size, it means the image was buffered */
	
        if (out_size > 0) {
            AVPacket pkt;
            av_init_packet(&pkt);

            pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
            if(c->coded_frame->key_frame)
                pkt.flags |= PKT_FLAG_KEY;
            pkt.stream_index= st->index;
            pkt.data= video_outbuf;
            pkt.size= out_size;

            ret = av_write_frame(oc, &pkt);
        } else {
            ret = 0;
        }
    avcodec_close(st->codec);
//    av_free(picture->data[0]);
    av_free(picture);
    if (tmp_picture) {
        av_free(tmp_picture->data[0]);
        av_free(tmp_picture);
    }
    av_free(video_outbuf);
    av_write_trailer(oc);

    /* free the streams */
    /*
    for(int i = 0; i < oc->nb_streams; i++) {
        av_freep(&oc->streams[i]->codec);
        av_freep(&oc->streams[i]);
    }
*/
    if (!(fmt->flags & AVFMT_NOFILE)) {
        /* close the output file */
        url_fclose(&oc->pb);
    }

    /* free the stream */
    av_free(oc);

}
