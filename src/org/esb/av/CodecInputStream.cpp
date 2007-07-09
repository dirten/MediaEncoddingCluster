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
    unsigned char  buffer[40];
    memset(buffer,0,sizeof(buffer));


//    read((unsigned char*)&codecId, sizeof(int));
//    cout << "CodecID Binary:"<<codecId<<endl;

    read(buffer, 45);
    cout << "Buffer"<<buffer<<endl;
    sscanf((char*)buffer,"%04d%04d", &codecId, &codecType);
    cout << "CodecId"<<codecId<<endl;
    cout << "CodecType"<<codecType<<endl;
    _codec=new Codec((CodecID)codecId);
    _codec->open();
    AVCodecContext * c=_codec->getCodecContext();
    
    sscanf((char*)buffer,"%04d%04d%010d%04d%04d%04d%01d%04d", &c->codec_id, &c->codec_type, &c->bit_rate,&c->pix_fmt,&c->width, &c->height, &hasBFrames, &c->extradata_size/*,&c->codec->priv_data_size*/);
	if(c->extradata_size>0){
    	uint8_t * extradata=new uint8_t[c->extradata_size];
    	read(extradata, c->extradata_size);
    	c->extradata=extradata;
    }

    if(c->codec->priv_data_size>0){
//	c->priv_data=malloc(c->codec->priv_data_size);
//    	uint8_t * privdata=new uint8_t[c->codec->priv_data_size];
//    	read((unsigned char *)c->priv_data, c->codec->priv_data_size);
//    	c->priv_data=privdata;
	cout << "setting priv_data"<<endl;
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