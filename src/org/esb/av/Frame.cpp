#include "Frame.h"
#include "avformat.h"
#include <assert.h>
using namespace org::esb::av;
Frame::Frame(AVPacket * packet, AVCodecContext * codecContext){
    assert(codecContext);
    _packet=packet;
    _frame=avcodec_alloc_frame();

    int bytesRemaining=_packet->size, frameFinished=0, bytesDecoded=0;
    uint8_t * rawData=packet->data;
    
    while(bytesRemaining > 0)
    {
//     cout << "Laenge : "<< strlen((const char *) rawData) << endl;

            // Decode the next chunk of data
      
      bytesDecoded=avcodec_decode_video(codecContext, _frame,
                                        &frameFinished, rawData, bytesRemaining);
     
	fprintf(stderr, "%d Bytes decoded\n", bytesDecoded);

            // Was there an error?
    
      if(bytesDecoded < 0)
      {
        fprintf(stderr, "Error while decoding frame\n");
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
