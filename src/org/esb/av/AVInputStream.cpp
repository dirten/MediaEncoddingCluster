#include "AVInputStream.h"
#include "Codec.h"
#include "Frame.h"
#include <assert.h>
#include <iostream>
#include "introspec.h"
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
                if(avcodec_open(_codecContext, _codec)<0) {
                    fprintf(stderr, "avcodec_open failed in AVInputStream\n");
                }
                _intCodec=new Codec(_codecContext);
            }
            AVInputStream::~AVInputStream(){
                delete _intCodec;
                _intCodec=0;
                avcodec_close(_codecContext);
//                delete _codec;
                
//                av_free(_codec);
//                av_free(_formatContext);
//                av_free(_codecContext);
            }

            Codec * AVInputStream::getCodec() {
                return _intCodec;
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

            AVPacket AVInputStream::getNextPacket() {
                AVPacket packet;
                packet.data=NULL;
                int i=0;
                do {
//                    cout << ++i <<"durchlauf"<<endl;
                    if(packet.data!=NULL){
  //                      delete packet->data;
//                        delete packet;
                        av_free_packet(&packet);
                    }
                    if(av_read_packet(_formatContext, &packet)<0){
                        cout <<"Packet read failed"<<endl;
//                        return NULL;
                    }
                } while(packet.stream_index!=_streamIndex);
//                Frame * frame=new Frame(&packet, _formatContext->streams[_streamIndex]->codec);
//                av_free_packet(&packet);                
                return packet;
            }

            Packet * AVInputStream::getNextPacket2() {
                Packet *packet=new Packet();
                packet->data=NULL;
                int i=0;
                do {
//                    cout << ++i <<"durchlauf"<<endl;
                    if(packet->data!=NULL)
                        av_free_packet(packet);
                    if(av_read_packet(_formatContext, packet)<0){
                        cout <<"Packet read failed"<<endl;
                        return NULL;
                    }
                } while(packet->stream_index!=_streamIndex);
//                Frame * frame=new Frame(&packet, _formatContext->streams[_streamIndex]->codec);
//                av_free_packet(&packet);                
//                Packet * p=new Packet(&packet);
//                av_free_packet(&packet);
                return packet;
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
//                Frame * frame=new Frame(&packet, _formatContext->streams[_streamIndex]->codec);
                av_free_packet(&packet);                
//                return frame;

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

