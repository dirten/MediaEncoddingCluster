#include "Codec.h"
extern "C" {
#include "avformat.h"
}
#include "Frame.h"
#include <iostream>
#include <assert.h>
using namespace std;
void __attribute__ ((constructor)) my_init(void);
//void _init() __attribute__((constructor));

void my_init(){
    cout << "Init AV Package Codec"<<endl;
    av_register_all();
    avcodec_register_all();
}
namespace org{
    namespace esb{
        namespace av{
    	    Codec::Codec(){}
            Codec::Codec(const CodecID codecId, int mode){
        		cout << "CodecConstructor:"<<codecId<<endl;
        		_codec_id=codecId;
        		_codec=NULL;
        		_mode=mode;
				if(_mode == DECODER){
		    		_codec=avcodec_find_decoder(_codec_id);
				}else{
		    		_codec=avcodec_find_encoder(_codec_id);
				}
				if(_codec==NULL)
		    		cout << "Codec not found for id :"<<codecId<<endl;
		    	if(_codec->type==CODEC_TYPE_VIDEO){
		    		cout << "CodecTypeVideo"<<endl;
		    	}
		    	if(_codec->type==CODEC_TYPE_AUDIO){
		    		cout << "CodecTypeAudio"<<endl;
		    	}
				avcodec_get_context_defaults2(this, _codec->type);
				_width=0;
				_height=0;
				_bit_rate=0;
				_gop_size=0;
				_channels=0;
				_sample_rate=0;
            }

            CodecType Codec::getCodecType(){
//                return _codecCtx->codec_type;
            }
            
            char * Codec::getCodecName(){
//                return _codecCtx->codec_name;
            }

            int Codec::getCodecId(){
//                return _codecCtx->codec_id;
            }

            AVCodecContext * Codec::getCodecContext(){
//                return _codecCtx;
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
				avcodec_get_context_defaults2(this, _codec->type);
				pix_fmt=_pix_fmt;
				width=_width;
				height=_height;
				bit_rate=_bit_rate;
				time_base=_time_base;
				gop_size=_gop_size;
				sample_rate=_sample_rate;
				sample_fmt=_sample_format;
				channels=_channels;
	    	}
	    
        void Codec::open(){
        	findCodec(_mode);
        	if(_codec->capabilities & CODEC_CAP_TRUNCATED)
        	    flags|=CODEC_FLAG_TRUNCATED;
		if(avcodec_open(this, _codec)<0){
		    cout << "ERROR : while openning Codec"<<endl;
		}
		    cout << "openning Codec"<<_codec_id<<endl;

		    	if(codec_type==CODEC_TYPE_VIDEO){
		    		cout << "CodecTypeVideo"<<endl;
		    	}
		    	if(codec_type==CODEC_TYPE_AUDIO){
		    		cout << "CodecTypeAudio"<<endl;
		    	}

        }
	    Codec::~Codec(){
			avcodec_close(this);
	    }
            void Codec::setWidth(int w){_width=w;}
            void Codec::setHeight(int h){_height=h;}
            void Codec::setPixelFormat(PixelFormat pf){_pix_fmt=pf;}
            void Codec::setBitRate(int rate){_bit_rate=rate;}
            void Codec::setTimeBase(AVRational tb){_time_base=tb;}
            void Codec::setGopSize(int size){_gop_size=size;}
            void Codec::setChannels(int c){_channels=c;}
            void Codec::setSampleRate(int rate){_sample_rate=rate;}
            void Codec::setSampleFormat(SampleFormat f){_sample_format=f;}

        }
    }
}


