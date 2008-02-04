#include "AVInputStream.h"

using namespace org::esb::av;

Codec * AVInputStream::getCodec(){
    return (Codec*)codec;
}

void AVInputStream::setFormatContext(AVFormatContext * ctx){_formatContext=ctx;}