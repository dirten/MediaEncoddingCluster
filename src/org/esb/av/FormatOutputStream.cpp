#include "FormatOutputStream.h"
#include "AV.h"

using namespace org::esb::av;
using namespace org::esb::io;

//extern "C" void avformat_get_context_defaults(AVFormatContext * s);
//extern "C" const AVClass av_format_context_class;

FormatOutputStream::FormatOutputStream(File * target_file) {


    AVFormatParameters params, *ap = &params;
    memset(ap, 0, sizeof (*ap));


    _file = target_file;
    _fmtCtx = av_alloc_format_context();
    _fmt = guess_format(NULL, target_file->getPath().c_str(), NULL);
    _fmtCtx->oformat = _fmt;
    //    _fmtCtx->timestamp=0;

    //    AVStream * st = av_new_stream(_fmtCtx, 0);

    if (!(_fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&_fmtCtx->pb, _file->getPath().c_str(), URL_WRONLY) < 0) {
            fprintf(stderr, "Could not open '%s'\n", _file->getPath().c_str());
            //            exit(1);
        }
    }

    if (av_set_parameters(_fmtCtx, ap) < 0) {
        fprintf(stderr, "Invalid output format parameters\n");
        exit(1);
    }

    //    av_write_header(_fmtCtx);
}

FormatOutputStream::~FormatOutputStream() {
    close();
}

void FormatOutputStream::addPacketStream(PacketOutputStream & pos, Encoder & encoder) {


}

void FormatOutputStream::write(char * buffer, int length) {
};

void FormatOutputStream::write(vector<unsigned char>&buffer) {
};

void FormatOutputStream::write(char * buffer) {
};

void FormatOutputStream::open() {
    if (!(_fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&_fmtCtx->pb, _file->getPath().c_str(), URL_WRONLY) < 0) {
            fprintf(stderr, "Could not open '%s'\n", _file->getPath().c_str());
            //            exit(1);
        }
    }
}

void FormatOutputStream::close() {
    av_write_trailer(_fmtCtx);
    url_fclose(_fmtCtx->pb);
    av_free(_fmtCtx);
    //	av_close_input_file(_fmtCtx);
};
