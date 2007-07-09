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
/*
    char codecId[5];
    char codecType[5];
    char codecBitrate[11];
    char codecPxmFmt[5];
    char codecWidth[5];
    char codecHeight[5];
    char codecHasBFrames[2];
    char codecExtraDataSize[5];
*/

    int codecId=0;
    int codecType=1;
    int hasBFrames=0;
    unsigned char  buffer[36];
    memset(buffer,0,sizeof(buffer));
    read(buffer, 35);
    cout << "Buffer"<<buffer<<endl;
    sscanf((char*)buffer,"%04d%04d", &codecId, &codecType);
    cout << "CodecId"<<codecId<<endl;
    cout << "CodecType"<<codecType<<endl;
    _codec=new Codec((CodecID)codecId);
    AVCodecContext * c=_codec->getCodecContext();
    sscanf((char*)buffer,"%04d%04d%010d%04d%04d%04d%01d%04d", &c->codec_id, &c->codec_type, &c->bit_rate,&c->pix_fmt,&c->width, &c->height, &hasBFrames, &c->extradata_size);
    uint8_t * extradata=new uint8_t[c->extradata_size];
    read(extradata, c->extradata_size);
    c->extradata=extradata;
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