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
            Codec::Codec(const CodecID codecId, int mode){
        	cout << "CodecConstructor"<<endl;
        	codec_id=codecId;
//        	_codec=NULL;
		if(mode == DECODER){
		    _codec=avcodec_find_decoder(codec_id);
		}else{
		    _codec=avcodec_find_encoder(codec_id);
		}
		if(_codec==NULL)
		    cout << "Codec not found for id :"<<codecId<<endl;
		avcodec_get_context_defaults2(this, _codec->type);
//		codec->capabilities=_codec->capabilities;
            }

/*            Codec::Codec(AVCodecContext * context){
//		codec=context->codec;//avcodec_find_decoder(context->codec_id);
		flags=context->flags;
		codec_id=context->codec_id;
		codec_type=context->codec_type;
		width=context->width;
		height=context->height;
		bit_rate=context->bit_rate;
		time_base=context->time_base;
		pix_fmt=context->pix_fmt;
		has_b_frames=context->has_b_frames;
		extradata_size=context->extradata_size;
		extradata=context->extradata;
//		open();
            }
*/            
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

            void Codec::initDefaults(){
		avcodec_get_context_defaults(this/*, CODEC_TYPE_UNKNOWN*/);
            }
            
	    void Codec::findCodec(int mode){
	    	if(mode == DECODER){
		    _codec=avcodec_find_decoder(codec_id);
		    if(_codec==NULL)
			cout << "Decoder not found for id :"<<_codec_id<<endl;
		}else{
		    _codec=avcodec_find_encoder(codec_id);
		    if(_codec==NULL)
			cout << "Encoder not found for id :"<<_codec_id<<endl;
		}
	    }
	    
            void Codec::open(int mode){
        	findCodec(mode);
        	if(_codec->capabilities & CODEC_CAP_TRUNCATED)
        	    flags|=CODEC_FLAG_TRUNCATED;
		avcodec_open(this, _codec);
            }

            Packet* Codec::encodeFrame(Frame & frame){
		Packet * pac=new Packet();
		int buffer_size=1024*256;
		uint8_t * buffer=NULL;
		int ret=avcodec_encode_video(this,buffer, buffer_size, &frame);
		
		pac->data=buffer;
		pac->size=ret;
		return pac;
            }
            
            Frame * Codec::decode(Packet & packet){
		Frame * frame=new Frame();
                assert(&packet);
		assert(_codec);
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


