#include "Frame.h"
#include "avformat.h"
#include <assert.h>
#include <iostream>
using namespace std;
using namespace org::esb::av;
Frame::Frame(AVPacket * packet, AVCodecContext * codecContext){
    assert(packet);
    assert(codecContext);
    _packet=packet;
//    _frame=avcodec_alloc_frame();
    _frame=new AVFrame();//avcodec_alloc_frame();


/*
    _codecContext=avcodec_alloc_context();
    AVCodec * codec=avcodec_find_decoder(codecContext->codec_id);

    if (avcodec_open(_codecContext, codec) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    */
    _codecContext=codecContext;
    
    int bytesRemaining=_packet->size, frameFinished=0, bytesDecoded=0;
//    cout << "PacketSize:"<<bytesRemaining<<endl;
    uint8_t * rawData=packet->data;
    
    while(bytesRemaining > 0)
    {
//     cout << "Laenge : "<< strlen((const char *) rawData) << endl;

            // Decode the next chunk of data
      
      bytesDecoded=avcodec_decode_video(_codecContext, _frame,
                                        &frameFinished, rawData, bytesRemaining);
     
    /*
	fprintf(stderr, "%d Bytes decoded\n", bytesDecoded);
	fprintf(stderr, "frameWidth %d\n", _frame->linesize[0]);
	fprintf(stderr, "frameHeight %d\n", _frame->linesize[1]);
    */
            // Was there an error?
    
      if(bytesDecoded < 0)
      {
        fprintf(stderr, "Error while decoding frame\n");
        break;
//        return true;
      }

      bytesRemaining-=bytesDecoded;
      rawData+=bytesDecoded;

            // Did we finish the current frame? Then we can return
      if(frameFinished){
	break;
//        return true;
      }
    }    
}

Frame::~Frame(){
//    av_free_packet(_packet);
//    av_free(_frame);
    if(_frame)
        delete _frame;
//    delete _packet;
}

AVPacket * Frame::getPacket(){
    return _packet;

}

uint8_t * Frame::getData(){
    return _packet->data;

}

int Frame::getSize(){
    return _packet->size;
}

AVFrame * Frame::getFrame(){
    return _frame;
}

int Frame::getHeight(){
    return _codecContext->height;
}

int Frame::getWidth(){
    return _codecContext->width;
}

void Frame::setFrame(AVFrame * frame){
//    if(_frame!=NULL)
//        av_free(_frame);
    _frame=frame;

}
