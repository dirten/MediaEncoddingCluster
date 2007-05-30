#include "AVInputStream.h"
#include "Codec.h"
#include "Frame.h"
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

            long AVInputStream::getDuration(){
                return _avStream->duration;
            }

            float AVInputStream::getTimeBase(){
                return av_q2d(_avStream->time_base);
            }

            long AVInputStream::getNumberFrames(){
                return _avStream->nb_frames;
            }
            Frame * AVInputStream::getNextFrame(){
//        	return *NULL;
            }
            Frame * AVInputStream::getFrame(int frameIdx){
//        	return NULL;
            }
        }
    }
}

