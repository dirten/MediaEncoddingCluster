#include "FormatOutputStream.h"


using namespace org::esb::av;
using namespace org::esb::io;

//extern "C" void avformat_get_context_defaults(AVFormatContext * s);
//extern "C" const AVClass av_format_context_class;


FormatOutputStream::FormatOutputStream(File * target_file){
    _fmtCtx=av_alloc_format_context();

    AVOutputFormat * fmt=guess_format(NULL,target_file->getPath(),NULL);
    _fmtCtx->oformat = fmt;

//    AVStream * st = av_new_stream(_fmtCtx, 0);

    if (!(fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&_fmtCtx->pb,target_file->getPath(), URL_WRONLY) < 0) {
            fprintf(stderr, "Could not open '%s'\n", target_file->getPath());
//            exit(1);
        }
    }
//    av_write_header(_fmtCtx);
}

FormatOutputStream::~FormatOutputStream(){
    close();
}

void FormatOutputStream::addPacketStream(PacketOutputStream & pos, Encoder & encoder){


}

void FormatOutputStream::write(char * buffer, int length){};
void FormatOutputStream::write(vector<unsigned char>&buffer){};
void FormatOutputStream::write(char * buffer){};
void FormatOutputStream::close(){
    av_write_trailer(_fmtCtx);
};
