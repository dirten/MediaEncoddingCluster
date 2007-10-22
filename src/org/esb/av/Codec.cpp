#include "Codec.h"
extern "C" {
#include "avformat.h"
}
#include "Frame.h"
#include <iostream>
#include <assert.h>
using namespace std;
namespace org{
    namespace esb{
        namespace av{
            Codec::Codec(const CodecID codecId){
				_codec=avcodec_find_decoder(codecId);
				if(!_codec)
		    		cout << "Codec not found for id :"<<codecId<<endl;
				 avcodec_get_context_defaults(this/*, CODEC_TYPE_UNKNOWN*/);
            }

            Codec::Codec(AVCodecContext * context){
		_codec=avcodec_find_decoder(context->codec_id);
		codec_id=context->codec_id;
		codec_type=context->codec_type;
		width=context->width;
		height=context->height;
		bit_rate=context->bit_rate;
		pix_fmt=context->pix_fmt;
		has_b_frames=context->has_b_frames;
		extradata_size=context->extradata_size;
		extradata=context->extradata;
		open();
            }
            
            CodecType Codec::getCodecType(){
                return _codecCtx->codec_type;
            }
            
            char * Codec::getCodecName(){
                return _codecCtx->codec_name;
            }

            int Codec::getCodecId(){
                return _codecCtx->codec_id;
            }

            AVCodecContext * Codec::getCodecContext(){
                return _codecCtx;
            }

            void Codec::open(){
				avcodec_open(this, _codec);
            }

            Packet* Codec::encodeFrame(Frame & frame){
            
            }
            Frame * Codec::decode(Packet & packet){
		Frame * frame=new Frame();
                assert(&packet);
		assert(codec);
		int _frameFinished=0;
    	        frame->_width=this->width;
	        frame->_height=this->height;
	        frame->_pixFormat=this->pix_fmt;
	        int bytesRemaining=packet.size,  bytesDecoded=0;
	        uint8_t * rawData=packet.data;
	        while(bytesRemaining > 0)
	        {
	          bytesDecoded=avcodec_decode_video(this, frame, &_frameFinished, rawData, bytesRemaining);
	          if(bytesDecoded < 0)
	          {
	            fprintf(stderr, "Error while decoding frame\n");
	            break;
	          }
	    
	          bytesRemaining-=bytesDecoded;
	          rawData+=bytesDecoded;
	          if(_frameFinished){
		    break;
	    	  }
		}
		return frame;
            }
        }
    }
}


