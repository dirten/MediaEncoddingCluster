#include "CodecOutputStream.h"

using namespace org::esb::av;

CodecOutputStream::CodecOutputStream(OutputStream * os){
    _os=os;
}

void CodecOutputStream::writeCodec(Codec * codec){
    AVCodecContext * ctx=codec;
    write((char*)&ctx->codec_id,sizeof(CodecID));
    write((char*)&ctx->codec_type,sizeof(CodecType));
    write((char*)&ctx->bit_rate,sizeof(int));
    write((char*)&ctx->pix_fmt,sizeof(PixelFormat));
    write((char*)&ctx->width,sizeof(int));
    write((char*)&ctx->height,sizeof(int));
    write((char*)&ctx->has_b_frames,sizeof(int));
    write((char*)&ctx->extradata_size,sizeof(int));
    write((char*)ctx->extradata,ctx->extradata_size);
}

void CodecOutputStream::write(char * buffer, int length){
    _os->write(buffer, length);
}

void CodecOutputStream::write(vector<unsigned char>&buffer){
    _os->write(buffer);
}

void CodecOutputStream::flush(){
    _os->flush();
}

