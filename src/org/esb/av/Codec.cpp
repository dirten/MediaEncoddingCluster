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
        	cout << "CodecConstructor:"<<codecId<<endl;
        	_codec_id=codecId;
        	_codec=NULL;
		if(mode == DECODER){
		    _codec=avcodec_find_decoder(_codec_id);
		}else{
		    _codec=avcodec_find_encoder(_codec_id);
		}
		if(_codec==NULL)
		    cout << "Codec not found for id :"<<codecId<<endl;
		avcodec_get_context_defaults2(this, _codec->type);
//		codec->capabilities=_codec->capabilities;
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

            void Codec::initDefaults(){
		avcodec_get_context_defaults(this/*, CODEC_TYPE_UNKNOWN*/);
            }
            
	    void Codec::findCodec(int mode){
	    	if(mode == DECODER){
		    _codec=avcodec_find_decoder(_codec_id);
		    if(_codec==NULL)
			cout << "Decoder not found for id :"<<_codec_id<<endl;
		}else{
		    _codec=avcodec_find_encoder(_codec_id);
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
	    Codec::~Codec(){
		avcodec_close(this);
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
        }
    }
}


