#include "AVInputStream.h"
#include "Codec.h"
#include "Frame.h"
#include <assert.h>
#include <iostream>
using namespace std;
namespace org {
    namespace esb {
        namespace av {
            AVInputStream::AVInputStream(FormatInputStream * fis, int index) {
                _fis=fis;
                _formatContext=fis->getFormatContext();

                /*nicht schön*/
                _streamIndex=index;
                _avStream=_formatContext->streams[_streamIndex];
                _codecContext=_avStream->codec;
                _codec=avcodec_find_decoder(_codecContext->codec_id);
                if(_codec->capabilities & CODEC_CAP_TRUNCATED) {
//                    _codecContext->flags|=CODEC_FLAG_TRUNCATED;
                }
                if(avcodec_open(_codecContext, _codec)<0) {
                    fprintf(stderr, "avcodec_open failed\n");
                }
            }

            AVInputStream::AVInputStream(AVFormatContext * context, int streamIndex) {
                assert(context!=NULL);
                _formatContext=context;
                _streamIndex=streamIndex;
                _avStream=_formatContext->streams[_streamIndex];
                _codecContext=_formatContext->streams[_streamIndex]->codec;
                _codec=avcodec_find_decoder(_codecContext->codec_id);
                if(_codec->capabilities & CODEC_CAP_TRUNCATED) {
                    _codecContext->flags|=CODEC_FLAG_TRUNCATED;
                }
                if(avcodec_open(_codecContext, _codec)<0) {
                    fprintf(stderr, "avcodec_open failed\n");
                }
            }
            void AVInputStream::selectStreamIndex(int index){
                _streamIndex=index;
            }
            Codec * AVInputStream::getCodec() {
                return new Codec(_codecContext);
            }

            long AVInputStream::getDuration() {
                return _avStream->duration;
            }

            double AVInputStream::getTimeBase() {
                return av_q2d(_avStream->time_base);
            }

            long AVInputStream::getNumberFrames() {
                return _avStream->nb_frames;
            }

            AVPacket * AVInputStream::getNextPacket() {
                AVPacket * packet=new Packet();
                packet->data=NULL;
                int i=0;
                do {
//                    cout << ++i <<"durchlauf"<<endl;
                    if(packet.data!=NULL)
                        av_free_packet(&packet);
                    if(av_read_packet(_formatContext, &packet)<0){
                        cout <<"Packet read failed"<<endl;
                        return NULL;
                    }
                } while(packet.stream_index!=_streamIndex);
//                Frame * frame=new Frame(&packet, _formatContext->streams[_streamIndex]->codec);
//                av_free_packet(&packet);                
                return &packet;
            }
            Frame * AVInputStream::getNextFrame() {
                AVPacket packet;
                packet.data=NULL;
                int i=0;
                do {
//                    cout << ++i <<"durchlauf"<<endl;
                    if(packet.data!=NULL)
                        av_free_packet(&packet);
                    if(av_read_packet(_formatContext, &packet)<0){
                        cout <<"Packet read failed"<<endl;
                        return NULL;
                    }
                } while(packet.stream_index!=_streamIndex);
                Frame * frame=new Frame(&packet, _formatContext->streams[_streamIndex]->codec);
                av_free_packet(&packet);                
                return frame;

   /*
                AVPacket *packet=new AVPacket();
		        packet->data=NULL;
                do {
                    if(packet->data!=NULL)
                        av_free_packet(packet);
                    if(av_read_packet(_formatContext, packet)<0){
                        cout << "Fehler:"<<endl;
                        return NULL;
                    }
                } while(packet->stream_index!=_streamIndex);
                return new Frame(packet, _formatContext->streams[_streamIndex]->codec);
*/
            }

            Frame * AVInputStream::getFrame(int frameIdx) {
                av_seek_frame(_formatContext,_streamIndex,frameIdx,AVSEEK_FLAG_ANY);
                Frame * frame=getNextFrame();
                return frame;
            }
        }
    }
}

