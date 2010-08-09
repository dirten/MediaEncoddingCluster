#include "PreviewFrameServer.h"
#include "org/esb/av/StreamInfo.h"

namespace org{
  namespace esb{
    namespace web{
      using namespace org::esb::av;
      PreviewFrameServer::PreviewFrameServer(){

      }
      Ptr<org::esb::av::Frame> PreviewFrameServer::getFrame(){
        return _frame;
      }

      Ptr<org::esb::av::Decoder> PreviewFrameServer::getDecoder(){
        return _decoder;
      }
      bool PreviewFrameServer::isOpen(){
        return(_fis.get()!=NULL && _fis->isValid());
      }
      void PreviewFrameServer::setFileName(std::string filename){
        File infile(filename);
        _fis=Ptr<FormatInputStream>(new FormatInputStream(&infile));
        _pis=Ptr<PacketInputStream>(new PacketInputStream(_fis.get()));

        int c=_fis->getStreamCount();
        for(int a = 0;a<c;a++){
          _stream_info=Ptr<org::esb::av::StreamInfo>(_fis->getStreamInfo(a));
          if(_stream_info->getCodecType()==CODEC_TYPE_VIDEO){
            _decoder=Ptr<Decoder>(new Decoder(_fis->getAVStream(_stream_info->getIndex())));
            _decoder->open();
            LOGDEBUG("Video Stream found!");
            break;
          }
        }
        if(isOpen())
          jumpForward(JUMP_NONE);
      }
      void PreviewFrameServer::jumpForward(JUMP_WIDTH width){
        Packet * packet;
        Frame * frame=NULL;
        int a=1;
        while ((packet = _pis->readPacket()) != NULL ) {
          if(packet->getStreamIndex()==_stream_info->getIndex()){
            frame=_decoder->decode2(*packet);
            if(frame->isFinished()&& a>=width){
              _last_packet=Ptr<org::esb::av::Packet>(packet);
              break;
            }
            delete frame;
            frame=NULL;
            a++;
          }
          delete packet;
        }
        if(frame&&!frame->isFinished()){
          delete frame;
          _frame=Ptr<org::esb::av::Frame>(new org::esb::av::Frame());
        }else{
          _frame=Ptr<org::esb::av::Frame>(frame);
        }
      }

      void PreviewFrameServer::jumpPrevious(JUMP_WIDTH width){
        AVRational frame_rate;
        frame_rate.num=_stream_info->getFrameRate().den;
        frame_rate.den=_stream_info->getFrameRate().num;
        AVRational time_base=_stream_info->getTimeBase();
        int duration=av_rescale_q(1,frame_rate, time_base);
        int64_t seek_ts=_last_packet->getDts()-(duration*width);
        _fis->seek(_stream_info->getIndex(), seek_ts);



        Packet * packet;
        Frame * frame=NULL;
        int a=1;
        while ((packet = _pis->readPacket()) != NULL ) {
          if(packet->getStreamIndex()==_stream_info->getIndex()){
            frame=_decoder->decode2(*packet);
            if(frame->isFinished()&& packet->getDts()>=seek_ts){
              _last_packet=Ptr<org::esb::av::Packet>(packet);
              break;
            }
            delete frame;
            frame=NULL;
            a++;
          }
          delete packet;
        }
        if(frame&&!frame->isFinished()){
          delete frame;
          _frame=Ptr<org::esb::av::Frame>(new org::esb::av::Frame());
        }else{
          _frame=Ptr<org::esb::av::Frame>(frame);
        }

      }
    }
  }
}


