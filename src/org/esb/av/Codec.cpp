#include "Codec.h"
#include "avformat.h"


namespace org{
    namespace esb{
        namespace av{
            Codec::Codec(const char * name){

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

            void Codec::decodeFrame(Frame * frame){
            
            }
            void Codec::encodeFrame(Frame * frame){
            
            }
        }
    }
}


