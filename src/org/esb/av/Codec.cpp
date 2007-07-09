#include "Codec.h"
extern "C" {
#include "avformat.h"
}
#include <iostream>
using namespace std;
namespace org{
    namespace esb{
        namespace av{
            Codec::Codec(const CodecID codecId){
		_codec=avcodec_find_decoder(codecId);
		if(!_codec)
		    cout << "Codec not found"<<endl;
		_codecCtx=avcodec_alloc_context();
//		_codecCtx->width=512;
//		_codecCtx->height=256;
		avcodec_open(_codecCtx, _codec);
		
            }

            Codec::Codec(AVCodecContext * context){
                _codecCtx=context;
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
/*
            void Codec::decodeFrame(Frame * frame){
            
            }
            void Codec::encodeFrame(Frame * frame){
            
            }
*/
        }
    }
}


