#include "CodecInputStream.h"
#include <iostream>
using namespace std;
using namespace org::esb::av;

CodecInputStream::CodecInputStream(InputStream * is){
//    _codec=new Codec((CodecID)13);
    _is=is;
}

CodecInputStream::~CodecInputStream(){
    delete _codec;
}

Codec * CodecInputStream::readCodec(){
    CodecID codecId;
    read((unsigned char*)&codecId,sizeof(CodecID));
    _codec=new Codec(codecId);
    AVCodecContext * ctx=_codec;
    read((unsigned char*)&ctx->codec_type,sizeof(CodecType));
    read((unsigned char*)&ctx->bit_rate,sizeof(int));
    read((unsigned char*)&ctx->pix_fmt,sizeof(PixelFormat));
    read((unsigned char*)&ctx->width,sizeof(int));
    read((unsigned char*)&ctx->height,sizeof(int));
    read((unsigned char*)&ctx->has_b_frames,sizeof(int));
    
    read((unsigned char*)&ctx->extradata_size,sizeof(int));
	if(ctx->extradata_size>0){
		uint8_t * extradata=new uint8_t[ctx->extradata_size];
		ctx->extradata=extradata;
    	read((unsigned char*)ctx->extradata,ctx->extradata_size);
    }
    return _codec;
}

int CodecInputStream::read(){
    return _is->read();
}

int CodecInputStream::read(vector<unsigned char >&buffer){
    return _is->read(buffer);
}
int CodecInputStream::read(unsigned char* buffer, int length){
    return _is->read(buffer, length);
}
int CodecInputStream::available(bool isBlocking){
    return _is->available(isBlocking);
}

