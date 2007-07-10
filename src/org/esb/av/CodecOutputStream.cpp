#include "CodecOutputStream.h"

using namespace org::esb::av;

CodecOutputStream::CodecOutputStream(OutputStream * os){
    _os=os;
}

void CodecOutputStream::writeCodec(Codec * codec){
	/*
    sprintf(codecId,"%04d", ctx->codec_id);
    sprintf(codecType,"%04d", ctx->codec_type);
    sprintf(codecBitrate,"%010d", ctx->bit_rate);
    sprintf(codecPxmFmt,"%04d", ctx->pix_fmt);
    sprintf(codecWidth,"%04d", ctx->width);
    sprintf(codecHeight,"%04d", ctx->height);
    sprintf(codecHasBFrames,"%01d", ctx->has_b_frames);
    sprintf(codecExtraDataSize,"%04d", ctx->extradata_size);
    sprintf(codecPrivDataSize,"%04d", ctx->codec->priv_data_size);
*/
    AVCodecContext * ctx=codec->getCodecContext();
    cout << "CodecIdSent:"<<ctx->codec_id<<endl;





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

void CodecOutputStream::writeCodec2(Codec * codec){
    AVCodecContext * ctx=codec->getCodecContext();
    char codecId[5];
    char codecType[5];
    char codecBitrate[11];
    char codecPxmFmt[5];
    char codecWidth[5];
    char codecHeight[5];
    char codecHasBFrames[2];
    char codecExtraDataSize[5];
    char codecPrivDataSize[5];
    
    memset(codecId,0,5);
    memset(codecType,0,5);
    memset(codecBitrate,0,11);
    memset(codecPxmFmt,0,5);
    memset(codecWidth,0,5);
    memset(codecHeight,0,5);
    memset(codecHasBFrames,0,2);
    memset(codecExtraDataSize,0,5);
    memset(codecPrivDataSize,0,5);
//    memset(codecExtraData,0,sizeof(codecExtraData));

    sprintf(codecId,"%04d", ctx->codec_id);
    sprintf(codecType,"%04d", ctx->codec_type);
    sprintf(codecBitrate,"%010d", ctx->bit_rate);
    sprintf(codecPxmFmt,"%04d", ctx->pix_fmt);
    sprintf(codecWidth,"%04d", ctx->width);
    sprintf(codecHeight,"%04d", ctx->height);
    sprintf(codecHasBFrames,"%01d", ctx->has_b_frames);
    sprintf(codecExtraDataSize,"%04d", ctx->extradata_size);
    sprintf(codecPrivDataSize,"%04d", ctx->codec->priv_data_size);

//    write((char*)&ctx->codec_id,sizeof(int));
    write((char*)codecId,4);
    write((char*)codecType,4);
    write((char*)codecBitrate,10);
    write((char*)codecPxmFmt,4);
    write((char*)codecWidth,4);
    write((char*)codecHeight,4);
    write((char*)codecHasBFrames,1);
    write((char*)codecExtraDataSize,4);
    write((char*)codecPrivDataSize,4);
    write((char*)ctx->extradata,ctx->extradata_size);
    write((char*)ctx->priv_data,ctx->codec->priv_data_size);
//	cout <<"TypeId"<<typeid(ctx).name()<<endl;
}

void CodecOutputStream::write(char * buffer, int length){
    _os->write(buffer, length);
}

void CodecOutputStream::write(vector<unsigned char>&buffer){
    _os->write(buffer);
}

