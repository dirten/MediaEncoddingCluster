#include "AVInputStream.h"
#include "Codec.h"
#include "Frame.h"
#include <assert.h>
#include <iostream>
using namespace std;
namespace org {
    namespace esb {
        namespace av {
            AVInputStream::AVInputStream(AVFormatContext * context, int streamIndex) {
                assert(context!=NULL);
                _formatContext=context;
                _streamIndex=streamIndex;
		_codecContext=_formatContext->streams[_streamIndex]->codec;
		_codec=avcodec_find_decoder(_codecContext->codec_id);
		if(_codec->capabilities & CODEC_CAP_TRUNCATED){
		    _codecContext->flags|=CODEC_FLAG_TRUNCATED;
		}
		if(avcodec_open(_codecContext, _codec)<0){
		    fprintf(stderr, "avcodec_open failed\n");
		}
            }

            Codec * AVInputStream::getCodec() {
                return new Codec(_avStream->codec);
            }

            long AVInputStream::getDuration() {
                return _avStream->duration;
            }

            float AVInputStream::getTimeBase() {
                return av_q2d(_avStream->time_base);
            }

            long AVInputStream::getNumberFrames() {
                return _avStream->nb_frames;
            }

            Frame * AVInputStream::getNextFrame() {
                AVPacket packet;
		        packet.data=NULL;
                do {
                    if(packet.data!=NULL)
                        av_free_packet(&packet);
                    if(av_read_packet(_formatContext, &packet)<0)
                        return NULL;
                } while(packet.stream_index!=_streamIndex);
                return new Frame(&packet, _formatContext->streams[_streamIndex]->codec);
            }

            Frame * AVInputStream::getFrame(int frameIdx) {
                av_seek_frame(_formatContext,_streamIndex,frameIdx,AVSEEK_FLAG_ANY);
		        Frame * frame=getNextFrame();
                return frame;
            }
        }
    }
}

