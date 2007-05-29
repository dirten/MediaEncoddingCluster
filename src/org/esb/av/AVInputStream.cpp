#include "AVInputStream.h"
#include "Codec.h"
#include <assert.h>
#include <iostream>
using namespace std;
namespace org{
    namespace esb{
        namespace av{
            AVInputStream::AVInputStream(AVStream * stream){
                assert(stream!=NULL);
                _avStream=stream;
            }
            Codec * AVInputStream::getCodec(){
                return new Codec(_avStream->codec);
            }
        }
    }
}

