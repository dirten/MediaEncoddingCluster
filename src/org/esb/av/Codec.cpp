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
        }
    }
}


